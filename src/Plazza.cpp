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

plazza::Kitchen::Kitchen(std::string id)
    : _id(id)
{
    this->_id = id;
    this->_process = process::ForkProcess([this] {
        LocalKitchen local_kitchen(this->_id);
    });

    // Create pipes
    std::cout << "Kitchen 1" << std::endl;
    // this->_input_pipe = std::optional(
    //     file::Pipe(
    //         "kitchen_pipe_" + this->_id + "_input",
    //         file::Pipe::Mode::WRITE
    //     )
    // );
    // std::cout << "Kitchen 2" << std::endl;
    // this->_output_pipe = std::optional(
    //     file::Pipe(
    //         "kitchen_pipe_" + this->_id + "_output",
    //         file::Pipe::Mode::READ
    //     )
    // );

    mkfifo("kitchen_pipe_1_input", 0777);
    int fd = open("kitchen_pipe_1_input", O_WRONLY);

    //this->_input_pipe->writeBuf(std::vector<char>{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'});
    std::cout << "Parent fd: " << fd << std::endl;
    write(fd, "A", 1);

    // comm::PingPacket packet(69);
    // packet >> *this->_input_pipe;

    std::cout << "Written message" << std::endl;

    // Wait for child process to finish
    this->_process->wait();
}

plazza::LocalKitchen::LocalKitchen(std::string id)
    : _id(id)
{
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "LocalKitchen 1" << std::endl;
    int fd = open("kitchen_pipe_1_input", O_RDONLY);
    // this->_input_pipe = std::optional(
    //     file::Pipe(
    //         "kitchen_pipe_" + this->_id + "_input",
    //         file::Pipe::Mode::READ
    //     )
    // );
    // int fd;
    std::cout << "LocalKitchen 2" << std::endl;
    // this->_output_pipe = std::optional(
    //     file::Pipe(
    //         "kitchen_pipe_" + this->_id + "_output",
    //         file::Pipe::Mode::WRITE
    //     )
    // );

    // Read from input pipe
    char buffer[1] = {0};

    std::this_thread::sleep_for(std::chrono::seconds(1));

    ssize_t s = read(fd, buffer, 1);

    std::cout << "Child fd:" << fd << std::endl;
    perror("coucou");

    std::cout << "Received message length: " << s << std::endl;
    std::cout << "Received message: " << buffer << std::endl;
//    while (true) {
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//         char buffer[4096] = {0};

//         std::size_t s = read(this->_input_pipe->getFd(), buffer, 4096);
//         if (s == 0)
//             continue;
//         std::cout << "Received message: " << buffer << std::endl;
//    }
}
