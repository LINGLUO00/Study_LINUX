#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <vector>
#include<functional>
#include<ctime>
#define threadnum 5

typedef std::function<void()> func_t;
// 等同于 using func_t = std::function<void()>
class ThreadData
{
public:
    ThreadData(const std::string &name, const uint64_t &ctime, func_t f)
        : threadname(name), creattime(ctime), func(f)
    {}

public:
    std::string threadname;
    uint16_t creattime;
    func_t func;
};
void Print()
{
    std::cout<<"do work print"<<std::endl;
}
void* ThreadRountine(void* args)
{
    ThreadData* td = static_cast<ThreadData*>(args);
    std::cout<<"new thread named: "<<td->threadname<<"create time: "<<td->creattime<<std::endl;
    td->func;
    //制造一个线程异常
    if(td->threadname=="thread-4")
    {
        int a=0;
        std::cout<<td->threadname<<"触发了异常"<<std::endl;
        //a /= 0;
    }

}
int main()
{
    // creat 5 thread
    std::vector<pthread_t> threads;
    for (size_t i = 0; i < threadnum; i++)
    {
        // creat name
        char threadname[1024];
        snprintf(threadname, sizeof(threadname), "%s-%lu", "thread", i);
        // creat data for each thread
        pthread_t tid;
        ThreadData* td = new ThreadData(threadname,(uint16_t)time(nullptr),Print);
        // creat thread
        pthread_create(&tid,nullptr,ThreadRountine,td);
        threads.push_back(tid);
        sleep(1);
    }
    std::cout<<"thread id :";
    for(const auto& tid:threads)
    {
        std::cout<<tid<<"  ";
    }
    std::cout<<std::endl;
    return 0;
}