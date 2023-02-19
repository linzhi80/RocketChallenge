// CS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include "Application.h"
using namespace std;

int main()
{
    Application app;

    if (app.initial() != 0) {
        exit(-1);
    }
        
    if (app.ready() == 0) {
        exit(0);
    }

    app.run();

    return 0;
}
