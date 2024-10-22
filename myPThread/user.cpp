#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <ctime>
#include "mythread.h"
#include "LockGuard.h"
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int tickit = 1000;
std::string GetThreadName()
{
    static int num = 0;
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "thread-%d", num);
    num++;
    return buffer;
}
class ThreadData
{
public:
    ThreadData(const std::string &name, pthread_mutex_t *lock)
        : _name(name), _lock(lock)
    {
    }

public:
    std::string _name;
    pthread_mutex_t *_lock;
};
void GetTickit(ThreadData *thread)
{
    while (true)
    {
        //LockGuard lockguard(thread->_lock);
        pthread_mutex_lock(thread->_lock);
        if (tickit > 0)
        {
            usleep(1000);
            std::cout << thread->_name << " get a tickit:" << tickit << std::endl;
            tickit--;
            // pthread_mutex_unlock(mutex);
        }
        else
        {
            std::cout << "没有票了," << thread->_name << "进入等待" << std::endl;
            pthread_cond_wait(&cond, thread->_lock);
            // pthread_mutex_unlock(mutex);
        }
        pthread_mutex_unlock(thread->_lock);
    }
}

int main()
{
    pthread_mutex_init(&mutex, nullptr);

    std::string name1 = GetThreadName();
    ThreadData *td1 = new ThreadData(name1, &mutex);
    Thread<ThreadData *> t1(name1, GetTickit, td1);

    std::string name2 = GetThreadName();
    ThreadData *td2 = new ThreadData(name2, &mutex);
    Thread<ThreadData *> t2(name2, GetTickit, td2);

    std::string name3 = GetThreadName();
    ThreadData *td3 = new ThreadData(name3, &mutex);
    Thread<ThreadData *> t3(name3, GetTickit, td3);

    std::string name4 = GetThreadName();
    ThreadData *td4 = new ThreadData(name4, &mutex);
    Thread<ThreadData *> t4(name4, GetTickit, td4);

    std::string name5 = GetThreadName();
    ThreadData *td5 = new ThreadData(name5, &mutex);
    Thread<ThreadData *> t5(name5, GetTickit, td5);

    t1.Start();
    t2.Start();
    t3.Start();
    t4.Start();
    t5.Start();

    sleep(2);
    while (true)
    {
        sleep(6);
        pthread_mutex_lock(&mutex);
        std::cout << "重新投递票" << std::endl;
        tickit += 1000;
        pthread_mutex_unlock(&mutex);
        sleep(6);
        pthread_cond_signal(&cond);
    }

    t1.Join();
    t2.Join();
    t3.Join();
    t4.Join();
    t5.Join();

    pthread_mutex_destroy(&mutex);
    return 0;
}
