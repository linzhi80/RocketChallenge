#include <winsock2.h>

#pragma once
#pragma comment(lib, "ws2_32.lib")

class Socket
{
public:
	Socket() {
		m_localSockAddr.sin_family = AF_INET;
		m_localSockAddr.sin_port = htons(6666);
		m_localSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	};

	virtual ~Socket() {

	}
	_declspec (dllexport) virtual SOCKET createSocket() = 0;
	_declspec (dllexport) virtual int bindSocket() = 0;
	_declspec (dllexport) virtual int connectSocket() = 0;
	_declspec (dllexport) virtual int listenSocket() = 0;
	_declspec (dllexport) virtual SOCKET acceptSocket() = 0;
	_declspec (dllexport) virtual int recFrom(const char * buff, int buffLen, SOCKET fromSocket = 0, int flags = 0) = 0;
	_declspec (dllexport) virtual int sendTo(const char* buff, int buffLen, SOCKET toSocket = 0, int flags = 0) = 0;

	_declspec (dllexport) int initial()
	{
		WORD wVer;
		WSADATA wsaData;
		wVer = MAKEWORD(2, 2);

		if (WSAStartup(wVer, &wsaData) != 0) {
			//WSAStartup Failed!"
			return 1;
		}

		if (wsaData.wVersion != wVer) {
			//The version of winsock isn't suited!
			WSACleanup();
			return 2;
		}

		return 0;
	}
	_declspec (dllexport) void setServerSockAddr(const SOCKADDR_IN& sockAddr)
	{
		m_localSockAddr = sockAddr;
	}
	_declspec (dllexport) SOCKADDR_IN localSockAddr() const { return m_localSockAddr; }
	_declspec (dllexport) SOCKET localSocket() const { return m_localSock; }
protected:
	SOCKET m_localSock = 0, m_clientSock = 0;
	SOCKADDR_IN m_localSockAddr{}, m_clientSockAddr{};
};

