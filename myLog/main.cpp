#include<iostream>
#include "Log.h"
int main()
{
    Log log;
    log.Style(12);
    log.LogMessage(Debug, "this is a log message: %d, %lf\n", 123, 3.14);
    std::cout<<std::endl;
    log.LogMessage(Info, "this is a log message: %d, %lf\n", 123, 3.14);
    std::cout<<std::endl;

    log.LogMessage(Waring, "this is a log message: %d, %lf\n", 123, 3.14);
    std::cout<<std::endl;



    return 0;
}