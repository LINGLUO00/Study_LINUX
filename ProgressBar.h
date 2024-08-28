#pragma once
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define STYLE '#'
#define SIZE 101
#define MAX_RATE 100
#define STIME 1000*40
#define TARGET_SIZE 1024*1024

#define DSIZE 1024*10

#define STYLE_BODY '='
#define STYLE_HEAD '>'

typedef void (*callBack)(double);
void Progress_1();

void Progress_2(double);

void Progress_3(double);
