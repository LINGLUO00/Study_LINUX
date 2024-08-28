#include"ProgressBar.h"
//version 1
void Progress_1()
{
	char bar[SIZE];
	memset(bar,'\0',sizeof(bar));
	int rate=0;
	char* str = "|/-\\";
	int num = strlen(str);
	while(rate<=MAX_RATE)
	{
		printf("[%-100s][%d%%][%c]\r",bar,rate,str[rate%num]);
		fflush(stdout);
		usleep(STIME);
		bar[rate++]=STYLE;
	}
	printf("\n");
	
}

//version 2
void Progress_2(double rate)
{
	static char bar[SIZE]={0};
	char* str = "|/-\\";
	int num = strlen(str);
	static cnt = 0;
	if(rate>=0 && rate<=MAX_RATE)
	{
		cnt++;
		cnt = (cnt < num ? cnt : 0 );

		printf("[%-100s][%.1f%%][%c]\r",bar,rate,str[cnt]);
		fflush(stdout);
		bar[(int)rate]=STYLE;
	}
}

//V3
	
void Progress_3(double rate)
{
	static char bar[SIZE]={0};
	char* str = "|/-\\";
	int num = strlen(str);
	static cnt = 0;
	if(rate>=0 && rate<=MAX_RATE)
	{
		cnt++;
		cnt = (cnt < num ? cnt : 0 );

		if(rate<MAX_RATE)
		{
			
			bar[(int)rate]=STYLE_BODY;
			bar[(int)rate+1]=STYLE_HEAD;
			
		}
		else
		{
			bar[MAX_RATE]=STYLE_BODY;
		}
		
		printf("[%-100s][%.1f%%][%c]\r",bar,rate,str[cnt]);
		
		fflush(stdout);
	}
}	
