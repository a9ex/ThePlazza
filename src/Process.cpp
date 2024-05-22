/*
** EPITECH PROJECT, 2024
** Process.cpp
** File description:
** Process.cpp
*/

#include <iostream>
#include "Process.hpp"
#include <sys/wait.h>
#include <sys/types.h>

process::ForkProcess::ForkProcess(std::function<void()> child_func)
{
    this->_pid = fork();

    if (this->_pid == 0) {
        child_func();
        std::exit(0);
    }
}

void process::ForkProcess::wait()
{
    waitpid(this->_pid, NULL, 0);
}
