/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** ThreadPool.cpp
*/

#include <thread>
#include "ThreadPool.hpp"

void ThreadPool::run()
{
    std::size_t i = 0;
    std::thread *threads = new std::thread[m_threadsLimit];

    for (; i < m_threadsLimit; ++i)
        threads[i] = std::thread([&] { this->consumer(); });
    for (i = 0; i < m_threadsLimit; ++i)
        threads[i].join();
    delete[] threads;
}

void ThreadPool::addTask(ThreadFunction task)
{
    m_queue.push(task);
}

void ThreadPool::consumer()
{
    bool keepConsuming = true;
    ThreadFunction task;

    do {
        keepConsuming = m_queue.tryPop(task);
        if (keepConsuming)
            task();
    } while (keepConsuming);
}
