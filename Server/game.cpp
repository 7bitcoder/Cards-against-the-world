#include "game.h"
#include <cstdlib>
#include <ctime>
#include <random>
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
	const char notEnoughPlayers = 28;//min 4 players required
	const char getNewWhiteCards = 29;//get new white cards from server;
	const char lobbyError = 30;//lobby has crushed;
	const char getAllchosenCards = 31;//get all chosen cards send vector to players
	const char setDoubleMode = 32;//in len 1 if no double mode 2 if doublemode
	const char startNext = 33;//go to next state choser/normal
	const char check = 34;//send check if player sended card
	const char gotAllCards = 35;

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
	clients[leader].sock = leader;
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

			if (!sendLen(it->first, buff, len)) {
				toDelete.push_back(it);
				printf("broadcast in fail\n");
			}
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
	clients[client].sock = client;
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
						printf("got ready\n");
						clients[sock].ready = true;
						broadCast(sock, buff, 4, true);//true to send all
						break;
					case codes::notReady://notReady
						printf("got not ready\n");
						clients[sock].ready = false;
						broadCast(sock, buff, 4, true);
						break;
					case codes::start://start game
					{
						printf("got not start\n");
						if (clients.size() < 3) {
							addMessagePrefix(buff, 1, codes::notEnoughPlayers, 0);
							if (!sendLen(leader, buff, 4));
							//TODO
							break;
						}
						bool br = false;
						for (auto it = clients.begin(); it != clients.end(); it++)
							if (it->first != leader)
								if (!it->second.ready) {
									addMessagePrefix(buff, 1, codes::notAllPlayersAreReady, 0);
									if (!sendLen(leader, buff, 4))
										;//todo
									br = true;
									break;
								}
						if (br)
							break;
						lock = true;
						addMessagePrefix(buff, 1, codes::start, 0);
						if (!broadCast(0, buff, 4, true)) {
							;//todo
						}
						printf("starting\n");
						return states::starting;
					}
					break;
					case codes::sendWhiteDequeLen:
						printf("got white Deque len\n");
						white.init(additionalInfo);
						break;
					case codes::timeUpdate:
						GameTime = additionalInfo;
						if (!broadCast(0, buff, 4, true)) {
							;//todo
						}
						printf("got time &d\n", GameTime);
						break;
					case codes::sendBlackDequeLen:
						printf("got black Deque len\n");
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
	begin = std::chrono::high_resolution_clock::now();
	//deques shuffle;
	white.shuffle();
	black.shuffle();
	printf("shufflin\n");
	//init inGame vector
	inGamePLayers.clear();
	for (auto& x : clients) {
		inGamePLayers.push_back(x.second);
	}
	std::sort(inGamePLayers.begin(), inGamePLayers.end(), [](slot & x, slot & y) {return x.id < y.id; });
	printf("init game vec\n");
	//send to all players 10 init cards
	addMessagePrefix(buff, 1, codes::sendRandomTenCards, 0);
	for (auto& player : clients) {
		for (int i = 0; i < 10; i++)
			codeCard(buff + 4 + i * 2, white.getCard());//add to raw data units 16 *10 cards = cards id
		if (!sendLen(player.first, buff, 4 + 10 * 2))
			disconnect(player.first);
	}
	printf("send 10 cards to palyers\n");
	//init random choser
	choser = std::rand() % inGamePLayers.size();
	printf("rand choser\n");
	return states::questionInit;
}
states game::questionInitF() {

	//send black card
	addMessagePrefix(buff, 1, codes::sendBlackCard, 0);
	codeCard(buff + 4, black.getCard());
	broadCast(0, buff, 6, true);
	printf("send black\n");
	//get new choser
	choser++;
	if (choser >= inGamePLayers.size())
		choser = 0;

	//send info choser
	addMessagePrefix(buff, 1, codes::sendChoserId, inGamePLayers.at(choser).id);
	broadCast(0, buff, 4, true);
	printf("send choser\n");
	return states::question;
}
states game::questionF() {
	allCardsToSend.clear();
	printf("start question\n");
	playersSended.clear();
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
					case codes::setDoubleMode:
						doubleMode = additionalInfo == 2;
						addMessagePrefix(buff, 1, codes::startNext, 0);
						if (!broadCast(0, buff, 4, true))
							;//todo
						break;
					case codes::sendChosenWhiteCards://get cards to send to choser
					{
						printf("got cards from players\n");
						bool testDouble = additionalInfo > 2;
						if (testDouble != doubleMode) {
							;//todo
						}
						playersSended.push_back(sock);
						int get = doubleMode ? 4 : 2;
						i = recv(sock, buff, get, 0);
						if (i != get) {
							disconnect(sock);
						}
						int len = 6;
						uint16_t newFirst, newSec;
						newFirst = decodeCard(buff);
						white.putCardBack(newFirst);
						allCardsToSend.push_back({ newFirst,playerID });
						newFirst = white.getCard();
						if (doubleMode) {
							newSec = decodeCard(buff + 2);
							white.putCardBack(newSec);
							allCardsToSend.push_back({ newSec,playerID });
							newSec = white.getCard();
							len += 2;
						}
						//send new card/s
						codeCard(buff + 4, newFirst);
						if (doubleMode)
							codeCard(buff + 6, newSec);
						addMessagePrefix(buff, len, codes::getNewWhiteCards, 0);
						if (!sendLen(sock, buff, len))
							disconnect(sock);
						addMessagePrefix(buff, 1, codes::check, playerID);
						if (!broadCast(0, buff, 4, true))
							;//todo
						printf("got %d cards\n", playersSended.size());
						if (playersSended.size() == clients.size() - 1) {//all players sended cards
							printf("sending all cards\n", playersSended.size());
							addMessagePrefix(buff, allCardsToSend.size(), codes::gotAllCards, 0);
							codeAllCards(buff + 4, allCardsToSend);
							if (!broadCast(0, buff, 4 + allCardsToSend.size() * 3, true)) {
								printf("broadcast fail\n");
							}
							return states::choseInit;
						}
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
states game::choseinitF() {
	printf("chose init\n", playersSended.size());
	playersSended.clear();
	return states::chose;
}
states game::choseF() {
	//find choser
	int length = 4;
	// Set up the file descriptor set.
	int count = 0;
	while (count < length) {
		fd_set tmp;
		FD_ZERO(&tmp);
		FD_SET(inGamePLayers.at(choser).sock, &tmp);
		int iResult = select(0, &tmp, nullptr, nullptr, nullptr);
		if (iResult == SOCKET_ERROR) {
			printf("select failed with error: %d\n", WSAGetLastError());
			//
		}
		int n = recv(inGamePLayers.at(choser).sock, buff + count, length, 0);
		if (n == SOCKET_ERROR) {
			printf("receiving data failed with code :%d\n", WSAGetLastError());
			disconnect(inGamePLayers.at(choser).sock);
		}
		else if (!n) {
			printf("client closed connection");
			disconnect(inGamePLayers.at(choser).sock);
		}
		count += n;
		length -= n;
	}
	char coding, playerID;
	int l = getMessagePrefix(buff, coding, playerID);
	if (coding == codes::choserWinnerId) {
		printf("got %d winner\n", int(playerID));
		addMessagePrefix(buff, 1, codes::sendWinner, playerID);
		if (!broadCast(0, buff, 4, true))
			;//todo
	}
	else {
		;//todo
	}
	return states::summing;
}
states game::summingF()//finalize round
{
	printf("summing\n");
	//check all time;
	auto end = std::chrono::high_resolution_clock::now();
	auto durr = std::chrono::duration_cast<std::chrono::minutes>(begin - end);
	int x = durr.count();
	if (durr.count() > GameTime || black.empty()) {
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
