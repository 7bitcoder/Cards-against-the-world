#include "chat.h"


bool chat::disconnect(SOCKET socket)
{
	printf("message error close socket\n");
	std::string msg = clients[socket].nick + " disconnected.";
	if (!broadCast(socket, msg)) {
		//todo
	}
	closesocket(socket);
	FD_CLR(socket, &fds);
	clients.erase(socket);
}
chat::~chat() {
	for (int i = 0; i < fds.fd_count; i++)
	{
		SOCKET socket = fds.fd_array[i];
		closesocket(socket);
	}
}
bool chat::broadCast(SOCKET socket, std::string msg)
{
	for (int i = 0; i < fds.fd_count; i++)
	{
		SOCKET outSock = fds.fd_array[i];
		if (outSock != socket && outSock != listenSocket)
		{
			if (!sendAll(outSock, msg.c_str(), msg.size()))
				continue;
		}
	}
	return true;
}
bool chat::sendAll(SOCKET socket, const char* data, int length)
{
	int iResult;
	int count = 0;
	while (count < length) {
		iResult = send(socket, data + count, length, 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			return false;
		}
		count += iResult;
		length -= iResult;
	}
	return true;
}
bool chat::computeNewClientData(SOCKET client)
{
	char code[21] = { 0 };
	char newLobby[2] = { 0 };
	char nickname[31] = { 0 };
	char lobbyId[31] = { 0 };
	ZeroMemory(&code, sizeof(code));
	ZeroMemory(&newLobby, sizeof(newLobby));
	ZeroMemory(&nickname, sizeof(nickname));
	ZeroMemory(&lobbyId, sizeof(lobbyId));
	printf("all : %s\n", buff);
	strncpy_s(code, buff, 20);
	strncpy_s(newLobby, buff + 22, 1);
	strncpy_s(nickname, buff + 24, 30);
	strncpy_s(lobbyId, buff + 55, 30);
	printf("%s\n", code);
	printf("%s\n", newLobby);
	printf("%s\n", nickname);
	printf("%s\n", lobbyId);
	int cmp = strcmp(passCode, code);
	if (cmp != 0)
	{
		printf("wrong code: %d", cmp);
		closesocket(client);
		return false;
	}
	printf("code is ok");
	return true;
	clients[client].nick = nickname;
}
bool chat::limitedResponseWait(int time, SOCKET socket)
{
	TIMEVAL tv = { time,0 };

	// Set up the file descriptor set.
	fd_set tmp;
	FD_ZERO(&tmp);
	FD_SET(socket, &tmp);

	// Set up the struct timeval for the timeout.

	// Wait until timeout or data received.
	int iResult = select(0, &fds, NULL, NULL, &tv);
	if (iResult == SOCKET_ERROR) {
		printf("select failed with error: %d\n", WSAGetLastError());
		closesocket(socket);
		return false;
	}
	else if (iResult == 0) {
		printf("response time for code is is up");
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
chat::chat(SOCKET socket_, std::string lobbyId_, std::string port_)
{
	listenSocket = socket_;
	lobbyId = lobbyId_;
	port = port_;
	FD_ZERO(&fds);
	FD_SET(listenSocket, &fds);
}
bool chat::acceptNewClient()
{
	SOCKET client = accept(listenSocket, nullptr, nullptr);
	if (client == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(client);
		return false;
	}
	limitedResponseWait(5, client);
	computeNewClientData(client);
	FD_SET(client, &fds);
	std::string welcome = "Welcome to lobby: " + lobbyId;
	printf("new user\n");
	if (!sendAll( client, welcome.c_str(), welcome.size() + 1)) {
		//TODO co zrobic ???
	}
	welcome = clients[client].nick + " joined lobby.";
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
		printf("socket wait\n");
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		printf("after select /interval/interrupted\n");
		for (int i = 0; i < socketCount; i++) {
			printf("in for %d\n", i);
			SOCKET sock = copy.fd_array[i];
			if (sock == listenSocket) {
				if (!acceptNewClient())
					return;
			}
			else {//get message
				ZeroMemory(rcvbuff, strlen(rcvbuff));
				int bytesIn = recv(sock, rcvbuff, strlen(rcvbuff), 0);
				printf("Message: %s\n", rcvbuff);
				if (bytesIn <= 0) {
					disconnect(sock);
				}
				else {
					std::string msg = clients[sock].nick + ": " + rcvbuff;
					if (!broadCast(sock, msg)) {
						//todo
					}
				}
			}
		}
	}
}
