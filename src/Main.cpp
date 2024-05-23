/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Main.cpp
*/

#include <iostream>
#include "Process.hpp"
#include "File.hpp"
#include "Plazza.hpp"
#include "ThreadPool.hpp"
#include "Packet.hpp"

#ifndef CRITERION

int main(void)
{
    plazza::Holders holders;
    plazza::Kitchen kitchen(holders, "1");

    return 0;
}

#endif
