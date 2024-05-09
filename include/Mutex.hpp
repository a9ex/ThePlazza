/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Mutex.hpp
*/

#ifndef __MUTEX_HPP_
    #define __MUTEX_HPP_
    #include <pthread.h>

class IMutex {
    public:
        virtual ~IMutex() = default;
        virtual void lock() = 0;
        virtual void unlock() = 0;
        virtual void trylock() = 0;
};

class Mutex : public IMutex {
    private:
        pthread_mutex_t m_mutex;
    public:
        Mutex();
        ~Mutex();
        void lock() override;
        void unlock() override;
        void trylock() override;
};

#endif /* !__MUTEX_HPP_ */
