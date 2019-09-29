#pragma once
#include "lobbyThread.h"
#define DEFAULT_BUFLEN 4024
class MainThreadListener
{
public:
	SOCKET oldSocket;
	std::u32string lobbyId;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	int listenPort;
	int iSendResult, iResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	MainThreadListener(SOCKET socket_, std::u32string lobbyId_);
	int createListenSocket();
	bool acceptLeaderConnection();
	~MainThreadListener();
};