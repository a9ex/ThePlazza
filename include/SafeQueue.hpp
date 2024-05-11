/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** SafeQueue.hpp
*/

#ifndef __SAFEQUEUE_HPP_
    #define __SAFEQUEUE_HPP_
    #include <queue>
    #include <mutex>
    #include <condition_variable>

template <typename T>
class SafeQueue {
    public:
        SafeQueue()
        {}

        ~SafeQueue()
        {}

        void push(T value)
        {
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                m_queue.push(value);
            }
            m_cv.notify_one();
        }

        bool tryPop(T &value)
        {
            bool valuePopped = false;
            std::unique_lock<std::mutex> lock(m_mutex);

            if (0 < m_queue.size()) {
                value = m_queue.front();
                m_queue.pop();
                valuePopped = true;
            }
            return valuePopped;
        }

        T pop()
        {
            T value = 0;

            {
                std::unique_lock<std::mutex> lock(m_mutex);

                m_cv.wait(lock, [ & ] ( ) { return !this->empty(); });
                value = m_queue.front();
                m_queue.pop();
            }
            return value;
        }

        bool empty() const noexcept
        {
            return m_queue.empty();
        }

    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_cv;
};

#endif /* !__SAFEQUEUE_HPP_ */
