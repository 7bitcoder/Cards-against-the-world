#include "game.h"

game::game(sf::RenderWindow& win, sf::String lobbyId_, sf::String nick): window(win)
{
	lobbyId = lobbyId_;
	nickname = nick;
}

ConnectErrors game::connect()
{
	return ConnectErrors();
}

game::~game()
{
}
