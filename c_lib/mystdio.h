#pragma once

// define the size of buffer
#define SIZE 4096
// the flag of three flush strategy
#define FLUSH_NONE 1
#define FLUSH_LINE (1 << 1)
#define FLUSH_ALL (1 << 2)

typedef struct _myFILE
{
    int fileno; // which file
    int flag;   // which flush strategy
    char buffer[SIZE];
    int end; // record current file end, because it maybe have message in it before
} myFILE;

// open write flush close
extern myFILE *my_fopen(const char *path, const char *mode); // mode: r,w
extern int my_fwrite(const char *s, int num, myFILE *stream);
extern int my_flush(myFILE* stream);
extern int my_fclose(myFILE* stream);
