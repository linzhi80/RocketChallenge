#include <atlstr.h>
#include <vector>

#pragma once

using namespace std;
class IOFile
{
public:
	static void write(const CString &fileName, const CString& string);
	static void write(const CString& fileName, const vector<CString>& strings);
	static vector<CString> read(const CString& fileName, bool addEnterFlag = false);

private:
	IOFile() = delete;
	IOFile(const IOFile&) = delete;
	IOFile& operator = (const IOFile&) = delete;
};

