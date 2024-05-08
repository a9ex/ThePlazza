/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Mutex.hpp
*/

#ifndef __IMUTEX_HPP_
    #define __IMUTEX_HPP_
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
        void lock();
        void unlock();
        void trylock();
};

#endif /* !__IMUTEX_HPP_ */
