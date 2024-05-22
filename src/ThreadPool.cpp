/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** ThreadPool.cpp
*/

#include <thread>
#include "ThreadPool.hpp"

ThreadPool::~ThreadPool()
{
    if (!m_isClosing)
        this->close();
}

void ThreadPool::run()
{
    std::size_t i = 0;

    for (; i < m_threadsLimit; ++i)
        m_threads[i] = std::thread([&] { this->consumer(); });
}

void ThreadPool::addTask(ThreadFunction task)
{
    if (!m_isClosing)
        m_queue.push(task);
}

void ThreadPool::close(bool dontScheduleRemainingTasks)
{
    std::size_t i = 0;

    m_isClosing = true;
    m_dontScheduleRemainingTasks = dontScheduleRemainingTasks;
    for (i = 0; i < m_threadsLimit; ++i)
        m_threads[i].join();
}

void ThreadPool::consumer()
{
    ThreadFunction task;

    while (!m_isClosing ||
        (m_isClosing && m_dontScheduleRemainingTasks && !m_queue.empty()))
        if (m_queue.tryPop(task))
            task();
}
