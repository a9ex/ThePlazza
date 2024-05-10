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
    int value = 0;
    SafeQueue<int> *queue = (SafeQueue<int> *) arg;

    while (1) {
        value = queue->pop();
        std::cout << value << std::endl;
    }
    return nullptr;
}

void *Producer(void *arg)
{
    int i = 0;
    std::uniform_int_distribution<std::mt19937::result_type> range(1, 9);
    SafeQueue<int> *queue = (SafeQueue<int> *) arg;

    for (; 10000 > i; ++i) {
        std::random_device dev;
        std::mt19937 rng(dev());
        queue->push(range(rng));
    }
    return nullptr;
}
