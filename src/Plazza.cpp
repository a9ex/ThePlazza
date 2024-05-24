/*
** EPITECH PROJECT, 2024
** Plazza.cpp
** File description:
** Plazza.cpp
*/

#include "Plazza.hpp"
#include "Packet.hpp"
#include <iostream>
#include <thread>
#include <chrono>

plazza::Kitchen::Kitchen(plazza::Holders &holders, plazza::KitchenSpec spec)
    : _spec(spec)
{
    this->_process = std::make_unique<process::ForkProcess>([this, &holders] {
        LocalKitchen local_kitchen(holders, this->_spec);
    });

    std::cout << "Creating Kitchen named '" << this->_spec.getId() << "'" << std::endl;

    // Create pipes
    this->_input_pipe = std::make_unique<file::Pipe>(
        "kitchen_pipe_" + this->_spec.getId() + "_input",
        file::Pipe::Mode::WRITE);

    this->_output_pipe = std::make_unique<file::Pipe>(
        "kitchen_pipe_" + this->_spec.getId() + "_output",
        file::Pipe::Mode::READ
    );

    // Create packet receiver thread

    bool exit_thread = false;
    auto thread = std::thread([this, &holders, &exit_thread] {
        while (!exit_thread) {
            std::vector<std::shared_ptr<comm::Packet>> packets = holders.getPacketHandler().constructPackets(this->_output_pipe->readBuf());
            if (!packets.empty()) {
                holders.addMainRunnable([packets, &holders] {
                    std::cout << "HEYY" << std::endl;
                    for (auto &packet : packets)
                        holders.getReceptionDispatcher().onPacketReceived(*packet);
                });
            }
        }
    });

    thread.detach();
    exit_thread = true;
}

plazza::LocalKitchen::LocalKitchen(plazza::Holders &holders, plazza::KitchenSpec spec)
    : _spec(spec)
{
    this->_input_pipe = std::make_unique<file::Pipe>(
        "kitchen_pipe_" + this->_spec.getId() + "_input",
        file::Pipe::Mode::READ
    );

    this->_output_pipe = std::make_unique<file::Pipe>(
        "kitchen_pipe_" + this->_spec.getId() + "_output",
        file::Pipe::Mode::WRITE
    );

    // std::unique_ptr<comm::Packet> packet =
    //     holders.getPacketHandler().constructPacket(this->_input_pipe->readBuf());

    // auto ping_packet = dynamic_cast<comm::PingPacket *>(packet.get());
    // std::cout << "Received message: " << ping_packet->getI() << std::endl;

    comm::PingPacket packet(420);
    packet >> *this->_output_pipe;

    comm::PingPacket p2(420);
    p2 >> *this->_output_pipe;

    // Wait 1s
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Fork finished" << std::endl;
}

void plazza::ReceptionDispatcher::onPacketReceived(comm::Packet &packet)
{
    std::cout << "Received packet" << std::endl;
}
