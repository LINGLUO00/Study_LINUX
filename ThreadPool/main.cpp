#include <iostream>
#include <memory>
#include <ctime>
#include "ThreadPool.h"
#include "Task.h"

// std::autmic<int> cnt;
int main()
{
    // std::unique_ptr<ThreadPool<Task>> tp(new ThreadPool<Task>());
    // tp->Start();

    sleep(10);
    ThreadPool<Task>::GetInstance()->Start();
    srand((uint64_t)time(nullptr) ^ getpid());

    while (true)
    {
        // 可以搞一个网络任务
        int x = rand() % 100 + 1;
        usleep(1234);
        int y = rand() % 200;
        usleep(1234);
        char oper = opers[rand() % opers.size()];

        Task t(x, y, oper);
        // std::cout << "make task: " << t.PrintTask() << std::endl;
        ThreadPool<Task>::GetInstance()->Push(t);

        sleep(1);
    }
    //ThreadPool<Task>::GetInstance()->Wait();
    // TODO
    return 0;
}