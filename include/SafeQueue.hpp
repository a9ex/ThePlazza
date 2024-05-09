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

class ISafeQueue {
    public:
        virtual ~ISafeQueue() = default;
        virtual void push(int value) = 0;
        virtual bool tryPop(int &value) = 0;
};

class SafeQueue : public ISafeQueue {
    public:
        SafeQueue();
        ~SafeQueue();
        void push(int value) override;
        bool tryPop(int &value) override;
    private:
        std::stack<int> m_stack;
        std::mutex m_mutex;
};

#endif /* !__SAFEQUEUE_HPP_ */
