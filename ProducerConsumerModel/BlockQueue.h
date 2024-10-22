#pragma once
#include"LockGuard.h"
#include <pthread.h>
#include <ctime>
#include<queue>
#include <sys/types.h>
#include <unistd.h>
const int defaultcapacity = 5;
template<class T>
class BlockQueue
{
public:
    BlockQueue(int cap = defaultcapacity):_capacity(cap)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_p_cond, nullptr);
        pthread_cond_init(&_c_cond, nullptr);
    }
    bool IsFull()
    {
        return _q.size() == _capacity;
    }
    bool IsEmpty()
    {
        return _q.size() == 0;
    }
    void Push(const T& in)
    {
        LockGuard lockguard(&_mutex);
        while(IsFull())
        {
            pthread_cond_wait(&_p_cond,&_mutex);
        }
        _q.push(in);
        pthread_cond_signal(&_c_cond);
    }
    void Pop(T* out)
    {
        LockGuard lockguard(&_mutex);
        while(IsEmpty())
        {
            pthread_cond_wait(&_c_cond,&_mutex);
        }
        *out=_q.front();
        _q.pop();
        pthread_cond_signal(&_p_cond);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_p_cond);
        pthread_cond_destroy(&_c_cond);
    }
private:
    std::queue<T> _q;
    int _capacity;
    pthread_mutex_t _mutex;
    pthread_cond_t _p_cond; // 给生产者的
    pthread_cond_t _c_cond; // 给消费者的
};