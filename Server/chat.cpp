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
			if (!sendLen(outSock, buff, msg.size()))
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
			if (!sendLen(outSock, buff, len))
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
	code = decode(buff + 4, 20);
	newLobby = decode(buff + 88, 1);
	nickname = decode(buff + 96, 30);
	lobbyId = decode(buff + 124, 30);
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
	printf("start\n");
	ZeroMemory(&buff, sizeof(buff));
	char playerId;
	char coding;
	if(!receiveLen(socket, buff, coding, playerId))
		return false;
	if (coding == 2)
		return false; //TODO
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
	addMessagePrefix(buff, welcome.size() * 4, 1, 0);
	code(welcome, buff + 4);
	if (!sendLen(client, buff, welcome.size())) {
		//TODO co zrobic ???
	}
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
				if (!clients[sock].received) {
					int i = recv(sock, clients[sock].buff, 4, 0);
					if (i != 4) //TODO
						disconnect(sock);
					char coding, playerId;
					clients[sock].all = getMessagePreffix(clients[sock].buff, coding, playerId)
						i = recv(socket, clients[sock].buff + 4, clients[sock].all, 0);
					if (i <= 0) {
						disconnect(sock);
					}
					if (i != clients[sock].all) {
						clients[sock].received = i;
						continue;//!!!!!+ 4
					}
					else
						clients[sock].received = 0;

				}
				else {
					int i = recv(socket, buff + clients[sock].received + 4, clients[sock].all - clients[sock].received, 0);
					if (i <= 0) {
						disconnect(sock);
					}
					if (i + clients[sock].received < clients[sock].all) {
						clients[sock].received += i;
						continue;
					}
					else {
						clients[sock].received = 0;
					}
				}
				printf("Message: %s\n", rcvbuff);
				if (len <= 0) {
					disconnect(sock);
				}

				else {
					if (!broadCast(sock, clients[sock].buff, clients[sock].all + 4)) {
						//todo
					}
					clients[sock].all = 0;
				}
			}
		}
	}
}
