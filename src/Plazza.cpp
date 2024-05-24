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

    auto thread = std::thread([this, &holders] {
        while (true) {
            std::vector<std::shared_ptr<comm::Packet>> packets = holders.getPacketHandler().constructPackets(this->_output_pipe->readBuf());
            if (!packets.empty()) {
                holders.addMainRunnable([this, packets, &holders] { // Capture 'this' in the lambda capture list
                    for (auto &packet : packets)
                        PacketReceiver().onReceive(holders, *this, *packet);
                });
            }
        }
    });

    thread.detach();
}

plazza::LocalKitchen::LocalKitchen(plazza::Holders &holders, plazza::KitchenSpec spec)
    : _spec(spec)
{
    // Init pipes
    this->_input_pipe = std::make_unique<file::Pipe>(
        "kitchen_pipe_" + this->_spec.getId() + "_input",
        file::Pipe::Mode::READ
    );

    this->_output_pipe = std::make_unique<file::Pipe>(
        "kitchen_pipe_" + this->_spec.getId() + "_output",
        file::Pipe::Mode::WRITE
    );

    // Init thread pool
    // We need to add 1 thread for the reception dispatcher
    std::size_t thread_count = this->_spec.getOvens() + 1;
    this->_thread_pool = std::make_unique<ThreadPool>(this->_spec.getOvens() + 1);
    this->_thread_pool->run();

    this->_thread_pool->addTask([this] {
        comm::PacketHandler packet_handler;
        while (true) {
            std::vector<std::shared_ptr<comm::Packet>> packets = packet_handler.constructPackets(this->_input_pipe->readBuf());
            if (!packets.empty()) {
                for (auto &packet : packets)
                    this->onPacketReceived(*packet);
            }
        }
    });

    comm::PingPacket packet(420);
    packet >> *this->_output_pipe;

    comm::PingPacket p2(420);
    p2 >> *this->_output_pipe;

    // Wait 1s
    std::this_thread::sleep_for(std::chrono::seconds(1));

    this->_thread_pool->close(false);

    std::cout << "Fork finished" << std::endl;
}

void plazza::PacketReceiver::onReceive(plazza::Holders &holders,
    plazza::Kitchen &kitchen,
    comm::Packet &packet)
{
    std::cout << "RECEPTION Packet received" << std::endl;
    comm::PingPacket ping_packet(6969);
    kitchen << ping_packet;
}

void plazza::LocalKitchen::onPacketReceived(comm::Packet &packet)
{
    std::cout << "LocalKitchen received packet" << std::endl;
}

std::shared_ptr<plazza::Kitchen> plazza::PizzaBalancer::balancePizza(plazza::Pizza &pizza,
    std::vector<std::shared_ptr<plazza::Kitchen>> kitchens)
{
    std::shared_ptr<plazza::Kitchen> kitchen{nullptr};

    // Remove kitchens that are not available
    kitchens.erase(std::remove_if(kitchens.begin(), kitchens.end(), [](std::shared_ptr<plazza::Kitchen> &kitchen) {
        return kitchen->getSpec().getOvens() == 0;
    }), kitchens.end());

    // Sort by kitchen with the minimum amount of ovens
    std::sort(kitchens.begin(), kitchens.end(), [](std::shared_ptr<plazza::Kitchen> &a, std::shared_ptr<plazza::Kitchen> &b) {
        return a->getSpec().getOvens() < b->getSpec().getOvens();
    });

    // TODO: Malo: Sort (not remove) by kitchen with number of ingredients
    // Design idea: Count the number of matching ingredients available in the kitchen and sort it from most to least

    // Get the first kitchen
    if (!kitchens.empty())
        kitchen = kitchens.front();

    return kitchen;
}
