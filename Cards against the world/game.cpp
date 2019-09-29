#include"game.h"
#include <intrin.h>
#include <winsock2.h>
#include <uchar.h>

game::game(sf::RenderWindow& win, sf::String lobbyId_, sf::String nick): window(win)
{
	lobbyId = lobbyId_;
	nickname = nick;
}

ConnectErrors game::connect()
{
	std::u32string sendStr(100, '\0');
	sendStr.insert(0, code);
	sendStr.insert(21, U"y");
	std::u32string tmp;
	for (int i = 0; i < nickname.getSize(); i++) {
		tmp.push_back(nickname[i]);
	}
	sendStr.insert(23, tmp);
	tmp.clear();
	for (int i = 0; i < lobbyId.getSize(); i++) {
		tmp.push_back(lobbyId[i]);
	}
	sendStr.insert(30, tmp);
	int j = 0;
	sf::Socket::Status status = socket.connect(address, portToConnect);
	if (status != sf::Socket::Done)
	{
		return ConnectErrors::unableToRechServer;
	}
	if (!Send(sendStr))
	{
		return ConnectErrors::unableToSendData;
	}
}

bool game::Send(std::u32string s)
{
	std::string sendStr(500, '\0');;
	mbstate_t p{};
	size_t length;
	int j = 0;
	char data[5] = { 0 };
	for (int i = 0; i < s.size(); i++) {
		// initializing the function 
		length = c32rtomb(data, s[i], &p);
		sendStr.insert(i * 4, data);
	}
	if (socket.send(sendStr.c_str(), 500) != sf::Socket::Done)
	{
		return false;
	}
	return false;
}

game::~game()
{
}
