#include "mystdio.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> //have O_RDONLY....
#include <errno.h>
#include <stdlib.h>

#define DFL_MODE 0666
myFILE *my_fopen(const char *path, const char *mode)
{
    int fd = 0;
    int flag = 0;
    if (strcmp(mode, "r") == 0)
    {
        flag |= O_RDONLY;
    }
    else if (strcmp(mode, "w") == 0)
    {
        flag |= (O_CREAT | O_TRUNC | O_WRONLY);
    }
    else if(strcmp(mode,"a")==0)
    {
        flag |= (O_WRONLY | O_CREAT | O_APPEND);
    }
    else{
        //DO NOTHING
    }
    if(flag & O_CREAT)
    {
        fd = open(path,flag,DFL_MODE);
    }
    else
    {
        fd = open(path,flag);
    }
    if(fd<0)
    {
        errno=2;
        return NULL;
    }
    myFILE* fp=(myFILE*)malloc(sizeof(myFILE));
    if(!fp)
    {
        errno=3;
        return NULL;
    }
    fp->flag=FLUSH_LINE;
    fp->fileno=fd;
    fp->end=0;
    return fp;
}

int my_fwrite(const char *s, int num, myFILE *stream)
{
    memcpy(stream->buffer+stream->end,s,num);
    stream->end += num;
    if((stream->flag & FLUSH_LINE)&& stream->end>0 && stream->buffer[stream->end-1]=='\n')
    {
        my_fflush(stream);
    }
    return num;
}

int my_fflush(myFILE* stream)
{
    if(stream->end>0)
    {
        write(stream->fileno,stream->buffer,stream->end);//flush the message of buffer to the file
        stream->end=0;
    }
    return 0;
}


int my_fclose(myFILE* stream)
{
    my_fflush(stream);
    return close(stream->fileno);
}