/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** TestsSafeQueue.cpp
*/

#include <thread>
#include <criterion/criterion.h>
#include "SafeQueue.hpp"

static SafeQueue<int> queue;
const int numberOfThreads = 32;

static void *tryPopStack(void *arg)
{
    if (1 == queue.tryPop(* (int *) arg)) {
        * (int *) arg = -1;
        return nullptr;
    }
    return nullptr;
}

static void *popStack(void *arg)
{
    * (int *) arg = queue.pop();
    return arg;
}

static void *pushStack(void *arg)
{
    queue.push(*(int *) arg);
    return nullptr;
}

Test(SafeQueue, test_try_pop_impl)
{
    int i = 0;
    std::thread pushThreads[numberOfThreads];
    int expected[numberOfThreads] = { 0 };

    for (; numberOfThreads > i; ++i)
        expected[i] = i;
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i] = std::thread(pushStack, &(expected[i]));
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i].join();
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i] = std::thread(tryPopStack, &(expected[i]));
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i].join();
    for (i = 0; numberOfThreads > i; ++i)
        cr_assert_eq(expected[i], -1);
}

Test(SafeQueue, test_pop_impl)
{
    int i = 0;
    std::thread pushThreads[numberOfThreads];
    int expected[numberOfThreads] = { 0 };
    int current[numberOfThreads] = { 0 };

    memset(current, 0, numberOfThreads * sizeof(int));
    for (; numberOfThreads > i; ++i)
        expected[i] = i;
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i] = std::thread(pushStack, &(expected[i]));
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i].join();
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i] = std::thread(popStack, &(current[i]));
    for (i = 0; numberOfThreads > i; ++i)
        pushThreads[i].join();
    for (i = 0; numberOfThreads > i; ++i)
        cr_assert_neq(current[i], -1);
}
