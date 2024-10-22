#include<iostream>
#include<cassert>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<cerrno>
#include<cstring>
#include<fcntl.h>
#include"comm.h"
int main()
{
    int wfd=open(FILENAME,O_WRONLY);
    if(wfd<0)
    {
        std::cerr<<"errno:"<<errno<<"errstring"<<strerror(errno)<<std::endl;
        return 1;
    }
    std::cout<<"open fifo sucess......"<<std::endl;

    std::string message;
    while(true)
    {
        std::cout<<"please enter:";
        std::getline(std::cin,message);
        ssize_t s=write(wfd,message.c_str(),message.size());
        if(s<0)
        {
            std::cerr<<"errno:"<<errno<<"strerror:"<<strerror(errno)<<std::endl;
            break;
        }
    }
    close(wfd);
    std::cout<<"close fifo sucess..."<<std::endl;
    return 0;
}
