/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Mutex.hpp
*/

#ifndef __MUTEX_HPP_
    #define __MUTEX_HPP_
    #include <pthread.h>

class Mutex {
    private:
        pthread_mutex_t m_mutex;
    public:
        Mutex();
        ~Mutex();
        void lock();
        void unlock();
        void trylock();
};

#endif /* !__MUTEX_HPP_ */
