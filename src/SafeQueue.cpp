/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** SafeQueue.cpp
*/

#include "SafeQueue.hpp"

SafeQueue::SafeQueue()
{
}

SafeQueue::~SafeQueue()
{
}

void SafeQueue::push(int value)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_stack.push(value);
}

bool SafeQueue::tryPop(int &value)
{
    bool valuePopped = false;

    std::unique_lock<std::mutex> lock(m_mutex);
    if (0 < m_stack.size()) {
        value = m_stack.top();
        m_stack.pop();
        valuePopped = true;
    }
    return valuePopped;
}
