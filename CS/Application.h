#pragma once
#include <winsock2.h>
#include <CString>

class Application
{
public:
	Application();
	~Application();

	int initial();
	int ready();
	void run();
};

