#include <iostream>
#include <cstring>
#include <sys/ipc.h> //Inter-Process Communication
#include <sys/shm.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include "comm.h"

int main()
{
    key_t key = GetKey();
    std::cout << "key:" << ToHex(key) << std::endl;
    sleep(3);
    int shmid = CreateShm(key);
    sleep(5);
    std::cout << "shmid: " << shmid << std::endl;

    std::cout << "开始将shm映射到进程的地址空间中" << std::endl;
    char *s = (char *)shmat(shmid, nullptr, 0); // attach

    sleep(5);
    std::cout << "开始将shm从进程的地址空间中移除" << std::endl;
    shmdt(s); // detach

    sleep(5);
    std::cout << "开始将shm从OS中删除" << std::endl;
    shmctl(shmid, IPC_RMID, nullptr);

    sleep(10);
    return 0;
}