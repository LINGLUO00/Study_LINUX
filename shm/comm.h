#pragma once

#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <cstdio>
#include <string>
#include <sys/ipc.h> //Inter-Process Communication
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
const int size = 4096;
const std::string pathname = "/home/hyl/study/shm/comm.h";
const std::string filename = "fifo";
const int proj_id = 0x2234;
key_t GetKey()
{
    key_t key = ftok(pathname.c_str(), proj_id);
    if (key < 0)
    {
        std::cerr << "errno:" << errno << "errstring:" << strerror << std::endl;
        exit(1);
    }
    return key;
}
int CreatShmHelper(key_t key, int flag)
{
    int shmid = shmget(key, size, flag);
    if (shmid < 0)
    {
        std::cerr << "errno:" << errno << "errstring:" << strerror << std::endl;
        exit(2);
    }
    return shmid;
}
int GetShm(key_t key)
{
    return CreatShmHelper(key, IPC_CREAT);
}

std::string ToHex(int key) // To Hexadecimal转为十六进制
{
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "0x%x", key);
    return buffer;
}

int CreateShm(key_t key)
{
    return CreatShmHelper(key, IPC_CREAT | IPC_EXCL | 0644);
}

bool Makefifo()
{
    int n = mkfifo(filename.c_str(), 0666);
    if (n < 0)
    {
        std::cerr << "errno:" << errno << "errstring:" << strerror << std::endl;

        return false;
    }
    std::cout << "mkfifo sucess......" << std::endl;
    return true;
}