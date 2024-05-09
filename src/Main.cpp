/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Main.cpp
*/

#include <iostream>
#include <thread>
#include "ProducerConsumer.hpp"
#include "SafeQueue.hpp"

#ifndef CRITERION

int main(void)
{
    SafeQueue queue;
    int i = 0;
    const int producersNumber = 3;
    const int consumersNumber = 100;
    std::thread producers[producersNumber];
    std::thread consumers[consumersNumber];

    for (; producersNumber > i; ++i)
        producers[i] = std::thread(Producer, &queue);
    for (i = 0; consumersNumber > i; ++i)
        consumers[i] = std::thread(Consumer, &queue);
    for (i = 0; producersNumber > i; ++i)
        producers[i].join();
    for (i = 0; consumersNumber > i; ++i)
        consumers[i].join();
    std::cout << "stack is empty: " << queue.tryPop(i) << std::endl;
}

#endif
