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

    for (; i < m_threadsLimit && !m_queue.empty(); ++i)
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
    int canPop = true;
    ThreadFunction task;

    while (!m_queue.empty() && canPop) {
        canPop = m_queue.tryPop(task);
        if (canPop)
            task();
    }
}
