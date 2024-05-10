/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** TestsMutex.cpp
*/

#include "Mutex.hpp"
#include "Thread.hpp"
#include <criterion/criterion.h>

static Mutex mutex;
static const int numberOfThreads = 32;
static const int iterations = 100;

static void *incrementNumber(void *arg)
{
    int i = 0;

    for (; iterations > i; ++i) {
        mutex.lock();
        ++(*(int *)arg);
        mutex.unlock();
    }
    return nullptr;
}

Test(Mutex, test_impl)
{
    int i = 0;
    int nb = 0;
    Thread threads[numberOfThreads];

    for (; numberOfThreads > i; ++i)
        threads[i].start(incrementNumber, &nb);
    for (i = 0; numberOfThreads > i; ++i)
        threads[i].join();
    cr_assert_eq(nb, numberOfThreads * iterations);
}
