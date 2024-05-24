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
#include <thread>
#include <chrono>

#ifndef CRITERION

int main(void)
{
    plazza::Holders holders;
    plazza::Kitchen kitchen(holders, "1");

    auto &runnabled_queue = holders.getMainThreadRunnables();
    while (true) {
        holders.getRunnablesSem().acquire();
        while (runnabled_queue.size() > 0) {
            runnabled_queue.front()();
            runnabled_queue.pop_front();
        }
        holders.getRunnablesSem().release();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

#endif
