#pragma once
#include "lobbyThread.h"
#define DEFAULT_BUFLEN 512
class MainThreadListener
{
public:
	SOCKET socket;
	std::string lobbyId;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	int listenPort;
	int iSendResult, iResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	MainThreadListener(SOCKET socket_, std::string lobbyId_);
	bool init();
	bool validateLeader();
	~MainThreadListener();
};

