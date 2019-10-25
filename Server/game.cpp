#include "game.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include <chrono>
#include <ratio>

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
	const char randomWhiteCardsRequest = 19;//when player time is up sent it to him
	const char getRandomWhiteCards = 20;//get random cards from request ^
	const char choserWinnerId = 21;//server get from choser winner
	const char randomWinnerRequest = 22;//if time is up request for random one
	const char getRandomWinner = 23;//response ^
	const char sendWinner = 24;//send to all player winner to update points;
	const char gameIsOver = 25;//time is up or rounds
	const char shuffleWhiteDeque = 26;//shuffle deque LEADER concept if cards are reapiting then shuffle it
	const char playerIsNotResponsing = 27;//if pleyer is not responsing send it to rest and stop game

}//TODO naprawnie deadlocka broadcast disconnect 
game::game(SOCKET listen, SOCKET leader_, std::u32string nick, std::u32string id)
{
	std::srand(std::time(nullptr));
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
	char coding, playerId;
	int iResult = receiveLen(client, buff, coding, playerId, 2, 0);//wait 2 sec
	if (iResult <= 0)
	{
		printf("receive failed with error: %d\n", WSAGetLastError());
		closesocket(client);
		return false;
	}//idk if necesary 
	addMessagePrefix(buff, 1, codes::error, 10);//char  = 10 is error lobby is locked
	if (!sendLen(client, buff, 4)) {
		//todo
	}
	closesocket(client);
}
bool game::acceptNewClient() {//TODO dorobienie zwracania kodu bledów
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
states game::waitingF()
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
					int additionalInfo = getMessagePrefix(buff, code, playerID);
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
						lock = true;
						addMessagePrefix(buff, 1, codes::start, 0);
						if (!broadCast(0, buff, 4, true)) {
							;//todo
						}
						return states::starting;
						break;
					case codes::sendWhiteDequeLen:
						white.init(additionalInfo);
						break;

					case codes::sendBlackDequeLen:
						black.init(additionalInfo);
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
states game::startingF()
{
	//deques shuffle;
	white.shuffle();
	black.shuffle();
	//send to all players 10 init cards
	addMessagePrefix(buff, 1, codes::sendRandomTenCards, 0);
	for (auto& player : clients) {
		for (int i = 0; i < 10; i++) {
			uint16_t id = white.getCard();
			id = htons(id);
			memcpy(buff + 4 + i * 2, (char*)& id, 2);//add to raw data units 16 *10 cards = cards id
		}
		if (!sendLen(player.first, buff, 4 + 10 * 2))
			;//todo
	}
	//init random choser
	int off = std::rand() % clients.size();
	auto it = clients.begin();
	while (off--) {
		it++;
	}
	choser = it;
	return states::questionInit;
}
states game::questionInitF() {

	//send black card
	uint16_t id = black.getCard();
	id = htons(id);
	memcpy(buff + 4, (char*)& id, 2);
	int id = black.getCard();
	addMessagePrefix(buff, 1, codes::sendBlackCard, 0);
	broadCast(0, buff, 6, true);

	//get new choser
	int min = 10;
	auto it = clients.end();
	for (auto x = clients.begin(); x != clients.end(); x++) {
		if (x->second.id > choser->second.id) {
			if (x->second.id < min) {
				min = x->second.id;
				it = x;
			}
		}
	}
	if (min == 10)
		choser = clients.find(leader); //leader = first new round
	else
		choser = it;

	//send info choser
	addMessagePrefix(buff, 1, codes::sendChoserId, choser->second.id);
	broadCast(0, buff, 4, true);

	return states::question;
}
states game::questionF() {
	cardsToSendToChoser.clear();
	alreadySended.clear();
	auto start = std::chrono::high_resolution_clock::now();
	TIMEVAL tv = { 60 * 3 , 0 };//3 min 0  sec

	while (true) {
		if (!clients.size())
			return states::kill;
		fd_set copy = fds;
		auto now = std::chrono::high_resolution_clock::now();
		auto durr = std::chrono::duration_cast<std::chrono::microseconds>(start - now);
		tv.tv_sec -= durr.count() / 1000000;
		tv.tv_usec -= durr.count() % 1000000;
		if (tv.tv_sec <= 0 && tv.tv_usec <= 0) {
			return states::questionOvertime;
		}
		int socketCount = select(0, &copy, nullptr, nullptr, &tv);
		if (socketCount == SOCKET_ERROR) {
			printf("select failed with error: %d\n", WSAGetLastError());
			continue;
		}
		else if (socketCount == 0) {
			return states::questionOvertime;
		}
		printf("after xd\n");
		for (int i = 0; i < socketCount; i++) {
			printf("in for %d\n", i);
			SOCKET sock = copy.fd_array[i];
			if (sock == listenSocket) {//listen
				rejectNewClient();
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
					case codes::sendChosenWhiteCards://get cards to send to choser
					{
						int len = 6;
						uint16_t* ptr = (uint16_t*)(buff + 4);
						cardsToSendToChoser.push_back(ntohs(*ptr));
						if (additionalInfo > 2) {
							len += 2;
							ptr = (uint16_t*)(buff + 6);
							cardsToSendToChoser.push_back(ntohs(*ptr));
						}
						if (!sendLen(choser->first, buff, len));
							;//todo
						if (alreadySended.size() == clients.size() - 1)//all players sended cards
							return states::choseInit;
					}
					default:
						break;
					}
				}
			}
		}
	}
	return states::choseInit;
}
states game::questionOvertimeF()
{
	for (auto& x : clients) {
		if (x.first != choser->first) {
			auto it = std::find(alreadySended.begin(), alreadySended.end(), x.second.id);
			if (it == alreadySended.end()) {//if player is did not send cards 
				addMessagePrefix(buff, 1, codes::randomWhiteCardsRequest, 0);
				if (!sendLen(x.first, buff, 4))
					;//todo
			}
		}
	}
	auto start = std::chrono::high_resolution_clock::now();
	TIMEVAL tv = { 5 , 0 };//3 min 0  sec

	while (true) {
		if (!clients.size())
			return states::kill;
		fd_set copy = fds;
		auto now = std::chrono::high_resolution_clock::now();
		auto durr = std::chrono::duration_cast<std::chrono::microseconds>(start - now);
		tv.tv_sec -= durr.count() / 1000000;
		tv.tv_usec -= durr.count() % 1000000;
		if (tv.tv_sec <= 0 && tv.tv_usec <= 0) {
			break;
		}
		int socketCount = select(0, &copy, nullptr, nullptr, &tv);
		if (socketCount == SOCKET_ERROR) {
			printf("select failed with error: %d\n", WSAGetLastError());
			continue;
		}
		else if (socketCount == 0) {
			break;
		}
		printf("after xd\n");
		for (int i = 0; i < socketCount; i++) {
			printf("in for %d\n", i);
			SOCKET sock = copy.fd_array[i];
			if (sock == listenSocket) {//listen
				rejectNewClient();
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
					case codes::sendChosenWhiteCards://get cards to send to choser
					{
						int len = 6;
						char16_t ch; mbstate_t state{};
						uint16_t* ptr = (uint16_t*)(buff + 4);
						cardsToSendToChoser.push_back(ntohs(*ptr));
						if (additionalInfo > 2) {
							len += 2;
							ptr = (uint16_t*)(buff + 6);
							cardsToSendToChoser.push_back(ntohs(*ptr));
						}
						if (!sendLen(choser->first, buff, len));
						;//todo
						if (alreadySended.size() == clients.size() - 1)//all players sended cards
							return states::choseInit;
					}
					default:
						break;
					}
				}
			}
		}
	}
	for (auto& x : clients) {
		if (x.first != choser->first) {
			auto it = std::find(alreadySended.begin(), alreadySended.end(), x.second.id);
			if (it == alreadySended.end()) {//if player is did not send cards 
				addMessagePrefix(buff, 1, codes::playerIsNotResponsing, x.second.id);
				if (!broadCast(x.first, buff, 4))
					;//todo
			}
		}
	}
}
states game::choseinitF() {
	white.putCardsBack(cardsToSendToChoser);//put back cards;
	cardsToSendToChoser.clear();
	return states::chose;
}
states game::choseF() {
	//find choser
	TIMEVAL tv = { 3 * 60, 0 };
	int length = 4;
	// Set up the file descriptor set.
	int count = 0;
	while (count < length) {
		fd_set tmp;
		FD_ZERO(&tmp);
		FD_SET(choser->first, &tmp);
		int iResult = select(0, &tmp, NULL, NULL, &tv);
		if (iResult == SOCKET_ERROR) {
			printf("select failed with error: %d\n", WSAGetLastError());
			//
		}
		else if (iResult == 0) {
			printf("response time for data is is up\n");
			return states::choseOvertime;
		}

		int n = recv(choser->first, buff + count, length, 0);
		if (n == SOCKET_ERROR) {
			printf("receiving data failed with code :%d\n", WSAGetLastError());
			//closesocket(sock);
		}
		else if (!n) {
			printf("client closed connection");
			//closesocket(sock);
		}
		count += n;
		length -= n;
	}
	char coding, playerID;
	int l = getMessagePrefix(buff,coding, playerID);
	if (coding == codes::choserWinnerId) {
		int winner = playerID;
		addMessagePrefix(buff, 1, codes::sendWinner, winner);
		if (!broadCast(0, buff, 4, true))
			;//todo
	}
	else {
		;//todo
	}
	return states::summing;
}
states game::choseOvertimeF()
{
	TIMEVAL tv = { 3 * 60, 0 };
	if (!receiveTime(choser->first, buff, 4, 5, 0))
		;//todo

	char coding, playerID;
	int l = getMessagePrefix(buff, coding, playerID);
	if (coding != codes::choserWinnerId)
		;//todo
	int winner = playerID;
	addMessagePrefix(buff, 1, codes::sendWinner, winner);
	if (!broadCast(0, buff, 4, true))
		;//todo
	return states::summing;
}
states game::summingF()//finalize round
{
	//check all time;
	if (black.empty()) {
		addMessagePrefix(buff, 1, codes::gameIsOver, 0);
		if (!broadCast(0, buff, 4, true))
			;//todo
		return states::waiting;
	}
	return states::questionInit;
}
game::~game()
{
	for (int i = 0; i < fds.fd_count; i++)
	{
		SOCKET socket = fds.fd_array[i];
		closesocket(socket);
	}
}
