/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Mutex.cpp
*/

#include <iostream>
#include "Mutex.hpp"

Mutex::Mutex()
{
    if (-1 == pthread_mutex_init(&m_mutex, nullptr))
        throw "Unable to initialise the mutex";
}

Mutex::~Mutex()
{
    if (-1 == pthread_mutex_destroy(&m_mutex))
        std::cerr << "Unable to destroy the mutex" << std::endl;
}

void Mutex::lock()
{
    if (-1 == pthread_mutex_lock(&m_mutex))
        throw "Unable to lock the mutex";
}

void Mutex::unlock()
{
    if (-1 == pthread_mutex_unlock(&m_mutex))
        throw "Unable to unlock the mutex";
}

void Mutex::trylock()
{
    if (-1 == pthread_mutex_trylock(&m_mutex))
        throw "Unable to trylock the mutex";
}
