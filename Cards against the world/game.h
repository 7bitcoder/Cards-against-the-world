#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "socketUtils.h"
#define LEN 4000


namespace message {
	const char unKnownError = 0;
	const char lobbyIsAlredyChosen = 1;
	const char noPortsAreAvailable = 2
	const char lobbyNameIsIncorrect = 3;
	const char couldNotFindLobby = 4;
	const char ServerIsFull = 5;
	const char lobbyNameIsLocked = 6;//server end
	const char unableToRechServer = 7;
	const char unableToSendData = 8;
	const char unableToGetData = 9;
	const char connected = 10;
	namespace {//private
	const std::vector<std::string> connectionError { 
		"Unknown error", 
		"Lobby is already chosen" ,
		"No ports are available on server",
		"Lobby name is incorrect",
		"Cund not find lobby",
		"Server is full",
		"Lobby name is locked"  };//7
	}
	std::string getConnectError(char error) {
	if(error < 1 || error >= errorVector.size())
	return errorVector[0];
	return errorVector[error];
	}
}

class game :public socketUtils
{
private:
	std::map<char, sf::String> players;
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
	ConnectErrors connect();
	bool Send(std::u32string s, sf::TcpSocket& socket);
	bool receive(sf::TcpSocket& socket, std::u32string& data, char& coding, char& playerId);
	~game();
	void test();
};

