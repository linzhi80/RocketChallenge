#pragma once

#include <atlstr.h>

class IOParser
{
public:
	static CString getClientName(const CString& string);
	static CString getClientMessage(const CString& string);

private:
	IOParser() = delete;
	IOParser(const IOParser&) = delete;
	IOParser& operator = (const IOParser&) = delete;
};

