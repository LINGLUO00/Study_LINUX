#pragma once
#include <iostream>
#include <cstdarg>
#include <ctime>
#include <fstream>

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
enum Level
{
    Debug = 0,
    Error,
    Waring,
    Info
};
enum
{
    Screen = 10,
    OneFile,
    ClassFile
};

const int defaultstyle = Screen;
const int defaultlevel = Info;
const std::string default_filename = "log."; // 默认文件名字

const std::string logdir = "log"; // 默认文件夹

class Log
{
public:
    Log()
        : style(defaultstyle), filename(default_filename)
    {
        mkdir(logdir.c_str(), 0775);
    }
    void Style(int sty)
    {
        style = sty;
    }
    std::string LevelToStr(const int level)
    {
        switch (level)
        {
        case Debug:
            return "Debug";
            break;
        case Error:
            return "Error";
            break;
        case Waring:
            return "Waring";
            break;
        case Info:
            return "Info";
            break;
        default:
            return "UnKnow";
            break;
        }
    }
    std::string TimestampToDate()
    {
        time_t currtime = time(nullptr);
        struct tm *curr = localtime(&currtime);
        char time_buffer[128];
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", curr);
        return time_buffer;
    }

    void WriteLogToOneFile(const std::string &logname, const std::string &message)
    {
        umask(0);
        int fd = open(logname.c_str(), O_CREAT | O_WRONLY | O_APPEND,0666);
        if (fd < 0)
        {
            return;
        }
        write(fd, message.c_str(), message.size());
        close(fd);
    }
    void WriteLogToClassFile(const std::string &levelstr, const std::string &message)
    {
        std::string logname = logdir;
        logname += "/";
        logname += filename;
        logname += levelstr;
        WriteLogToOneFile(logname, message);
    }
    void WriteLog(const std::string &levelstr, const std::string &message)
    {
        switch (style)
        {
        case Screen:
            std::cout << message;
            break;
        case OneFile:
            WriteLogToOneFile("all", message);
            break;
        case ClassFile:
            WriteLogToClassFile(levelstr, message);
            break;
        default:
            break;
        }
    }
    void LogMessage(int level, const char *format, ...)
    {
        char naturebuffer[1024];
        std::string levelstr = LevelToStr(level);
        std::string currtime = TimestampToDate();

        std::string idstr = std::to_string(getpid());
        snprintf(naturebuffer, sizeof(naturebuffer), "[%s][%s][%s]", LevelToStr(level).c_str(),
               currtime.c_str(), idstr.c_str());

        char msgbuffer[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(msgbuffer, sizeof(msgbuffer), format, args);
        va_end(args);
        std::string loginfo = naturebuffer;
        loginfo += msgbuffer;
        WriteLog(levelstr, loginfo);
    }
    ~Log() {}

private:
    int style;
    std::string filename;
};
Log lg;
