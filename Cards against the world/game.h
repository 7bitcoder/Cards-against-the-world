#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "socketUtils.h"
#include"messages.h"
#define LEN 4000


struct player {
	sf::String nick;
};
class game :public socketUtils
{
private:
	std::map<int, player> players;//0 listen rest players up to 8
	unsigned short portToConnect = 3000;
	sf::IpAddress address = "127.0.0.1";//"3.229.14.134";
	std::u32string code = U"a7dzRwQjnw5kW6uEnhx7";
	sf::RenderWindow& window;
	sf::String lobbyId;
	sf::String nickname;
	sf::TcpSocket entranceSocket;
	sf::TcpSocket lobbySocket;
	sf::TcpSocket chatSocket;
	char buff[LEN];
	bool newLobby;
	char playerId;
public:
	game(sf::RenderWindow& win, sf::String lobbyId, sf::String nick, bool newlobby);
	message::code connect();
	bool Send(std::u32string s, sf::TcpSocket& socket);
	int getCommand(sf::TcpSocket& socket, char& coding, char& playerId);
	std::u32string getString(sf::TcpSocket& socket, int length);
	std::u32string getString(sf::TcpSocket& socket, char* buff, int length);
	bool receive(sf::TcpSocket& socket, std::u32string& data, char& coding, char& playerId);
	~game();
	void test();
};

