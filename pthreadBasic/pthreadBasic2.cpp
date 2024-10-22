#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
// something about thread 分离，取消。和让不同thread solve the task

////cancel

// std::string ToHex(pthread_t tid)
// {
//     char id[64];
//     snprintf(id, sizeof(id), "0x%lx", tid);
//     return id;
// }
// void *threadRoutine(void *arg)
// {
//     std::string name = static_cast<char *>(arg);
//     while (true)
//     {

//         std::cout << "new thread is running, thread name: " << name << " thread id: " << ToHex(pthread_self()) << std::endl;
//     }

// }
// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, threadRoutine, (void *)"thread-1");
//     sleep(1);
//     int n = pthread_cancel(tid);
//     std::cout << "main thread cancel done"  << "n:" << n << std::endl;
//     return 0;
// }

// return,也可以用一个ThreatReturn类，来返回
std::string ToHex(pthread_t tid)
{
    char id[64];
    snprintf(id, sizeof(id), "0x%lx", tid);
    return id;
}
void *threadRoutine(void *arg)
{
    std::string name = static_cast<char *>(arg);
    int cnt = 5;
    while (cnt--)
    {

        std::cout << "new thread is running, thread name: " << name << " thread id: " << ToHex(pthread_self()) << std::endl;
    }

    return (void *)"thread-1 exit";
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine, (void *)"thread-1");
    sleep(1);
    int tmp = 0;
    void *ret = nullptr;
    tmp = pthread_join(tid, &ret);
    std::cout << "thread return: " << (char*)ret << std::endl;
    return 0;
}


