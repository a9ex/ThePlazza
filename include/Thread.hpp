/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Thread.hpp
*/

#ifndef __THREAD_HPP_
    #define __THREAD_HPP_
    #include <pthread.h>
    #include <time.h>

using ThreadFunction = void *(*)(void *);

class IThread {
    public:
        virtual ~IThread() = default;
        virtual bool isStarted() = 0;
        virtual bool isRunning() = 0;
        virtual bool isSleeping() = 0;
        virtual bool isDead() = 0;
        virtual void start(ThreadFunction function, void *arg) = 0;
        virtual void cancel() = 0;
        virtual void *join() = 0;
        virtual void detach() = 0;
        virtual void sleep(long seconds) = 0;
};

class Thread : public IThread {
    public:

        Thread() : m_status(ThreadStatus::ThreadDead)
        {}

        ~Thread()
        {
            if (ThreadStatus::ThreadDead != m_status) {
                this->detach();
                this->cancel();
            }
            setStatus(ThreadStatus::ThreadDead);
        }

        bool isStarted() override
        {
            return ThreadStatus::ThreadStarted == getStatus();
        }

        bool isRunning() override
        {
            return ThreadStatus::ThreadRunning == getStatus();
        }

        bool isSleeping() override
        {
            return ThreadStatus::ThreadSleeping == getStatus();
        }

        bool isDead() override
        {
            return ThreadStatus::ThreadDead == getStatus();
        }

        void start(ThreadFunction function, void *arg) override
        {
            if (ThreadStatus::ThreadDead != m_status)
                throw "The thread is already started";
            if (-1 == pthread_mutex_init(&m_mutex, nullptr))
                throw "Unable to init the thread's mutex";
            if (-1 == pthread_cond_init(&m_cond, nullptr))
                throw "Unable to init the thread's condition";
            if (-1 == pthread_create(&m_thread, nullptr, function, arg))
                throw "Unable to start the thread";
            setStatus(ThreadStatus::ThreadStarted);
        }

        void cancel() override
        {
            if (-1 == pthread_cancel(m_thread))
                throw "Unable to cancel the thread";
            if (-1 == pthread_mutex_destroy(&m_mutex))
                throw "Unable to destroy the thread's mutex";
            if (-1 == pthread_cond_destroy(&m_cond))
                throw "Unable to destroy the thread's condition";
            setStatus(ThreadStatus::ThreadDead);
        }

        void *join() override
        {
            void *return_value = nullptr;

            if (ThreadStatus::ThreadDead == m_status)
                throw "The thread is already dead";
            setStatus(ThreadStatus::ThreadRunning);
            if (-1 == pthread_join(m_thread, &return_value))
                throw "Unable to join the thread";
            if (-1 == pthread_mutex_destroy(&m_mutex))
                throw "Unable to destroy the thread's mutex";
            if (-1 == pthread_cond_destroy(&m_cond))
                throw "Unable to destroy the thread's condition";
            this->detach();
            setStatus(ThreadStatus::ThreadDead);
            return return_value;
        }

        void detach() override
        {
            if (-1 == pthread_detach(m_thread))
                throw "Unable to detach the thread";
        }

        void sleep(long seconds) override
        {
            struct timespec timeout;

            if (-1 == clock_gettime(CLOCK_REALTIME, &timeout))
                throw "Unable to get the CLOCK_REALTIME data";
            timeout.tv_sec += seconds;
            setStatus(ThreadStatus::ThreadSleeping);
            if (-1 == pthread_cond_timedwait(&m_cond, &m_mutex, &timeout))
                throw "Unable to sleep the thread";
            setStatus(ThreadStatus::ThreadRunning);
        }

    private:
        typedef enum EThreadStatus {
            ThreadStarted,
            ThreadRunning,
            ThreadSleeping,
            ThreadDead,
        } ThreadStatus;

        ThreadStatus getStatus()
        {
            ThreadStatus status;

            /*if (-1 == pthread_mutex_lock(&m_mutex))
                throw "Unable to lock the thread's mutex";*/
            status = m_status;
            /*if (-1 == pthread_mutex_unlock(&m_mutex))
                throw "Unable to unlock the thread's mutex";*/
            return status;
        }

        void setStatus(ThreadStatus status)
        {
            /*if (-1 == pthread_mutex_lock(&m_mutex))
                throw "Unable to lock the thread's mutex";*/
            m_status = status;
            /*if (-1 == pthread_mutex_unlock(&m_mutex))
                throw "Unable to unlock the thread's mutex";*/
        }

        pthread_t m_thread;
        pthread_mutex_t m_mutex;
        pthread_cond_t m_cond;
        ThreadStatus m_status;
};

#endif /* !__THREAD_HPP_ */
