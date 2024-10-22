#include<iostream>
#include<unistd.h>
#include<signal.h>
void handler(int signo)
{
    std::cout<<"获得2号信号"<<std::endl;
    exit(1);
}
int main()
{
    signal(2,handler);
    while(true)
    {
        std::cout<<"running...,pid: "<<getpid()<<std::endl;
        sleep(1);
    }
}