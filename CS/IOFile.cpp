#include <fstream>
#include <iostream>
#include <string>

#include "IOFile.h"

void IOFile::write(const CString& fileName, const CString& string)
{
	ofstream ofs;
	
	ofs.open(fileName, ios::app);

	USES_CONVERSION;

	ofs.write(std::string(W2A(string)).c_str(), string.GetLength());

	ofs.close();
}

void IOFile::write(const CString& fileName, const vector<CString>& strings)
{
	ofstream ofs;

	ofs.open(fileName, ios::app);

	USES_CONVERSION;
	for (const auto& string : strings) {
		ofs.write(std::string(W2A(string)).c_str(), string.GetLength());
	}

	ofs.close();
}

vector<CString> IOFile::read(const CString& fileName, bool addEnterFlag)
{
	ifstream ifs;
	vector<CString> unsendMessages;

	ifs.open(fileName, ios::in);

	if (ifs.is_open()) {
		char oneRowString[1024];
		while (ifs.getline(oneRowString, sizeof(oneRowString))) {
			if (addEnterFlag) {
				unsendMessages.push_back(strcat(oneRowString, "\n"));
			}
			else {
				unsendMessages.push_back(oneRowString);
			}
		}
	}

	ifs.close();
	return unsendMessages;
}