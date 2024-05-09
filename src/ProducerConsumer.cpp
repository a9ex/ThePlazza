/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** ProducerConsumer.cpp
*/

#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include "SafeQueue.hpp"

void *Consumer(void *arg)
{
    int tries = 0;
    int value = 0;
    SafeQueue *queue = (SafeQueue *) arg;

    while (5 > tries) {
        if (false == queue->tryPop(value)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            ++tries;
            continue;
        }
        tries = 0;
        std::cout << value << std::endl;
    }
    return nullptr;
}

void *Producer(void *arg)
{
    int i = 0;
    std::uniform_int_distribution<std::mt19937::result_type> range(1, 9);
    SafeQueue *queue = (SafeQueue *) arg;

    for (; 10000 > i; ++i) {
        std::random_device dev;
        std::mt19937 rng(dev());
        queue->push(range(rng));
    }
    return nullptr;
}
