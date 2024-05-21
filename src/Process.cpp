/*
** EPITECH PROJECT, 2024
** Process.cpp
** File description:
** Process.cpp
*/

#include <iostream>
#include "Process.hpp"

process::ForkProcess::ForkProcess(std::function<void()> child_func)
{
    this->_pid = fork();

    if (this->_pid == 0) {
        child_func();
        std::exit(0);
    }
}
