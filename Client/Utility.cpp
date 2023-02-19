#include "Utility.h"

//Account name must be alphabet
bool Utility::isInputedAccountRight(const CString& accountName)
{
	for (int i = 0; i < accountName.GetLength(); i++) {
		if (!isalpha(accountName[i])) {
			return false;
		}
	}

	return true;
}

//Account name must be digit
bool Utility::isInputedPasswordRight(const CString& password)
{
	for (int i = 0; i < password.GetLength(); i++) {
		if (!isdigit(password[i])) {
			return false;
		}
	}

	return true;
}
