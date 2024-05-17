/*
** EPITECH PROJECT, 2024
** Process.cpp
** File description:
** Process.cpp
*/

#include "Process.hpp"

process::ForkProcess::ForkProcess()
{
    this->_pid = fork();
}
