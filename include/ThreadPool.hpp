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

        ThreadPool(std::size_t threadsLimit) : m_threadsLimit(threadsLimit),
            m_threads{threadsLimit}
        {}

        ~ThreadPool();

        void run();

        void addTask(ThreadFunction task);

        void close(bool dontScheduleRemainingTasks = false);

    private:
        std::size_t m_threadsLimit;
        SafeQueue<ThreadFunction> m_queue;
        std::vector<std::thread> m_threads;
        bool m_dontScheduleRemainingTasks = false;
        bool m_isClosing = false;
        void consumer();
};

#endif /* !__THREAD_POOL_HPP_ */
