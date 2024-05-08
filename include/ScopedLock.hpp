/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** ScopedLock.hpp
*/

#ifndef __ISCOPEDLOCK_HPP_
    #define __ISCOPEDLOCK_HPP_
    #include "Mutex.hpp"

class IScopedLock {
    public:
        virtual ~IScopedLock() = default;
};

class ScopedLock : public IScopedLock {
    private:
        Mutex &m_mutex;
    public:
        ScopedLock(Mutex &mutex);
        ~ScopedLock();
};

#endif /* !__ISCOPEDLOCK_HPP_ */
