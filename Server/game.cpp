#include "game.h"
#include <vector>

namespace codes {
	/*protokól komunikacji
	1 bajt  CODE
	0 - ZWYKŁE dane czyli raw data z sizem podanym w Size
	1 - UNICODE message size w bajtach podany w Size
	2 - ERROR kod erroru podany w playerId
	KOmendy :
	z kodem podanym  w playerId oraz ew danymi w Size
	2 bajtach SIZE
	short długość wiadomości zawsze podana w bajtach lub dodatkowe dane
	1 bajt PLAYERID
	 id gracza wysyłającego jeśli server to 0 , 1 to broadcast reszta to id gracza lub dodatkowe dane
	4+ bajty wiadomość
	*/
	const char raw = 0;//raw  data
	const char unicode = 1;//unicode string 
	const char error = 2;//error
	const char newPlayer = 3;// for others get id newplayer
	const char getId = 4;// get new player id;
	const char getLobbyInfo = 5;// get lobby nicks itp
	const char disconnect = 6;// disconnect player of this id 
	const char Ready = 7;// - Ready wcisniecie ready i wyslanie do servera i wszyatkich  zmiana na zielony
	const char notReady = 8;// - notReady to samo pu tylko not ready zmiana na czerowny
	const char start = 9;// - start leader jeśli wszyscy ready to zmiana stanu na grę LEADER
	const char dequeUpdate = 10;// - update talii //aktualnie jeszcze nie LEADER
	const char timeUpdate = 11;// - update czas gry LEADER
	const char notAllPlayersAreReady = 12; // nie wszyscy gracze są gotowi

}//TODO naprawnie deadlocka broadcast disconnect 
game::game(SOCKET listen, SOCKET leader_, std::u32string nick, std::u32string id)
{
	for (int i = 0; i < 9; i++) {
		free[i] = true;
	}
	lobbyId = id;
	leader = leader_;
	clients[leader].nick = nick;
	clients[leader].id = 1;
	clients[leader].ready = false;
	free[1] = false;
	listenSocket = listen;
	FD_SET(listenSocket, &fds);
	FD_SET(leader, &fds);
}
bool game::broadCast(SOCKET socket, char* buff, int len, bool all)
{
	std::vector<std::map<SOCKET, slot>::iterator> toDelete;
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		if (all || it->first != socket)
		{
			if (!sendLen(it->first, buff, len))
				toDelete.push_back(it);
		}
	}
	for (auto& x : toDelete)
		disconnect(x->first);
	return true;
}
bool game::computeNewClientData(SOCKET socket, std::u32string & nick)
{
	std::u32string code;
	std::u32string newLobby;
	std::u32string lobbyId;
	mbstate_t state;
	printf("all : %s\n", buff);
	code = decode(buff + 4, 20);
	newLobby = decode(buff + 88, 1);
	nick = decode(buff + 96, 0);
	lobbyId = decode(buff + 220, 0);
	if (passCode != code)
	{
		printf("wrong code: \n");
		return false;
	}
	printf("code is ok\n");
	return true;
}
bool game::acceptNewClient() {//TODO dorobienie zwracania kodu b³êdów
	int i = 0;
	for (i = 1; i < 9; i++) {
		if (free[i]) {
			break;
		}
	}
	int ID = i;
	if (!ID)
		return false;
	printf("try to accept chat client\n");
	SOCKET client = accept(listenSocket, nullptr, nullptr);
	if (client == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(client);
		return false;
	}

	char playerId;
	char coding;
	int iResult = receiveLen(client, buff, coding, playerId, 2, 0);//wait 2 sec
	if (iResult <= 0)
	{
		printf("receive failed with error: %d\n", WSAGetLastError());
		closesocket(client);
		return false;
	}
	std::u32string nick;
	if (!computeNewClientData(client, nick)) {
		closesocket(client);
		return false;
	}
	for (auto it = clients.begin(); it != clients.end(); it++) {
		if (it->second.nick == nick) {
			closesocket(client);
			return false;
		}
	}
	free[ID] = false;
	clients[client].nick = nick;
	clients[client].id = ID;
	clients[client].ready = false;
	FD_SET(client, &fds);
	//send new id to player
	addMessagePrefix(buff, 1, codes::getId, ID);
	if (!sendLen(client, buff, 4)) {
		//todo
	}
	//sent all info (about all players in lobby) to new client
	addMessagePrefix(buff, 1, codes::getLobbyInfo, clients.size());
	if (!sendLen(client, buff, 4)) {
		//todo
	}
	for (auto it = clients.begin(); it != clients.end(); it++) {
		if (it->first != client) {
			int size = it->second.nick.size() * 4;
			addMessagePrefix(buff, size, 1, it->second.id);
			code(it->second.nick, buff + 4);
			if (!sendLen(client, buff, size + 4)) {
				//TODO co zrobic ???
			}
		}
	}
	//sent to all players info about new player
	addMessagePrefix(buff, nick.size() * 4, codes::newPlayer, ID);
	code(nick, buff + 4);
	if (broadCast(client, buff, nick.size() * 4 + 4))
	{
		//TODO
	}
}
void game::disconnect(SOCKET socket) {
	if (clients.find(socket) != clients.end()) {
		int id = clients[socket].id;
		free[clients[socket].id] = true;
		clients.erase(socket);
		closesocket(socket);
		FD_CLR(socket, &fds);
		addMessagePrefix(buff, 1, 6, id);
		broadCast(socket, buff, 4);
	}
}
states game::waiting()
{
	while (true) {
		if (!clients.size())
			return states::kill;
		fd_set copy = fds;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		if (socketCount == SOCKET_ERROR) {
			printf("select failed with error: %d\n", WSAGetLastError());
			continue;
		}
		printf("after xd\n");
		for (int i = 0; i < socketCount; i++) {
			printf("in for %d\n", i);
			SOCKET sock = copy.fd_array[i];
			if (sock == listenSocket) {//listen
				if (!acceptNewClient())
					continue;
			}///TODO if leared is disconnected what happens?????
			else {//get message
				int i = recv(sock, buff, 4, 0);
				if (i <= 0) //TODO
					disconnect(sock);
				else {//decode
					char code, playerID;
					getMessagePrefix(buff, code, playerID);
					switch (code) {
					case codes::Ready://ready
						clients[sock].ready = true;
						broadCast(sock, buff, 4, true);//true to send all
						break;
					case codes::notReady://notReady
						clients[sock].ready = false;
						broadCast(sock, buff, 4, true);
						break;
					case codes::start://start game
						for (auto it = clients.begin(); it != clients.end(); it++) 
							if (it->first != leader)
								if (!it->second.ready) {
									addMessagePrefix(buff, 1, codes::notAllPlayersAreReady, 0);
									if (!sendLen(leader, buff, 4));
									//TODO
									break;
								}
						//start
						break;
					default:
						break;
					}
				}
			}
		}
	}
	return states();
}

game::~game()
{
	for (int i = 0; i < fds.fd_count; i++)
	{
		SOCKET socket = fds.fd_array[i];
		closesocket(socket);
	}
}
