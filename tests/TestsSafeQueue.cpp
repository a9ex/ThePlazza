/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** TestsSafeQueue.cpp
*/

#include "Thread.hpp"
#include "SafeQueue.hpp"
#include <criterion/criterion.h>

static SafeQueue queue;
const int numberOfThreads = 32;

static void *popStack(void *arg)
{
    if (1 == queue.tryPop(* (int *) arg)) {
        * (int *) arg = -1;
        return nullptr;
    }
    return nullptr;
}

static void *pushStack(void *arg)
{
    queue.push(*(int *) arg);
    return nullptr;
}

Test(SafeQueue, test_impl)
{
    int i = 0;
    Thread pushThreads[numberOfThreads];
    int expected[numberOfThreads] = { 0 };

    for (; numberOfThreads > i; ++i)
        expected[i] = i;
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i].start(pushStack, &(expected[i]));
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i].join();
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i].start(popStack, &(expected[i]));
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i].join();
    for (i = 0; numberOfThreads > i; ++i)
        cr_assert_eq(expected[i], -1);
}
