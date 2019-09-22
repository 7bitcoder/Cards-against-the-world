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
#define LEN 1024

class tcpListener
{
private:
	const char* passCode = "a7dzRwQjnw5kW6uEnhx7"; // "o6EA9s4XGw1pRCtvqg02stfvfAlp8p";
	int port;
	std::string ip;
	SOCKET sock;
	SOCKET clientSocket;
	char buff[LEN];
	char code[21] = { 0 };
	char newLobby[2] = { 0 };
	char nickname[31] = { 0 };
	char lobbyId[31] = { 0 };
public:
	bool sendAll(const char* data, int length);
	bool checkData();
	tcpListener(int port, std::string ipv4 ="127.0.0.1");
	~tcpListener();
	bool init();
	bool run();
	void closeConnection() { closesocket(clientSocket); }
	std::string getLobby() { return std::string(lobbyId); }
	bool isNewLobby() { return newLobby[0] == 'y'; }
	std::string getNickname() { return std::string(nickname); }
	SOCKET getNewClientSocket() { return clientSocket; }
	void wait() { // check if socket is ready to write
		fd_set tmp;
		FD_ZERO(&tmp);
		FD_SET(clientSocket, &tmp);
		int iResult = select(0, nullptr, &tmp, nullptr, nullptr);
	}
};

