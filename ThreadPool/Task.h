#pragma once
#include <pthread.h>


#include <ctime>
#include <sys/types.h>
#include<iostream>
#include <unistd.h>
const int defaultvalue = 0;
enum
{
    ok = 0,
    div_zero,
    mod_zero,
    unknow
};
const std::string opers = "+-*/%)(&";
class Task
{
public:
    Task() {}
    Task(int x, int y, char oper)
        : _x(x), _y(y), _oper(oper)
    {
    }
    void Run()
    {
        switch (_oper)
        {
        case '+':
            _result = _x + _y;
            break;
        case '-':
            _result = _x - _y;
            break;
        case '*':
            _result = _x * _y;
            break;
        case '/':
        {
            if (_y == 0)
                _code = div_zero;
            else
                _result = _x / _y;
        }
        break;
        case '%':
        {
            if (_y == 0)
                _code = mod_zero;
            else
                _result = _x % _y;
        }

        break;
        default:
            _code = unknow;
            break;
        }
    }
    void operator()()
    {
        Run();
        sleep(2);
    }
    std::string PrintTask()
    {
        std::string s;
        s = std::to_string(_x);
        s += _oper;
        s += std::to_string(_y);
        s += "=?";

        return s;
    }
        std::string PrintResult()
    {
        std::string s;
        s = std::to_string(_x);
        s += _oper;
        s += std::to_string(_y);
        s += "=";
        s += std::to_string(_result);
        s += " [";
        s += std::to_string(_code);
        s += "]";

        return s;
    }
    ~Task()
    {
    }
private:
    int _x;
    int _y;
    char _oper;
    int _result;
    int _code;
};