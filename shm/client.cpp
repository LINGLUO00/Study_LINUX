#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <sys/ipc.h> //Inter-Process Communication
#include <sys/shm.h>
#include "comm.h"
int main()
{
    key_t key = GetKey();
    int shmid = GetShm(key);
    return 0;
}