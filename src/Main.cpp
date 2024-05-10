/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Main.cpp
*/

#include <unistd.h>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include "SafeQueue.hpp"
#include "ThreadPool.hpp"

#ifndef CRITERION

void *printRandomNumber(void *arg)
{
    Thread *this_thread = (Thread *) arg;

    std::uniform_int_distribution<std::mt19937::result_type> range(1, 9);
    std::random_device dev;
    std::mt19937 rng(dev());
    this_thread->sleep(1);
    std::cout << range(rng) << std::endl;
    return nullptr;
}

int main(void)
{
    std::size_t i = 0;
    SafeQueue<ThreadFunction> queue;
    ThreadPool pool(8, queue);

    for (; i < 1000; ++i)
        queue.push(printRandomNumber);
    pool.run();
    return 0;
}

#endif
