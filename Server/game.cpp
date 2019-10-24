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
	const char Ready = 7;// - Ready send info that player is ready
	const char notReady = 8;// - notReady player is not ready
	const char start = 9;// - start if all players are ready then start game - change state LEADER
	const char dequeUpdate = 10;// - update deque
	const char timeUpdate = 11;// - update game time / rounds
	const char notAllPlayersAreReady = 12; // not all pleyrs arre ready
	const char sendWhiteDequeLen = 13;//send white deque len;
	const char sendBlackDequeLen = 14;
	const char sendRandomTenCards = 15; //send init cards to players
	const char sendChoserId = 16;//send info to all plaers that id of player is id of choser
	const char sendBlackCard = 17;//send black card to players
	const char sendChosenWhiteCards = 18;//send one or 2 chosen cards from normal players to choser
	const char getRandomWhiteCardsRequest = 19;//when player time is up sent it to him
	const char getRandomWhiteCards = 20;//get random cards from request ^
	const char choserWinnerId = 21;//server get from choser winner
	const char getRandomWinnerRequest = 22;//if time is up request for random one
	const char getRandomWinner = 23;//response ^
	const char sendWinner = 24;//send to all player winner to update points;
	const char gameIsOver = 25;//time is up or rounds
	const char shuffleWhiteDeque = 26;//shuffle deque LEADER concept if cards are reapiting then shuffle it
	const char playerIsNotResponsing = 27;//if pleyer is not responsing send it to rest and stop game

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
bool game::rejectNewClient() {
	SOCKET client = accept(listenSocket, nullptr, nullptr);
	if (client == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(client);
		return false;
	}
	int iResult = receiveLen(client, buff, coding, playerId, 2, 0);//wait 2 sec
	if (iResult <= 0)
	{
		printf("receive failed with error: %d\n", WSAGetLastError());
		closesocket(client);
		return false;
	}//idk if necesary 
	addMessagePrefix(buff, 1, codes::getId, ID);//add error lobby is already in game
		if (!sendLen(client, buff, 4)) {
		//todo
	}
	closesocket(client);
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
					int additionlInfo = getMessagePrefix(buff, code, playerID);
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
						lockLobby = true;
						return states::starting;
						break;
						case codes::sendWhiteDequeLen:
							white.init(aditionalInfo);
							break;
							
						case codes::sendBlackDequeLen:
							black.init(aditionalInfo);
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
states game::starting()
{
	//deques shuffle;
	white.shuffle();
	black.shuffle();
	//send to all players 10 init cards
	addMessagePrefix(buff, 1, codes::sendRandomTenCards, 0);
	for(auto & player : clients){
		for(int i = 0; i < 10 ; i++){
			unit16_t id = white.getCard();
			id = htons(id);
			memcpy(buff + 4 + i*2, (char*)& id, 2);//add to raw data units 16
		}
		if (!sendLen(player->first, buff, 4 + 10*2))
		//todo
	}
	return state::questionInit;
}
states game::questionInit(){
	//get new choser
	int min = 10;
		auto it =  in clients smollest in range choser : 10
	if(min == 10)
		choser = 1; //leader = first new round
	
	//send info choser
	addMessagePrefix(buff, 1, codes::sendChoserId, choser);
	broadCast(0,buff,4,true)
	
	//send black card
	int id = black.getCard();
	addMessagePrefix(buff, 1, codes::sendBlackCard, choser);
	broadCast(0,buff,4,true)
		return state::question;
}
state game::question(){
	chrono time start now
	TIMEVAL tv = { 60*3 , 0 };//3 min
while (true) {
		if (!clients.size())
			return states::kill;
		fd_set copy = fds;
	chrono get time;
	durr  = time  - start in microsec
		tv.tv_sec -= durr / 1000000;
		tv.tv_usec -= durr % 1000000;
		int socketCount = select(0, &copy, nullptr, nullptr, time);
		if (socketCount == SOCKET_ERROR) {
			printf("select failed with error: %d\n", WSAGetLastError());
			continue;
		}
		else if (socketCount == 0) {
			printf("response time for data is is up\n");
			//todo
		}
		printf("after xd\n");
		for (int i = 0; i < socketCount; i++) {
			printf("in for %d\n", i);
			SOCKET sock = copy.fd_array[i];
			if (sock == listenSocket) {//listen
				rejectNewClient()
					continue;
			}///TODO if leared is disconnected what happens?????
			else {//get message
				int i = recv(sock, buff, 4, 0);
				if (i <= 0) //TODO
					disconnect(sock);
				else {//decode
					char code, playerID;
					int additionalInfo = getMessagePrefix(buff, code, playerID);
					switch (code) {
					case codes::Ready://ready
						break;
					case codes::notReady://notReady
						break;
					case codes::start://start game
						break;
					default:
						break;
					}
				}
			}
		}
	}
	return states::choseInit;	
}
states game::choseinit(){
	
}
game::~game()
{
	for (int i = 0; i < fds.fd_count; i++)
	{
		SOCKET socket = fds.fd_array[i];
		closesocket(socket);
	}
}
