#include"ProgressBar.h"
//模拟download
void download(callBack cb)
{
	int target = TARGET_SIZE;
	int total = 0;
	int testwait=100;
	while(total<=target)
	{
		usleep(STIME);//模拟下载时间
		total += DSIZE;
		
		double rate = total*100.0/target;
		if(rate>50.0 && testwait)//模拟卡住
		{
			total=target/2;
			testwait--;
		}
		cb(rate);	
	}
	cb(MAX_RATE);
	printf("\n");
}
int main()
{
	download(Progress_3);
	return 0;
}
