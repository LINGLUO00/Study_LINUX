#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <functional>
#include "Log.h"
#include "mythread.h"
#include "LockGuard.h"
static const int defaultnum = 5;
class ThreadData
{
public:
    ThreadData(const std::string &name)
        : threadname(name)
    {
    }
    ~ThreadData() {}

public:
    std::string threadname;
};

template <class T>
class ThreadPool
{
    ThreadPool(int thread_num = defaultnum)
        : _thread_num(thread_num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        for (int i = 0; i < _thread_num; i++)
        {
            std::string threadname = "thread-";
            threadname += std::to_string(i + 1);
            ThreadData td(threadname);
            // _threads.emplace_back(threadname, std::bind(&ThreadPool<T>::ThreadRun, this), td);
            Thread<ThreadData> t(threadname,
                                 std::bind(&ThreadPool<T>::ThreadRun, this, std::placeholders::_1), td);
            _threads.push_back(t);
            lg.LogMessage(Info, "%s is created...\n", threadname.c_str());
        }
    }
    ThreadPool(const ThreadPool<T> &tp) = delete;
    const ThreadPool<T> &operator=(const ThreadPool<T>) = delete;
public:
    static ThreadPool<T> *GetInstance()
    {
        if (instance == nullptr)
        {
            LockGuard lock(&sig_lock);
            if (instance == nullptr)
            {
                lg.LogMessage(Debug, "创建单例成功...\n");
                instance = new ThreadPool<T>();
            }
        }
        return instance;
    }
    bool Start()
    {
        for (auto &thread : _threads)
        {
            thread.Start();
            lg.LogMessage(Info, "%s is running...\n", thread.ThreadName().c_str());
        }
        return true;
    }
    void ThreadWait(const ThreadData &td)
    {
        lg.LogMessage(Debug, "no task, %s is sleeping...\n", td.threadname.c_str());
        pthread_cond_wait(&_cond, &_mutex);
    }
    void ThreadWakeup()
    {
        pthread_cond_signal(&_cond);
    }
    void ThreadRun(ThreadData &td)
    {
        while (true)
        {
            T t;
            {
                LockGuard lockguard(&_mutex);
                while (_q.empty())
                {
                    ThreadWait(td);
                    lg.LogMessage(Debug, "thread %s is wakeup\n", td.threadname.c_str());
                }
                t = _q.front();
                _q.pop();
            }
            t();
            lg.LogMessage(Debug, "%s handler task %s done, result is : %s\n",
                           td.threadname.c_str(), t.PrintTask().c_str(), t.PrintResult().c_str());
        }
    }
    void Push(T &in)
    {
        lg.LogMessage(Debug, "other thread push a task, task is:%s\n", in.PrintTask().c_str());
        LockGuard lock(&_mutex);
        _q.push(in);
        ThreadWakeup();
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

private:
    std::queue<T> _q;
    std::vector<Thread<ThreadData>> _threads;
    int _thread_num;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;

    static ThreadPool<T> *instance;
    static pthread_mutex_t sig_lock;
    // 扩展1：
    // int _thread_num;
    // int _task_num;

    // int _thread_num_low_water;  // 3
    // int _thread_num_high_water; // 10
    // int _task_num_low_water;    // 0
    // int _task_num_high_water;   // 30

    // 扩展2: 多进程+多线程

    // int number{1};
};
template <class T>
ThreadPool<T> *ThreadPool<T>::instance = nullptr;
template <class T>
pthread_mutex_t ThreadPool<T>::sig_lock = PTHREAD_MUTEX_INITIALIZER;