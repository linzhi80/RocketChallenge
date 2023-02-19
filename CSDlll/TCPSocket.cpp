#include "pch.h"
#include "TCPSocket.h"

#pragma comment(lib, "ws2_32.lib")

SOCKET TCPSocket::createSocket()
{
	m_localSock = socket(AF_INET, SOCK_STREAM, 0);
	return m_localSock;
}

int TCPSocket::bindSocket()
{
	int bindRes = bind(m_localSock, (SOCKADDR*)&m_localSockAddr, sizeof(SOCKADDR));
	if (bindRes == SOCKET_ERROR) {
		WSAGetLastError();
		return 0;
	}
	return 1;
}

int TCPSocket::connectSocket()
{
	return connect(m_localSock, (SOCKADDR*)(&m_localSockAddr), sizeof(SOCKADDR));
}

int TCPSocket::listenSocket()
{
	return (listen(m_localSock, 5) == 0) ? 1 : 0;
}

SOCKET TCPSocket::acceptSocket()
{
	int len = sizeof(m_clientSockAddr);
	m_clientSock = accept(m_localSock, (sockaddr*)(&m_clientSockAddr), &len);

	return m_clientSock;
}

int TCPSocket::recFrom(const char* buffer, int buffLen, SOCKET fromSocket, int flags)
{
	return recv(fromSocket, const_cast<char*>(buffer), buffLen, flags);
}

int TCPSocket::sendTo(const char* buffer, int buffLen, SOCKET toSocket, int flags)
{
	return send(toSocket, const_cast<char*>(buffer), buffLen, flags);
}
