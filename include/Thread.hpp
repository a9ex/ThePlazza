/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Thread.hpp
*/

#ifndef __THREAD_HPP_
    #define __THREAD_HPP_
    #include <iostream>
    #include <pthread.h>
    #include <time.h>

using ThreadFunction = void *(*)(void *);

class Thread  {
    public:

        Thread() : m_status(ThreadStatus::ThreadDead)
        {
            if (-1 == pthread_mutex_init(&m_mutex, nullptr))
                throw "Unable to init the thread's mutex";
            if (-1 == pthread_cond_init(&m_cond, nullptr))
                throw "Unable to init the thread's condition";
        }

        ~Thread()
        {
            if (ThreadStatus::ThreadDead != m_status) {
                this->detach();
                this->cancel();
            }
            if (-1 == pthread_mutex_destroy(&m_mutex))
                std::cerr << "Unable to destroy the thread's mutex"
                    << std::endl;
            if (-1 == pthread_cond_destroy(&m_cond))
                std::cerr << "Unable to destroy the thread's condition"
                    << std::endl;
            m_status = ThreadStatus::ThreadDead;
        }

        void start(ThreadFunction function, void *arg)
        {
            if (ThreadStatus::ThreadDead != m_status)
                throw "The thread is already started";
            if (-1 == pthread_create(&m_thread, nullptr, function, arg))
                throw "Unable to start the thread";
            m_status = ThreadStatus::ThreadStarted;
        }

        void cancel()
        {
            if (-1 == pthread_cancel(m_thread))
                throw "Unable to cancel the thread";
            if (-1 == pthread_cond_destroy(&m_cond))
                throw "Unable to destroy the thread's condition";
            m_status = ThreadStatus::ThreadDead;
        }

        void *join()
        {
            void *return_value = nullptr;

            if (ThreadStatus::ThreadDead == m_status)
                throw "The thread is already dead";
            m_status = ThreadStatus::ThreadRunning;
            if (-1 == pthread_join(m_thread, &return_value))
                throw "Unable to join the thread";
            this->detach();
            m_status = ThreadStatus::ThreadDead;
            return return_value;
        }

        void detach()
        {
            if (-1 == pthread_detach(m_thread))
                throw "Unable to detach the thread";
        }

        void sleep(long seconds)
        {
            struct timespec timeout;

            if (-1 == clock_gettime(CLOCK_REALTIME, &timeout))
                throw "Unable to get the CLOCK_REALTIME data";
            timeout.tv_sec += seconds;
            m_status = ThreadStatus::ThreadSleeping;
            if (-1 == pthread_cond_timedwait(&m_cond, &m_mutex, &timeout))
                throw "Unable to sleep the thread";
            m_status = ThreadStatus::ThreadRunning;
        }

    private:
        typedef enum EThreadStatus {
            ThreadStarted,
            ThreadRunning,
            ThreadSleeping,
            ThreadDead,
        } ThreadStatus;

        pthread_t m_thread;
        pthread_mutex_t m_mutex;
        pthread_cond_t m_cond;
        ThreadStatus m_status;
};

#endif /* !__THREAD_HPP_ */
