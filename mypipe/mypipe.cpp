#include <iostream>
#include<cstdio>
#include <cassert>
#include <cstring>
#include<cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 4096
using namespace std;

int main()
{
    cout<<"hello"<<endl;
    // first creat a pipe
    int pipefd[2] = {0};
    int n = pipe(pipefd);
    assert(n == 0);
    (void)n;
    cout << "pipefd[0]:" << pipefd[0] << ", pipefd[1]:" << pipefd[1] << endl;
    // second fork
    pid_t id = fork();
    if (id < 0)
    {
        perror("fork fail");
        return 1;
    }
    // father r, child w
    if (id == 0)
    {
        int cnt=1000;
        close(pipefd[0]);
        while (true)
        {
            char message[MAX];
            snprintf(message, sizeof(message), "hello father, I am child, pid: %d,cnt:%d", getpid(),cnt);
            write(pipefd[1], message, strlen(message));
            cnt--;
            cout<<"child sleep"<<endl;
            sleep(1);
            //if(cnt>3) break;
        }
        cout<<"child close w point"<<endl;
        exit(0);
    }

    close(pipefd[1]);
    char buffer[MAX];
    while(true)
    {
        cout<<"father sleep"<<endl;
        sleep(10);
        
        int n = read(pipefd[0],buffer,sizeof(buffer));
        if(n==0)
        {
            cout<<"child quit,me too"<<endl;
        }
        else
        {
            cout<<"child say:"<<buffer<<"to me"<<endl;
        }
        cout<<"father return val n:"<<n<<endl;
        //sleep(1);
        //break;
    }
    close(pipefd[0]);
    sleep(10);

    int status=0;
    pid_t rid = (id,&status,0);
    if(rid == id)
    {
        cout<<"wait success"<<endl;
    }
    return 0;

}