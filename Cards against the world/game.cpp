#include"game.h"
#include <intrin.h>
#include <winsock2.h>
#include <uchar.h>
#include <iostream>
game::game(sf::RenderWindow& win, sf::String lobbyId_, sf::String nick, bool newlobby_): window(win)
{
	newLobby = newlobby_;
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
	sf::Socket::Status status = entranceSocket.connect(address, portToConnect);
	if (status != sf::Socket::Done)
	{
		return ConnectErrors::unableToRechServer;
	}
	if (!Send(sendStr, entranceSocket))
	{
		return ConnectErrors::unableToSendData;
	}
	std::size_t received;
	// TCP socket:
	if (entranceSocket.receive(buff, LEN, received) != sf::Socket::Done)
	{
		return ConnectErrors::unableToGetData;
	}
	int Lobbyport = atoi(buff);
	std::cout << Lobbyport << std::endl;
	status = lobbySocket.connect(address, Lobbyport);
	if (status != sf::Socket::Done)
	{
		return ConnectErrors::unableToRechServer;
	}
	if (newLobby) {
		if (lobbySocket.receive(buff, LEN, received) != sf::Socket::Done)
		{
			return ConnectErrors::unableToGetData;
		}
	}
	else {
		if (entranceSocket.receive(buff, LEN, received) != sf::Socket::Done)
		{
			return ConnectErrors::unableToGetData;
		}
	}
	int chatPort = atoi(buff);
	entranceSocket.disconnect();
	status = chatSocket.connect(address, chatPort);
	if (status != sf::Socket::Done)
	{
		return ConnectErrors::unableToRechServer;
	}
	if (!Send(sendStr, chatSocket))
	{
		return ConnectErrors::unableToSendData;
	}
	std::cout << "done\n";
}

bool game::Send(std::u32string s, sf::TcpSocket & socket)
{
	socketUtils::code(s, buff);
	if (socket.send(buff, s.size() + 1) != sf::Socket::Done)
	{
		return false;
	}
	return true;
}

game::~game()
{
}
