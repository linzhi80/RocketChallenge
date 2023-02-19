#pragma once
#include <atlstr.h>

class Utility
{
public:
	static bool isInputedAccountRight(const CString& accountName);
	static bool isInputedPasswordRight(const CString& password);

private:
	Utility() = delete;
	Utility(const Utility&) = delete;
	Utility& operator = (const Utility&) = delete;
};

