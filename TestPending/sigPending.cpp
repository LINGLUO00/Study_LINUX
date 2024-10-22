#include <iostream>
#include <unistd.h>
#include <signal.h>
void PrintPending(const sigset_t& pending)
{
    for (int signo = 31; signo > 0; signo--)
    {
        if (sigismember(&pending, signo))
        {
            std::cout << "1";
        }
        else
        {
            std::cout << "0";
        }
    }
    std::cout<<"\n";
}
void handler(int signo)
{
    sigset_t pending;
    std::cout<<"########################"<<std::endl;
    sigpending(&pending);
    PrintPending(pending);
    std::cout<<"########################"<<std::endl;
    std::cout<<"handler"<<signo<<std::endl;

}
int main()
{
    std::cout<<"pid is "<<getpid()<<std::endl;
    sleep(3);
    signal(2,handler);
    // masking sig 2
    sigset_t set, oset; // each sigset_t have a bit for each sig
    sigemptyset(&set);  // clear all sig
    sigemptyset(&oset);
    sigaddset(&set, 2);
    sigprocmask(SIG_BLOCK, &set, &oset); // oset is a backup
    // let the process get the process pending all the time
    int cnt = 0;
    sigset_t pending;
    while (true)
    {
        sigpending(&pending);
        PrintPending(pending);
        sleep(3);
        std::cout<<"cnt="<<cnt<<std::endl;
        cnt++;
        if (cnt == 5)
        {
            std::cout << "解除对2号信号的屏蔽，2号信号准备递达" << std::endl;
            sigprocmask(SIG_SETMASK, &oset, nullptr); // set the set of blocked sig to the set called oset
            PrintPending(pending);
            break;
        }
        
    }

}
