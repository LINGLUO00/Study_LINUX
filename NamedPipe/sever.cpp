#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "comm.h"
bool Makefifo() // creat a named pipe
{
    int n = mkfifo(FILENAME, 0666);
    if (n < 0)
    {
        std::cerr << "errno" << errno << "errstring" << strerror(errno) << std::endl;
        return false;
    }
    std::cout << "mkfifo sucess......" << std::endl;
    return true;
}

int main()
{
Start:
    int rfd = open(FILENAME, O_RDONLY);
    if (rfd < 0)
    {
        std::cerr << "errno" << errno << "errsting" << strerror(errno) << std::endl;
        if (Makefifo())
            goto Start;
        else
            return 1;
    }

    std::cout << "open fifo sucess..." << std::endl;

    char buffer[1024];
    while (true)
    {
        ssize_t s = read(rfd, buffer, sizeof(buffer) - 1);
        if(s>0)
        {
            buffer[s]=0;
            std::cout<<"Client say:"<<buffer<<std::endl;
        }
        else if(s==0)
        {
            std::cout<<"client quit, server quit too!"<<std::endl;
            break;
        }

    }
    close(rfd);
    std::cout << "close fifo success..." << std::endl;
    return 0;
}