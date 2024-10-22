
#include "BlockQueue.h"
#include "Task.h"
#include"LockGuard.h"

#include <pthread.h>
#include <ctime>
#include <sys/types.h>
#include<iostream>
#include<queue>
#include <unistd.h>
class ThreadData
{
public:
    BlockQueue<Task> *bq;
    std::string name;
};
void* consumer(void* args)
{
    ThreadData* td=(ThreadData*)args;
    while(true)
    {
        Task t;
        td->bq->Pop(&t);
        t();
        std::cout<<"consumer data: " << t.PrintResult() << ", " << td->name << std::endl;
    }
    return nullptr;

}
void* productor(void* args)
{
    BlockQueue<Task>* bq=static_cast<BlockQueue<Task>*>(args);
    while(true)
    {
        int data1=rand()%10;
        usleep(rand()%123);
        int data2=rand()%10;
        usleep(rand()%123);
        char oper=opers[rand()%(opers.size())];
        Task t(data1,data2,oper);
        std::cout<<"productor task: "<<t.PrintTask()<<std::endl;
        bq->Push(t);
        sleep(1);
    }
    return nullptr;
}
int main()
{
    srand((uint64_t)time(nullptr)^getpid()^pthread_self());
    BlockQueue<Task> *bq = new BlockQueue<Task>();
    pthread_t c[3], p[2]; // 消费者和生产者
    ThreadData *td = new ThreadData();
    td->bq = bq;
    td->name = "thread-1";
    pthread_create(&c[0], nullptr, consumer, td);


    ThreadData *td1 = new ThreadData();
    td1->bq = bq;
    td1->name = "thread-2";
    pthread_create(&c[0], nullptr, consumer, td1);

    ThreadData *td2 = new ThreadData();
    td2->bq = bq;
    td2->name = "thread-3";
    pthread_create(&c[0], nullptr, consumer, td2);


    pthread_create(&p[0], nullptr, productor, bq);
    pthread_create(&p[1], nullptr, productor, bq);

    pthread_join(c[0], nullptr);
    pthread_join(c[1], nullptr);
    pthread_join(c[2], nullptr);
    pthread_join(p[0], nullptr);
    pthread_join(p[1], nullptr);

    return 0;
}