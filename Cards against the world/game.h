#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
enum ConnectErrors {unableToRechServer,unableToSendData, lobbyIsAlredyChosen, noPortsAreAvailable, lobbyNameIsIncorrect, couldNotFindLobby, ServerIsFull, lobbyNameIsLocked, connected };
class game
{
private:
	unsigned short portToConnect = 3000;
	sf::IpAddress address = "127.0.0.1";
	std::u32string code = U"a7dzRwQjnw5kW6uEnhx7";
	sf::RenderWindow& window;
	sf::String lobbyId;
	sf::String nickname;
	sf::TcpSocket socket;
	char sendbuff[4000];
public:
	game(sf::RenderWindow& win, sf::String lobbyId, sf::String nick);
	ConnectErrors connect();
	bool Send(std::u32string s);
	~game();
};

