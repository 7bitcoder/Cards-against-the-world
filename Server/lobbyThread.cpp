#pragma once
#include "lobbyThread.h"

std::mutex mut;
std::map<std::string, int> mapaLobby;

#define DEFAULT_BUFLEN 512
void lobbyThread(SOCKET socket_, std::string lobbyId)//, std::mutex, std::map<std::string, int>)
{
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo* result = NULL;
	struct addrinfo hints;
	
	int iSendResult, iResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock

	printf("startin thread\n");
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		return ;
	}
	sockaddr_in addr;
	int size = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(0);
	addr.sin_addr.s_addr = INADDR_ANY;
	printf("binding with %d port thread\n",0);
	iResult = bind(ListenSocket, (struct sockaddr*) & addr, size);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		return ;
	}
	getsockname(ListenSocket, (struct sockaddr*) & addr, &size);
	int listenPort = ntohs(addr.sin_port);
	freeaddrinfo(result);
	mut.lock();
	mapaLobby[lobbyId] = listenPort;
	mut.unlock();

	std::string msg = std::to_string(listenPort);
	send(socket_,msg.c_str(), msg.size(), 0);
	Sleep(1);
	closesocket(socket_);
	printf("listening with %d port thread\n", listenPort);
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		return ;
	}

	// Accept a client socket
	printf("accepting with %d port thread\n", listenPort);
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		return ;
	}
	TIMEVAL tv = { 1000,0 };

	// Set up the file descriptor set.
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(ClientSocket, &fds);

	// Set up the struct timeval for the timeout.

	// Wait until timeout or data received.
	printf("accepted with %d port thread\n", listenPort);
	iResult = select(0, &fds, NULL, NULL, &tv);
	if (iResult == SOCKET_ERROR) {
		printf("select failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		return;
	}
	else if (iResult == 0) {
		printf("response time for code is is up");
		closesocket(ClientSocket);
		return;
	}
	printf("start\n");
	char buff[500];
	ZeroMemory(&buff, sizeof(buff));
	iResult = recv(ClientSocket, buff, 500, 0);
	if (iResult <= 0)
	{
		printf("receive failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		return ;
	}

	printf("received and exiting\n", listenPort);
	printf("%s\n", buff);
	closesocket(ClientSocket);
	return ;
}

