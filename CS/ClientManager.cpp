#include <winsock2.h>
#include <windows.h>
#include <direct.h>
#include "ClientManager.h"
#include "IOFile.h"

#pragma comment(lib, "ws2_32.lib")

ClientManager* ClientManager::g_own = nullptr;

ClientManager* ClientManager::instance()
{
	if (g_own == nullptr) {
		g_own = new ClientManager;
		g_own->loadAccountInfo();
		atexit(destroy);
	}
	return g_own;
}

void ClientManager::destroy()
{
	if (g_own != nullptr) {
		//close sockets...
		for (const auto &account : g_own->m_accountInfos) {
			shutdown(account.second.second, 2);
			closesocket(account.second.second);
		}
		delete g_own;
		g_own = nullptr;
	}
}

bool ClientManager::findAccount(const CString& userName) const
{
	return m_accountInfos.find(userName) != m_accountInfos.end();
}

//Check whether the inputed passord is the same as the registed password
bool ClientManager::checkAccountPassword(const CString& userName, const CString& password) const
{
	auto iter = m_accountInfos.find(userName);

	if (iter != m_accountInfos.end()) {
		return iter->second.first == password;
	}

	return false;
}
SOCKET ClientManager::getOneClientSocket(const CString& userName) const
{
	SOCKET socket = 0;

	auto iter = m_accountInfos.find(userName);

	if (iter != m_accountInfos.end()) {
		socket = iter->second.second;
	}

	return socket;
}

vector<pair<CString, SOCKET>> ClientManager::getAllClientSocket() const
{
	vector<pair<CString, SOCKET>> allSocket;

	for(const auto &accountInfo: m_accountInfos)
	{
		allSocket.push_back(make_pair(accountInfo.first, accountInfo.second.second));
	}

	return allSocket;
}

void ClientManager::loginClient(const CString& userName, const CString& password, UINT_PTR socket)
{
	auto iter = m_accountInfos.find(userName);

	if (iter == m_accountInfos.end()) {
		m_accountInfos.insert(std::make_pair(userName, pair<CString, SOCKET>(password, socket)));
		vector<CString>strings = { userName+"\n", password+"\n" };

		char path[512] = {0};
		if(_getcwd(path, sizeof(path) - 1) != NULL)
			IOFile::write(CString(path)+"\\AccountLoginInfo.txt", strings);
	}
	else {
		iter->second.second = socket;
	}
}

//Load account info from file when server is started
void ClientManager::loadAccountInfo()
{
	char path[512] = { 0 };
	if (_getcwd(path, sizeof(path) - 1) != NULL)
		return;
	vector<CString> accountInfos = IOFile::read(CString(path) + "\\AccountLoginInfo.txt");

	m_accountInfos.clear();

	int row = 0;
	CString userName, password;
	for (const auto& accountInfo : accountInfos)
	{
		if (row % 2) {
			password = accountInfo;
			m_accountInfos.insert(make_pair(userName, pair<CString, SOCKET>(password, 0)));
		}
		else {
			userName = accountInfo;
		}
		row++;
	}

}
