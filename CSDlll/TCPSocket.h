#pragma once
#include "Socket.h"
class TCPSocket : public Socket
{
public:
	TCPSocket() : Socket() {}
	virtual ~TCPSocket() {}

	_declspec (dllexport) virtual SOCKET createSocket() override;
	_declspec (dllexport) virtual int bindSocket() override;
	_declspec (dllexport) virtual int connectSocket() override;
	_declspec (dllexport) virtual int listenSocket() override;
	_declspec (dllexport) virtual SOCKET acceptSocket() override;
	_declspec (dllexport) virtual int recFrom(const char* buffer, int buffLen, SOCKET fromSocket = 0, int flags = 0) override;
	_declspec (dllexport) virtual int sendTo(const char* buffer, int buffLen, SOCKET toSocket = 0, int flags = 0) override;
};

