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
			if (!sendRaw(outSock, buff, msg.size()))
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
			if (!sendRaw(outSock, buff, len))
				continue;
		}
	}
	return true;
}
bool chat::sendRaw(SOCKET socket, const char* data, int length)
{
        int count = 0;
        while (count < length) {
		wait(socket);
                int n = send(socket, data + count, length, 0);
                if (n == SOCKET_ERROR) {
                        return false;
                }
                count += n;
                length -= n;
        }
        return true;
}
bool chat::computeNewClientData(SOCKET client)
{
	std::u32string code;
	std::u32string newLobby;
	std::u32string nickname;
	std::u32string lobbyId;
	char * buff = this->buff + 4;
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
	char playerId;
	char coding;
	int len = getMessagePrefix(rcvBuff, coding, plyerId)
	if(!receiveLen(socket, buff + 4, len, 1))
		return false;
	if(iresult
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
	addMessagePrefix(buff, welcome.size()*4, 1, 0);
	code(welcome, buff + 4);
	if (!sendRaw( client, buff , welcome.size())) {
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
				if(!clients[sock].received) {
					int i = recv(socket, clients[sock].buff, 4, 0);
					if(i!= 4) //TODO
						disconnect(sock);
					char coding, playerId;
					clients[sock].all = getMessagePreffix(clients[sock].buff, coding, playerId)
					i = recv(socket, clients[sock].buff + 4, clients[sock].all , 0);
					if (i <= 0 ) {
					disconnect(sock);
					}
					if(i != clients[sock].all) {
						clients[sock].received = i;
						continue;//!!!!!+ 4
					}
					else
						clients[sock].received = 0;
					
				}
				else{
					int i = recv(socket, buff + clients[sock].received + 4, clients[sock].all - clients[sock].received , 0);
					if (i <= 0 ) {
						disconnect(sock);
					}
					if(i + clients[sock].received < clients[sock].all) {
						clients[sock].received += i;
						continue;
					}
					else {
						clients[sock].received = 0;
					}
				}
				printf("Message: %s\n", rcvbuff);
				if (len <= 0 ) {
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
