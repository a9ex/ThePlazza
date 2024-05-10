/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** SafeQueue.hpp
*/

#ifndef __SAFEQUEUE_HPP_
    #define __SAFEQUEUE_HPP_
    #include <stack>
    #include <mutex>
    #include <condition_variable>

template <typename T>
class ISafeQueue {
    public:
        virtual ~ISafeQueue() = default;
        virtual void push(T value) = 0;
        virtual bool tryPop(T &value) = 0;
        virtual T pop() = 0;
        virtual bool empty() const = 0;
};

template <typename T>
class SafeQueue : public ISafeQueue<T> {
    public:
        SafeQueue()
        {}

        ~SafeQueue()
        {}

        void push(T value) override
        {
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                m_stack.push(value);
            }
            m_cv.notify_one();
        }

        bool tryPop(T &value) override
        {
            bool valuePopped = false;
            std::unique_lock<std::mutex> lock(m_mutex);

            if (0 < m_stack.size()) {
                value = m_stack.top();
                m_stack.pop();
                valuePopped = true;
            }
            return valuePopped;
        }

        T pop() override
        {
            T value = 0;

            {
                std::unique_lock<std::mutex> lock(m_mutex);

                m_cv.wait(lock, [ & ] ( ) { return !this->empty(); });
                value = m_stack.top();
                m_stack.pop();
            }
            return value;
        }

        bool empty() const override
        {
            return m_stack.empty();
        }

    private:
        std::stack<T> m_stack;
        std::mutex m_mutex;
        std::condition_variable m_cv;
};

#endif /* !__SAFEQUEUE_HPP_ */
