#pragma once
#include <pthread.h>
#include <iostream>

class Mutex
{
public:
    Mutex(pthread_mutex_t *lock)
        : _lock(lock)
    {}
    void Lock()
    {
        pthread_mutex_lock(_lock);
    }
    void unLock()
    {
        pthread_mutex_unlock(_lock);
    }
    ~Mutex()
    {}

private:
    pthread_mutex_t *_lock;
};

class LockGuard
{
public:
    LockGuard(pthread_mutex_t* mutex)
    :_mutex(mutex)
    {
        _mutex.Lock();
    }
    ~LockGuard()
    {
        _mutex.unLock();
    }

private:
    Mutex _mutex;
};