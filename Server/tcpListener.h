#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#else
#endif // !WIN32_LEAN_AND_MEAN
#include "socketUtils.h"

class tcpListener: public socketUtils
{
private:
	int port;
	std::string ip;
	SOCKET sock;
	SOCKET clientSocket;
	char buff[LEN];
	std::u32string code;
	std::u32string newLobby;
	std::u32string nickname;
	std::u32string lobbyId;
public:
	bool checkData();
	tcpListener(int port, std::string ipv4 ="127.0.0.1");
	~tcpListener();
	bool init();
	bool run();
	void send(const char* buff, int len) { socketUtils::sendU(clientSocket, buff, len); }
	void closeConnection() { closesocket(clientSocket); }
	std::u32string getLobby() { return lobbyId; }
	bool isNewLobby() { return newLobby[0] == U'y'; }
	std::u32string getNickname() { return nickname; }
	SOCKET getNewClientSocket() { return clientSocket; }
	void wait() { // check if socket is ready to write
		fd_set tmp;
		FD_ZERO(&tmp);
		FD_SET(clientSocket, &tmp);
		int iResult = select(0, nullptr, &tmp, nullptr, nullptr);
	}
};

