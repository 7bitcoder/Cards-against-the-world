#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "socketUtils.h"
#define LEN 4000
enum ConnectErrors {unableToRechServer,unableToSendData, unableToGetData, lobbyIsAlredyChosen, noPortsAreAvailable, lobbyNameIsIncorrect, couldNotFindLobby, ServerIsFull, lobbyNameIsLocked, connected };
class game:public socketUtils
{
private:
	unsigned short portToConnect = 3000;
	sf::IpAddress address = "127.0.0.1";
	std::u32string code = U"a7dzRwQjnw5kW6uEnhx7";
	sf::RenderWindow& window;
	sf::String lobbyId;
	sf::String nickname;
	sf::TcpSocket entranceSocket;
	sf::TcpSocket lobbySocket;
	sf::TcpSocket chatSocket;
	char buff[LEN];
	bool newLobby;
public:
	game(sf::RenderWindow& win, sf::String lobbyId, sf::String nick, bool newlobby);
	ConnectErrors connect();
	bool Send(std::u32string s, sf::TcpSocket& socket, bool addSize = false);
	std::u32string Receive(sf::TcpSocket& socket);
	~game();
	void test();
};

