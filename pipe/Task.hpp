//target: using nameless pipe to achive a ProceesPool(one father n child)
#pragma once
#include<iostream>
#include<cstdio>
#include<vector>
#include<ctime>
#include<unistd.h>
#include<functional>


typedef std::function<void()> task_t;
//diferent work
void Download()
{
    std::cout<<"我是一个下载任务"<<getpid()<<std::endl;
}

void PrintLog()
{
    std::cout<<"我是一个打印日志任务"<<getpid()<<std::endl;

}

void PushVideo()
{
    std::cout<<"我是一个推送视频的任务"<<getpid()<<std::endl;

}


class TaskSys
{
public:
    std::vector<task_t> tasks;
    TaskSys()
    {
        tasks.push_back(Download);
        tasks.push_back(PrintLog);
        tasks.push_back(PushVideo);
        srand(time(nullptr) ^ getpid());
    }
    bool CheckSafe(int TaskCode)
    {
        if(TaskCode>=0 &&TaskCode<tasks.size())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void RunTask(int flag)
    {
        return tasks[flag]();
    }
    int SelectTask()
    {
        return rand()%tasks.size();
    }
    std::string TaskName(int flag)//方便看到是哪一个任务
    {
        switch(flag)
        {
            case 0:
                return "DownLoad";
            case 1:
                return "PrintLog";
            case 2:
                return "PushVideo";
            default:
                return "Unknow";
        }
    }

};

TaskSys tasksys;

