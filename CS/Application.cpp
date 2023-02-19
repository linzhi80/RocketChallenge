#include <thread>
#include <iostream>
#include <string.h>
#include <direct.h>
#include <cstdio>
#include <ctime>
#include "Application.h"
#include "ClientManager.h"
#include "../CSDlll/TCPSocket.h"
#include "IOParser.h"
#include "IOFile.h"

#pragma comment(lib, "..\\CS\\x64\\Debug\\CSDlll.lib")

constexpr const char* inputAcc = "Input your Account:";
constexpr const char* inputPW = "Input your Password:";
constexpr const char* successStr = "Connect successfully!";
constexpr const char* noDest = "The user isn't exist!";

TCPSocket g_tcpSocket;
ClientManager* g_clientManager = ClientManager::instance();

struct ClientInfo {
    CString     accountName;
    SOCKET      clientSock;
    SOCKADDR_IN clientAddr;
};
ClientInfo gClientInfo{};

void* threadFunc(void* arg)
{
    if (arg == nullptr)
        return nullptr;

    SOCKET clientSock = ((ClientInfo*)arg)->clientSock;
    SOCKADDR_IN clientAddr = ((ClientInfo*)arg)->clientAddr;
    CString accountName = ((ClientInfo*)arg)->accountName;

    //get message time
    auto messageTime = []() {
        time_t currentTime;
        time(&currentTime);
        tm* localTime = localtime(&currentTime);
        CString formatTime;
        formatTime.Format(_T("%4d:%2d:%2d %2d:%2d:%2d"), localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
                                                         localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

        return formatTime;
    };

    auto sendMesage = [=](SOCKET destSocket, const char* sendBuff, const CString& srcName) {
        CString message(accountName + " said : " + IOParser::getClientMessage(CString(sendBuff)));

        USES_CONVERSION;
        //Save message to file if sending failed
        if (g_tcpSocket.sendTo(std::string(W2A(message)).c_str(), message.GetLength(), destSocket) <= 0) {
            //save message to file
            char path[512] = { 0 };
            if (_getcwd(path, sizeof(path) - 1) != NULL)
                IOFile::write(CString(path) + "\\" + srcName + ".txt", messageTime() + "\n" + message + "\n");
        }
    };

    while (1) {
        char buffer[1024] = {};
        int result = g_tcpSocket.recFrom(buffer, sizeof(buffer), clientSock);
        USES_CONVERSION;
        if (result == 0) {
            cout << inet_ntoa(clientAddr.sin_addr) << " " << std::string(W2A(accountName)).c_str() << " is disconnected!" << endl;
            break;
        }
        else if (result == SOCKET_ERROR) {
            cout << inet_ntoa(clientAddr.sin_addr) << " " << std::string(W2A(accountName)).c_str() << " Connection encoured error!" << endl;
            break;
        }
        else if (result == INVALID_SOCKET) {
            cout << inet_ntoa(clientAddr.sin_addr) << " " << std::string(W2A(accountName)).c_str() << " It's a invalid socket!" << endl;
            break;
        }
        else if (result > 0) {
            cout << std::string(W2A(messageTime())).c_str() << endl;
            cout << "Received message from " << inet_ntoa(clientAddr.sin_addr) 
                << " " << std::string(W2A(accountName)).c_str() << endl;

            //Analyze client's message, "@all" means broadcast message, "@name" means p2p, 
            //"!quit" means client is disconneted, others means sending message to server
            CString name(IOParser::getClientName(CString(buffer)));

            if (name.IsEmpty()) {
                if (stricmp(buffer, "!quit")) {
                    cout << std::string(W2A(accountName)).c_str() << " is offline" << endl;
                    break;
                } else {
                    cout << buffer << endl;
                }
            }
            else if (name == "all") {
                vector<pair<CString, SOCKET>> allSockets = g_clientManager->getAllClientSocket();
                for (const auto& destSocket : allSockets) {
                    if (clientSock != destSocket.second) {
                        sendMesage(destSocket.second, buffer, destSocket.first);
                    }
                }
            }
            else {
                if (g_clientManager->findAccount(name)) {
                    sendMesage(g_clientManager->getOneClientSocket(name), buffer, name);
                }
                else {
                    g_tcpSocket.sendTo(noDest, strlen(noDest), clientSock);
                }
            }
        }
    }
    shutdown(clientSock, 2);
    closesocket(clientSock);
    return nullptr;
}

Application::Application()
{
}
Application::~Application()
{
}

int Application::initial()
{
	return g_tcpSocket.initial();
}

int Application::ready()
{
	g_tcpSocket.createSocket();
	
	if (g_tcpSocket.bindSocket() == SOCKET_ERROR) {
		return 0;
	}

	if (g_tcpSocket.listenSocket() == 0) {
		return 0;
	}

	return 1;
}

void Application::run()
{
    cout << "Server start! " << endl;
	while (1) {
		SOCKET clientSocket = g_tcpSocket.acceptSocket();
		
		g_tcpSocket.sendTo(inputAcc, strlen(inputAcc), clientSocket);

		char buffer[1024] = {};

		while (1) {
			int result = g_tcpSocket.recFrom(buffer, sizeof(buffer), clientSocket);

			if (result > 0 && result != INVALID_SOCKET) {
				CString accountName(buffer);
				if (g_clientManager->findAccount(accountName)) {
					g_tcpSocket.sendTo(inputPW, strlen(inputPW), clientSocket);

					memset(buffer, 0, sizeof(buffer));
					while (1) {
						if (g_tcpSocket.recFrom(buffer, sizeof(buffer), clientSocket) != SOCKET_ERROR) {
							CString password(buffer);
							if (g_clientManager->checkAccountPassword(accountName, password)) {
								g_tcpSocket.sendTo(successStr, strlen(successStr), clientSocket);
								g_clientManager->loginClient(accountName, buffer, clientSocket);
								gClientInfo = { accountName, clientSocket , g_tcpSocket.localSockAddr()};
								thread handleThread(threadFunc, (void*)(&gClientInfo));
								handleThread.detach();

								//send history unreceived message
                                char path[512] = { 0 };
                                if (_getcwd(path, sizeof(path) - 1) != NULL) {
                                    CString fileName = CString(path) + "\\" + accountName + ".txt";
                                    vector<CString> unsendMessages = IOFile::read(fileName, true);
                                    USES_CONVERSION;
                                    remove(std::string(W2A(fileName)).c_str());
                                    for (const auto& message : unsendMessages) {
                                        g_tcpSocket.sendTo(std::string(W2A(message)).c_str(), message.GetLength(), clientSocket);
                                    }
                                }
                                break;
							}
                            else {
                                g_tcpSocket.sendTo(inputPW, strlen(inputPW), clientSocket);
                            }
						}
					}
				}
                else {
                    send(clientSocket, inputPW, strlen(inputPW), 0);
                    memset(buffer, 0, sizeof(buffer));
                    while (1) {
                        int res = g_tcpSocket.recFrom(buffer, sizeof(buffer), clientSocket);
                        if (res > 0 && res != INVALID_SOCKET) {
                            g_tcpSocket.sendTo(successStr, strlen(successStr), clientSocket);
                            g_clientManager->loginClient(accountName, buffer, clientSocket);
                            gClientInfo = { accountName, clientSocket , g_tcpSocket.localSockAddr() };
                            thread handleThread(threadFunc, (void*)(&gClientInfo));
                            handleThread.detach();
                            break;
                        }
                    }

                }

                break;
			}
		}
	}

    SOCKET localSocket = g_tcpSocket.localSocket();
    shutdown(localSocket, 2);
    closesocket(localSocket);
    WSACleanup();
}
