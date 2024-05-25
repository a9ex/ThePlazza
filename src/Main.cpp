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
#include "Pizza.hpp"
#include <thread>
#include <chrono>

#ifndef CRITERION

int main(void)
{
    plazza::Holders holders;

    int numberOfKitchens = 1;
    std::vector<std::shared_ptr<plazza::Kitchen>> kitchens;
    for (int i = 0; i < numberOfKitchens; i++) {
        plazza::KitchenSpec spec(std::to_string(i), 3);
        kitchens.push_back(std::make_shared<plazza::Kitchen>(holders, spec));
    }

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
