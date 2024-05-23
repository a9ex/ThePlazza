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

plazza::Kitchen::Kitchen(plazza::Holders &holders, std::string id)
    : _id(id)
{
    this->_id = id;
    this->_process = process::ForkProcess([this, &holders] {
        LocalKitchen local_kitchen(holders, this->_id);
    });

    std::cout << "Creating Kitchen named '" << id << "'" << std::endl;

    // Create pipes
    auto ip = file::Pipe("kitchen_pipe_" + this->_id + "_input",
        file::Pipe::Mode::WRITE);
    this->_input_pipe = std::move(ip);

    auto op = file::Pipe("kitchen_pipe_" + this->_id + "_output",
        file::Pipe::Mode::READ);
    this->_output_pipe = std::move(op);

    comm::PingPacket packet(69);
    packet >> *this->_input_pipe;

    std::cout << "Written message" << std::endl;

    // Wait for child process to finish
    this->_process->wait();
}

plazza::LocalKitchen::LocalKitchen(plazza::Holders &holders, std::string id)
    : _id(id)
{
    auto ip = file::Pipe("kitchen_pipe_" + this->_id + "_input",
        file::Pipe::Mode::READ);
    this->_input_pipe = std::move(ip);

    auto op = file::Pipe("kitchen_pipe_" + this->_id + "_output",
        file::Pipe::Mode::WRITE);
    this->_output_pipe = std::move(op);

    std::unique_ptr<comm::Packet> packet =
        holders.getPacketHandler().constructPacket(this->_input_pipe->readBuf());

    auto ping_packet = dynamic_cast<comm::PingPacket *>(packet.get());
    std::cout << "Received message: " << ping_packet->getI() << std::endl;
}
