#include "chat.h"


bool chat::disconnect(SOCKET socket)
{
	printf("plyer disconnected\n");
	std::u32string msg = clients[socket].nick + U" disconnected.";
	if (!broadCast(socket, msg)) {
		//todo
	}
	closesocket(socket);
	FD_CLR(socket, &fds);
	clients.erase(socket);
	return true;
}
chat::~chat() {
	for (int i = 0; i < fds.fd_count; i++)
	{
		SOCKET socket = fds.fd_array[i];
		closesocket(socket);
	}
}
bool chat::broadCast(SOCKET socket, std::u32string msg)
{
	code(msg, buff);
	for (int i = 0; i < fds.fd_count; i++)
	{
		SOCKET outSock = fds.fd_array[i];
		if (outSock != socket && outSock != listenSocket)
		{
			if (!sendU(outSock, buff, msg.size() + 1))
				continue;
		}
	}
	return true;
}
bool chat::broadCast(SOCKET socket, char* buff, int len)
{
	for (int i = 0; i < fds.fd_count; i++)
	{
		SOCKET outSock = fds.fd_array[i];
		if (outSock != socket && outSock != listenSocket)
		{
			if (!sendU(outSock, buff, len))
				continue;
		}
	}
	return true;
}
bool chat::computeNewClientData(SOCKET client)
{
	std::u32string code;
	std::u32string newLobby;
	std::u32string nickname;
	std::u32string lobbyId;
	mbstate_t state;
	printf("all : %s\n", buff);
	code = decode(buff, 20);
	newLobby = decode(buff + 84, 1);
	nickname = decode(buff + 92, 30);
	lobbyId = decode(buff + 120, 30);
	if (passCode != code)
	{
		printf("wrong code: \n");
		closesocket(client);
		return false;
	}
	printf("code is ok\n");
	clients[client].nick = nickname;
	return true;
}
bool chat::limitedResponseWait(int time, SOCKET socket)
{
	TIMEVAL tv = { time, 0 };

	// Set up the file descriptor set.
	fd_set tmp;
	FD_ZERO(&tmp);
	FD_SET(socket, &tmp);

	// Set up the struct timeval for the timeout.

	// Wait until timeout or data received.
	int iResult = select(0, &tmp, NULL, NULL, &tv);
	if (iResult == SOCKET_ERROR) {
		printf("select failed with error: %d\n", WSAGetLastError());
		closesocket(socket);
		return false;
	}
	else if (iResult == 0) {
		printf("response time for code is is up\n");
		closesocket(socket);
		return false;
	}
	printf("start\n");
	ZeroMemory(&buff, sizeof(buff));
	iResult = recv(socket, buff, LEN, 0);
	if (iResult <= 0)
	{
		printf("receive failed with error: %d\n", WSAGetLastError());
		closesocket(socket);
		return false;
	}
}
chat::chat(SOCKET socket_, std::u32string lobbyId_, std::string port_)
{
	printf("Creating chat object\n");
	listenSocket = socket_;
	lobbyId = lobbyId_;
	port = port_;
	FD_ZERO(&fds);
	FD_SET(listenSocket, &fds);
}
bool chat::acceptNewClient()
{
	printf("try to accept chat client\n");
	SOCKET client = accept(listenSocket, nullptr, nullptr);
	if (client == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(client);
		return false;
	}
	if (!limitedResponseWait(100, client))
		return false;
	if (!computeNewClientData(client))
		return false;
	FD_SET(client, &fds);
	std::u32string welcome = U"Welcome to lobby: " + lobbyId;
	printf("new user\n");
	code(welcome, buff);
	//if (!sendU( client, buff , welcome.size() + 1)) {
		//TODO co zrobic ???
	//}
	welcome = clients[client].nick + U" joined lobby.";
	if (!broadCast(client, welcome))
	{
		//??????
	}
	return true;
}
void chat::run()
{
	while (true) {
		fd_set copy = fds;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		if (socketCount == SOCKET_ERROR) {
			printf("select failed with error: %d\n", WSAGetLastError());
		}
		printf("after xd\n");
		for (int i = 0; i < socketCount; i++) {
			printf("in for %d\n", i);
			SOCKET sock = copy.fd_array[i];
			if (sock == listenSocket) {
				if (!acceptNewClient())
					return;
			}
			else {//get message
				ZeroMemory(rcvbuff, strlen(rcvbuff));
				int len = receiveLen(sock, rcvbuff, LEN);
				printf("Message: %s\n", rcvbuff);
				if (len <= 0 ) {
					disconnect(sock);
				}
				
				else {
					if (!broadCast(sock, rcvbuff, len)) {
						//todo
					}
				}
			}
		}
	}
}
