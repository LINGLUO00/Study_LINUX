#include<iostream>
#include<cstdio>
#include <string>
#include<vector>
#include<unistd.h>
#include <cassert>
#include<sys/wait.h>
#include<sys/types.h>
#include"Task.hpp"
using namespace std;

static int number = 1;
const int num = 5;
class channel
{
public:
    channel(int fd,pid_t id)//fd:w point id:the fd of the child
    :ctrlfd(fd)
    ,workerid(id)
    {
        name="channel-"+ std::to_string(number++);
    }
    
    int ctrlfd;
    pid_t workerid;
    std::string name;
};

void work()//随机派发任务
{
    while(true)//一个任务做完接着做下一个
    {
        int TaskCode=0;
        ssize_t n=read(0,&TaskCode,sizeof(TaskCode));//0 have been change to pipefd[0]
        if(n==sizeof(TaskCode))
        {
            
            //防止code越界，正常情况不会出现
            if(!tasksys.CheckSafe(TaskCode))
            {
                continue;
            }
            tasksys.RunTask(TaskCode);
        }
        else if(n==0)
        {
            //EOF
            break;
        }
        else
        {
            //wrong
            //do nothing
        }
    }

}
void CreateChannels(std::vector<channel>* c)
{
    std::vector<int> old;
    for(int i=0;i<num;i++)
    {
        int pipefd[2];
        int n = pipe(pipefd);
        assert(n==0);
        (void)n; 

        pid_t id=fork();
        assert(id != -1);
        if(id==0)
        {
            if(!old.empty())
            {
                for(auto fd:old)
                {
                    close(fd);
                }
            }
            close(pipefd[1]);
            dup2(pipefd[0],0);
            work();
            exit(0);
        }  
        close(pipefd[0]);
        c->push_back(channel(pipefd[1],id));
        old.push_back(pipefd[1]);
    }

}

void SendCommand(const std::vector<channel>& c,bool isloop,int num=-1)//指定派发任务
{
    int pos=0;
    while(true)
    {
        int task=tasksys.SelectTask();
        const auto& channel=c[pos++];
        pos %= c.size();
        //debug
        std::cout<<"send task"<<"["<<task<<"]:"<<tasksys.TaskName(task)<<" in "<<channel.name<<" worker is:"<<channel.workerid<<std::endl;
        write(channel.ctrlfd,&task,sizeof(task));//把任务编号写进去，让worker读到
        if(!isloop)
        {
            num--;
            if(num<=0)
            {
                break;
            }

        }
        sleep(1);

    }
    std::cout<<"SendCommand done......"<<std::endl;
}
void ReleaseChannels(const std::vector<channel>& c)
{
    for(const auto& channel:c)
    {
        close(channel.ctrlfd);
        waitpid(channel.workerid,nullptr,0);

    }
}
int main()
{
    //creat channels and process
    std::vector<channel> channels;
    CreateChannels(&channels);

    //send task
    const bool g_always_loop=true;
    SendCommand(channels,!g_always_loop,10);//loop or 10 times
    
    //release(close w point)
    ReleaseChannels(channels);

    return 0;
}