/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** SafeQueue.hpp
*/

#ifndef __ISAFEQUEUE_HPP_
    #define __ISAFEQUEUE_HPP_
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
        void push(int value);
        bool tryPop(int &value);
    private:
        std::stack<int> m_stack;
        std::mutex m_mutex;
};

#endif /* !__ISAFEQUEUE_HPP_ */
