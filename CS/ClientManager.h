#pragma once
#include <map>
#include <vector>
#include <utility>
#include <atlstr.h>
#include <winsock2.h>

//class Client;
using namespace std;
struct AccountInfo{
	CString	userName;
	CString password;
};

//singelton
class ClientManager
{
private:
	map<CString, pair<CString, SOCKET>> m_accountInfos = {};

private:
	static ClientManager* g_own;
	ClientManager(){};

	static void destroy();
	void loadAccountInfo();

public:
	static ClientManager* instance();

	bool findAccount(const CString& userName) const;
	bool checkAccountPassword(const CString& userName, const CString& password) const;
	SOCKET getOneClientSocket(const CString& userName) const;
	vector<pair<CString, SOCKET>> getAllClientSocket() const;
	void loginClient(const CString& userName, const CString& password, UINT_PTR socket);
};