#include "tcpListener.h"
#include <uchar.h>
#ifndef TIME_WAIT
#define TIME_WAIT 100
#endif // !1

bool tcpListener::checkData()
{
	mbstate_t state;
	printf("all : %s\n", buff);
	code = decode(buff, 20);
	newLobby = decode( buff + 84, 1);
	nickname = decode( buff + 92, 30);
	lobbyId = decode( buff + 120, 30);
	if (passCode != code)
	{
		printf("wrong code: \n");
		closeConnection();
		return false;
	}
	printf("code is ok\n");
	return true;
}

tcpListener::tcpListener(int port_, std::string ipv4)
	: port(port_), ip(ipv4) {
	sock = INVALID_SOCKET;
	clientSocket = INVALID_SOCKET;
}

tcpListener::~tcpListener() {}
bool tcpListener::run() {
	printf("start servera\n");
	int iResult;

	printf("przed accpt\n");
	clientSocket = accept(sock, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		return false;
	}
	TIMEVAL tv = { TIME_WAIT ,0 };

	// Set up the file descriptor set.
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(clientSocket, &fds);

	// Set up the struct timeval for the timeout.

	// Wait until timeout or data received.
	iResult = select(0, &fds, NULL, NULL, &tv);
	if (iResult == SOCKET_ERROR) {
		printf("select failed with error: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		return false;
	}
	else if (iResult == 0) {
		printf("response time for code is is up\n");
		closesocket(clientSocket);
		return false;
	}
	printf("start\n");
	ZeroMemory(&buff, sizeof(buff));
	iResult = recv(clientSocket, buff, LEN, 0);
	if (iResult <= 0)
	{
		printf("receive failed with error: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		return false;
	}
	return true;
}

bool tcpListener::init()
{
	int iResult;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	// Initialize Winsock


	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return false;
	}

	// Create a SOCKET for connecting to server
	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}
	iResult = bind(sock, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(sock);
		WSACleanup();
		return false;
	}
	iResult = listen(sock, SOMAXCONN);

	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		return false;
	}
	freeaddrinfo(result);
	return true;
}
