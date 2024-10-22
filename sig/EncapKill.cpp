#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>
int signo=0;
// static void Usage(const std::string &proc)
// {
    
// }
// ./EncapKill -9 pid
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        exit(0);
    }
    int signumber=std::stoi(argv[1]+1);
    int processpid=std::stoi(argv[2]);
    kill(processpid,signumber);

}