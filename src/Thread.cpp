/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Thread.cpp
*/

#include "Thread.hpp"

Thread::Thread()
{
    m_status = ThreadStatus::ThreadDead;
}

Thread::~Thread()
{
    if (ThreadStatus::ThreadDead != m_status) {
        this->detach();
        this->cancel();
    }
    m_status = ThreadStatus::ThreadDead;
}

void Thread::start(ThreadFunction function, void *arg)
{
    if (-1 == pthread_create(&m_thread, nullptr, function, arg))
        throw "Unable to start the thread";
    m_status = ThreadStatus::ThreadStarted;
}

void Thread::cancel()
{
    if (-1 == pthread_cancel(m_thread))
        throw "Unable to cancel the thread";
    m_status = ThreadStatus::ThreadDead;
}

void *Thread::join()
{
    void *return_value = nullptr;

    m_status = ThreadStatus::ThreadRunning;
    if (-1 == pthread_join(m_thread, &return_value))
        throw "Unable to join the thread";
    this->detach();
    m_status = ThreadStatus::ThreadDead;
    return return_value;
}

void Thread::detach()
{
    if (-1 == pthread_detach(m_thread))
        throw "Unable to detach the thread";
    m_status = ThreadStatus::ThreadDead;
}
