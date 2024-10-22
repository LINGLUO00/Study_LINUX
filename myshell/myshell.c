#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include<sys/stat.h>
#include<fcntl.h>


#define NUM 1024
#define SIZE 64
#define SEP " "
//#define Debug 1
//redir
#define NoneRedir 0
#define OutputRedir 1
#define AppendRedir 2
#define InputRedir 3
int redir = NoneRedir;
char* filename=NULL;

char cwd[1024];

char enval[1024];//only test, in fact it need an array of two-dimensional character pointers
int lastcode=0;
char* homepath()
{
    char*home=getenv("HOME");
    if(home) return home;
    else return (char*)".";
}
const char* getUsername()
{
    const char* name = getenv("USER");
    if(name) return name;
    else return "none";
}


const char* getHostname()
{
    const char* hostname = getenv("HOSTNAME");
    if(hostname) return hostname;
    else return "none";
}


const char* getCwd()
{
    const char* cwd = getenv("PWD");
    if(cwd) return cwd;
    else return "none";
}

int getUserCommand(char* command,int num)
{
    
    printf("[%s@%s %s]# ",getUsername(),getHostname(),getCwd());
    char* r = fgets(command,num,stdin);
    if(r==NULL) return -1;
    command[strlen(command)-1] = '\0';//?
    return strlen(command);
    
}
void CommandSplit(char* in,char* out[])
{
 
    int argc = 0;
    //2.分割字符串strtok
    out[argc++]=strtok(in,SEP);
    while(out[argc++]=strtok(NULL,SEP));
#ifdef Debug
    for(int i = 0; out[i];i++)
    {
     printf("%d:%s\n",i,out[i]);
    }
#endif    
}
int execut(char* argv[])
{
  
    //执行对应命令
    pid_t id = fork();
    if(id<0) return -1;
    else if(id==0)
    {
        int fd =0;
        if(redir == InputRedir)
        {
            fd = open(filename, O_RDONLY);
            dup2(fd , 0);
        }
        else if(redir== OutputRedir)
        {
            fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(fd,1);
        }
        else if(redir== AppendRedir)
        {
            fd = open(filename,O_WRONLY | O_CREAT | O_APPEND, 0666);
            dup2(fd,1);
        }
        //exec command
        execvp(argv[0],argv);//程序替换
        exit(1);
    }
    else
    {
       int status=0;
       pid_t rid = waitpid(id,&status,0);
       if(rid>0){
          lastcode=WEXITSTATUS(status);    
       };
    }

    return 0;
}
void cd(const char* path)
{
    chdir(path);//直接变换工作目录到path的一个库函数
    char tmp[1024];
    getcwd(tmp,sizeof(tmp));//获取当前的绝对路径
    sprintf(cwd,"PWD=%s",tmp);//将tmp里的绝对路径格式化为pwd=xxx的格式输出到cwd
    putenv(cwd);//将cwd导出为环境变量，这样可以查看pwd
}
//void cd(const char* path)
//{
//    sprintf(cwd,"PWD=%s",path);
//    putenv(cwd);
//    chdir(path);
//}
int doBuildin(char* argv[])
{
    //这里以cd命令为例
    if(strcmp(argv[0],"cd")==0)
    {
        char* path=NULL;
        if(argv[1]==NULL) path=homepath();
        else path=argv[1];
        cd(path);
        return 1;   
    }
    else if(strcmp(argv[0],"export")==0)
    {
        if(argv[1]==NULL) return 1;
        strcpy(enval,argv[1]);
        putenv(enval);
        return 1;
    }
    else if(strcmp(argv[0],"echo")==0)
    {
       if(argv[1]==NULL){
          printf("\n");
          return 1;
       }
       if((*(argv[1])) == '$' && strlen(argv[1])>1){
       char* val =argv[1]+1;//$PATH  &?
       if(strcmp(val,"?")==0)
       {
           printf("%d\n",lastcode);
           lastcode=0;
       }
       else
       {
         const char* enval = getenv(val);
         if(enval){
         printf("%s\n",enval);
         }
         else printf("\n");
       }
         return 1;
       }
       else{
         printf("%s\n",argv[1]);
         return 1;
       }
    }
    return 0;   
}
#define SkipSpace(filename) do{while(isspace(*filename)) filename++;}while(0)
void checkRedir(char usercommand[], int len)
{
     char* end = usercommand +len -1;
     char* start = usercommand;
     while(end>start)
     {
        if(*end=='>')
        {
            if(*(end-1) == '>')
            {
                *(end-1) = '\0';
                filename = end+1;
                SkipSpace(filename);
                redir=AppendRedir;
                break;
            }
            else
            {
                *end='\0';
                filename =end+1;
                SkipSpace(filename);
                redir=OutputRedir;
                break;
            }
        }
        else if(*end =='<')
        {
            *end = '\0';
            filename=end+1;
            SkipSpace(filename);
            redir=InputRedir;
            break;
        }
        else
        {
            end--;
        }

     }

}
int main()
{
    while(1)
    {

         redir = NoneRedir;
         filename=NULL;
         char usercommand[NUM];
         char*argv[SIZE];
         int n = getUserCommand(usercommand,sizeof(usercommand));
         if(n<=0) continue;
         //判断是否有重定向
         checkRedir(usercommand,sizeof(usercommand));
         //分割字符串
         CommandSplit(usercommand,argv);
         //检查对应的命令是不是内建命令
         n = doBuildin(argv);
         if(n) continue;
         //执行对应命令
         execut(argv);
    }
}

