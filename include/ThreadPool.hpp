/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** ThreadPool.hpp
*/

#ifndef __THREAD_POOL_HPP_
    #define __THREAD_POOL_HPP_
    #include <functional>
    #include "SafeQueue.hpp"

class ThreadPool {
    public:

        using ThreadFunction = std::function<void()>;
        using ThreadFunctionCallback = std::function<void(ThreadFunction)>;

        ThreadPool(std::size_t threadsLimit,
            SafeQueue<ThreadFunction> &queue) : m_threadsLimit(threadsLimit),
                m_queue(queue)
        {}

        ~ThreadPool()
        {}

        void run();

        void producer(ThreadFunction task);

    private:
        std::size_t m_threadsLimit;
        SafeQueue<ThreadFunction> &m_queue;

        void consumer();
};

#endif /* !__THREAD_POOL_HPP_ */
