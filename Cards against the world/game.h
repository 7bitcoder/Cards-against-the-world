#pragma once
#include <SFML/Graphics.hpp>#include <SFML/Network.hpp>
enum ConnectErrors { lobbyIsAlredyChosen, noPortsAreAvailable, lobbyNameIsIncorrect, couldNotFindLobby, ServerIsFull, lobbyNameIsLocked, connected };
class game
{
private:
	sf::RenderWindow& window;
	sf::String lobbyId;
	sf::String nickname;
	//sf::TcpSocket socket;
	std::string code = "a7dzRwQjnw5kW6uEnhx7";
public:
	game(sf::RenderWindow& win, sf::String lobbyId, sf::String nick);
	ConnectErrors connect();
	~game();
};

