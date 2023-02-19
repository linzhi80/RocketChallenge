#include "IOParser.h"

CString IOParser::getClientName(const CString& string)
{
	CString name;

	int startPos = string.Find('@');

	if (startPos == 0) {
		int endPos = string.Find(' ');
		if (endPos > 1) {
			name = string.Mid(startPos + 1, endPos - startPos - 1);
		}
	}

	return name;
}

CString IOParser::getClientMessage(const CString& string)
{
	CString data;

	if (string.Find('@') == 0) {
		int startPos = string.Find(' ');
		if (startPos > 1) {
			data = string.Mid(startPos + 1);
		}
	}

	return data;
}
