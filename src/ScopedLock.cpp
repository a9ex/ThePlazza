/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** ScopedLock.cpp
*/

#include "ScopedLock.hpp"

ScopedLock::ScopedLock(Mutex &mutex) : m_mutex(mutex)
{
    m_mutex.lock();
}

ScopedLock::~ScopedLock()
{
    m_mutex.unlock();
}
