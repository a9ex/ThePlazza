/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** ThreadPool.hpp
*/

#ifndef __THREAD_POOL_HPP_
    #define __THREAD_POOL_HPP_
    #include "SafeQueue.hpp"
    #include "Thread.hpp"

class ThreadPool {
    public:
        ThreadPool(std::size_t threadsLimit,
            SafeQueue<ThreadFunction> &queue)
        : m_threadsLimit(threadsLimit),
            m_queue(queue)
        {}

        ~ThreadPool()
        {}

        void run()
        {
            std::size_t i = 0;
            std::size_t j = 0;
            Thread *threads = new Thread[m_threadsLimit];

            while (!m_queue.empty()) {
                for (i = 0; i < m_threadsLimit && !m_queue.empty(); ++i)
                    threads[i].start(m_queue.pop(), &(threads[i]));
                for (j = 0; j < i; ++j)
                    threads[j].join();
            }
            delete[] threads;
        }

    private:
        std::size_t m_threadsLimit;
        SafeQueue<ThreadFunction> &m_queue;
};

#endif /* !__THREAD_POOL_HPP_ */
