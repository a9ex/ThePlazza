/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Thread.hpp
*/

#ifndef __THREAD_HPP_
    #define __THREAD_HPP_
    #include <pthread.h>

typedef void *(*ThreadFunction)(void *);

class IThread {
    public:
        virtual ~IThread() = default;
        virtual void start(ThreadFunction function, void *arg) = 0;
        virtual void cancel() = 0;
        virtual void *join() = 0;
        virtual void detach() = 0;
};

class Thread : public IThread {
    public:
        typedef enum EThreadStatus {
            ThreadStarted,
            ThreadRunning,
            ThreadDead
        } ThreadStatus;
        Thread();
        ~Thread();
        void start(ThreadFunction function, void *arg) override;
        void cancel() override;
        void *join() override;
        void detach() override;
    private:
        pthread_t m_thread;
        ThreadStatus m_status;
};

#endif /* !__THREAD_HPP_ */
