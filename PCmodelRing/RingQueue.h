#pragma once
#include "LockGuard.h"
#include <pthread.h>
#include <ctime>
#include <queue>
#include <sys/types.h>
#include <semaphore.h>
#include <unistd.h>
const int defaultsize = 5;
template <class T>
class RingQueue
{
public:
    RingQueue(int size = defaultsize) : _size(size), _ringqueue(size), _p_step(0), _c_step(0)
    {
        sem_init(&_space_sem, 0, size);
        sem_init(&_data_sem, 0, 0);
        pthread_mutex_init(&_p_mutex, nullptr);
        pthread_mutex_init(&_c_mutex, nullptr);
    }
    void Push(const T &in)
    {
        P(_space_sem);
        {
            LockGuard lockguard(&_p_mutex);
            _ringqueue[_p_step] = in;
            _p_step++;
            _p_step %= _size;
        }
        V(_data_sem);
    }
    void Pop(T *out)
    {
        P(_data_sem);
        {
            LockGuard lockguard(&_c_mutex);
            *out = _ringqueue[_c_step];
            _c_step++;
            _c_step %= _size;
        }
        V(_space_sem);
    }
    ~RingQueue()
    {
        sem_destroy(&_space_sem);
        sem_destroy(&_data_sem);

        pthread_mutex_destroy(&_p_mutex);
        pthread_mutex_destroy(&_c_mutex);
    }

private:
    std::vector<T> _ringqueue;
    int _size;
    void P(sem_t &sem)
    {
        sem_wait(&sem);
    }
    void V(sem_t &sem)
    {
        sem_post(&sem);
    }
    int _p_step; // 生产者的生产位置
    int _c_step;

    sem_t _space_sem;
    sem_t _data_sem;
    pthread_mutex_t _p_mutex;
    pthread_mutex_t _c_mutex;
};