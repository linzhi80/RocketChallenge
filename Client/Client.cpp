// Client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <winsock2.h>
#include <string.h>
#include <atlbase.h>
#include <thread>
#include "../CSDlll/TCPSocket.h"
#include "Utility.h"

#pragma comment(lib, "..\\CS\\x64\\Debug\\CSDlll.lib")
using namespace std;

TCPSocket g_tcpSocket;

constexpr const char* inputAcc = "Input your Account:";
constexpr const char* inputPW = "Input your Password:";
constexpr const char* successStr = "Connect successfully!";

void* recThreadFunc(void* arg)
{
    if (arg == nullptr)
        return nullptr;
 
    while (1) {
        char buffer[1024] = {};
        int result = g_tcpSocket.recFrom(buffer, sizeof(buffer), (SOCKET)(arg));
        if (result > 0 && result != INVALID_SOCKET) {
            cout << buffer << endl;
        }
    }
    return nullptr;
}

int inputAccountInfo()
{
    char sendBuff[1024] = {};

    while (1) {
        memset(sendBuff, 0, sizeof(sendBuff));
        cin.getline(sendBuff, sizeof(sendBuff));
        if (stricmp(sendBuff, "!quit") == 0) {
            g_tcpSocket.sendTo(sendBuff, sizeof(sendBuff), g_tcpSocket.localSocket());
            return -1;
        }
        else if (Utility::isInputedAccountRight(sendBuff)) {
            break;
        }
        else {
            cout << "Please input alphabet!" << endl;
        }
    }

    g_tcpSocket.sendTo(sendBuff, sizeof(sendBuff), g_tcpSocket.localSocket());
    return 0;
}

int inputPassword()
{
    char sendBuff[1024] = {};

    while (1) {
        memset(sendBuff, 0, sizeof(sendBuff));
        cin.getline(sendBuff, sizeof(sendBuff));
        if (stricmp(sendBuff, "!quit") == 0) {
            g_tcpSocket.sendTo(sendBuff, sizeof(sendBuff), g_tcpSocket.localSocket());
            return -1;
        }
        else if (Utility::isInputedPasswordRight(sendBuff)) {
            break;
        }
        else {
            cout << "Please input digit!" << endl;
        }
    }

    g_tcpSocket.sendTo(sendBuff, sizeof(sendBuff), g_tcpSocket.localSocket());
    return 0;
}

int inputMessage()
{
    char sendBuff[1024] = {};
    auto StringCat = [](char* destStr, const char* srcStr) {
        if (strlen(destStr) + strlen(srcStr) < 1024) {
            strcat(destStr, srcStr);
        }
    };

    while (1) {
        char inputBuff[1024] = {};
        
        cin.getline(inputBuff, sizeof(inputBuff));

        if (stricmp(inputBuff, "!quit") == 0) {
            g_tcpSocket.sendTo(sendBuff, sizeof(sendBuff), g_tcpSocket.localSocket());
            break;
        }
        else if (inputBuff[strlen(inputBuff) - 1] == '\\') {
            StringCat(sendBuff, inputBuff);
            StringCat(sendBuff, "\n");
        }
        else {
            StringCat(sendBuff, inputBuff);
            g_tcpSocket.sendTo(sendBuff, sizeof(sendBuff), g_tcpSocket.localSocket());
            memset(sendBuff, 0, sizeof(sendBuff));
        }
    }

    return -1;
}

int main()
{
    if (g_tcpSocket.initial() != 0) {
        exit(-1);
    }

    SOCKET clientSocket = g_tcpSocket.createSocket();  

    if (g_tcpSocket.connectSocket() == SOCKET_ERROR) {
        closesocket(clientSocket);
        cout << "Connect failed!" << endl;
        exit(0);
    }

    char recvBuff[1024] = {};

    while (1) {
        memset(recvBuff, 0, sizeof(recvBuff));

        g_tcpSocket.recFrom(recvBuff, sizeof(recvBuff), clientSocket);

        if (strcmp(recvBuff, inputAcc) == 0) {
            cout << inputAcc << endl;
            if (inputAccountInfo() == -1) {
                break;
            }
        } 
        else if (strcmp(recvBuff, inputPW) == 0) {
            cout << inputPW << endl;
            if (inputPassword() == -1) {
                break;
            }
        } 
        else if (strcmp(recvBuff, successStr) == 0) {
            cout << successStr << endl;
            cout << "You can chat now!" << endl;

            thread receiveThread(recThreadFunc, (void*)(clientSocket));
            receiveThread.detach();

            if (inputMessage() == -1) {
                break;
            }
        }
        else {
        }
    }

    shutdown(clientSocket, 2);
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
