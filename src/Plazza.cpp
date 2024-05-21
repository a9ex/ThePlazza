/*
** EPITECH PROJECT, 2024
** Plazza.cpp
** File description:
** Plazza.cpp
*/

#include "Plazza.hpp"
#include <iostream>

plazza::Kitchen::Kitchen(std::string id)
    : _id(id)
{
    this->_process = std::optional(process::ForkProcess([this]() {
        plazza::LocalKitchen local_kitchen(this->_id);
    }));
    this->_output_pipe = std::optional(file::Pipe(
        "fifo_" + this->_id,
        file::Pipe::Mode::WRITE,
        true
    ));
    this->_input_pipe = std::optional(file::Pipe(
        "fifo_" + this->_id,
        file::Pipe::Mode::READ,
        false
    ));
    std::cout << "Kitchen " << this->_id << " is ready" << std::endl;
}

plazza::LocalKitchen::LocalKitchen(std::string id)
    : _id(id),
      _input_pipe("fifo_" + id, file::Pipe::Mode::READ, false),
      _output_pipe("fifo_" + id, file::Pipe::Mode::WRITE, false)
{
    std::cout << "Local kitchen " << this->_id << " is ready" << std::endl;
}
