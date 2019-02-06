#ifndef THREAD_GUARD_H
#define THREAD_GUARD_H

#include <thread>

class Thread_Guard
{
public:
    explicit Thread_Guard(std::thread& t):mThread(t){}

    ~Thread_Guard()
    {
        if(mThread.joinable())
            mThread.join();
    }

    Thread_Guard(const Thread_Guard&)=delete;
    Thread_Guard& operator=(const Thread_Guard&)=delete;

private:
    std::thread& mThread;
};

#endif // THREAD_GUARD_H
