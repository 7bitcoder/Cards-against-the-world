#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <map>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define LEN 1024
#define DEFAULT_PORT "3000"
struct client
{
	std::string nick;
};

class chat
{
protected:
	const char* passCode = "a7dzRwQjnw5kW6uEnhx7";
	char buff[LEN];
	char rcvbuff[LEN];
	std::map<SOCKET, client> clients;
	fd_set fds;
	SOCKET listenSocket;
	std::string port;
	std::string lobbyId;
public:
	chat(SOCKET socket_, std::string lobbyId_, std::string port_);
	~chat();
	void run();
	bool acceptNewClient();
	bool limitedResponseWait(int time, SOCKET socket);
	bool computeNewClientData(SOCKET client);
	bool broadCast(SOCKET socket, std::string msg);
	bool sendAll(SOCKET socket, const char* data, int length);
	bool disconnect(SOCKET socket);
	void wait(SOCKET socket) { // check if socket is ready to write
		fd_set tmp;
		FD_ZERO(&tmp);
		FD_SET(socket, &tmp);
		int iResult = select(0, nullptr, &tmp, nullptr, nullptr);
	}
};


