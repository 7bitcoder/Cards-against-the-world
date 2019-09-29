#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#else
#endif // !WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>


#pragma comment (lib, "Ws2_32.lib")
#define LEN 4024

class tcpListener
{
private:
	std::u32string passCode = U"a7dzRwQjnw5kW6uEnhx7"; // "o6EA9s4XGw1pRCtvqg02stfvfAlp8p";
	int port;
	std::string ip;
	SOCKET sock;
	SOCKET clientSocket;
	char buff[LEN];
	int stop;
	std::u32string code;
	std::u32string newLobby;
	std::u32string nickname;
	std::u32string lobbyId;
public:
	bool sendAll(const char* data, int length);
	std::u32string decode(char* source, int limit);
	bool checkData();
	tcpListener(int port, std::string ipv4 ="127.0.0.1");
	~tcpListener();
	bool init();
	bool run();
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

