#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "socketUtils.h"
#include"messages.h"
#include "chat.h"
#define LEN 4000


class game :public socketUtils
{
private:
	enum state{lobby, inGame, exit};
	state state_;
	chat Chat;
	std::map<int, sf::String > players;//0 listen rest players up to 8
	unsigned short portToConnect = 3000;
	sf::IpAddress address = "3.229.14.134";
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
	bool ready_ = false;
	bool lockLobby  = false;
	//
	sf::Texture backgroundTexture;
	sf::Sprite background;
	sf::Font font;
	sf::SoundBuffer clickBuff;
	sf::SoundBuffer switchBuff;
	//sf::Sprite background;
	sf::Texture block;
	sf::Texture blockPressed;
	sf::Texture offButton;
	sf::Texture checkOff;

	//
	bool Send(std::u32string s, sf::TcpSocket& socket);
	int getCommand(sf::TcpSocket& socket, char& coding, char& playerId);
	std::u32string getString(sf::TcpSocket& socket, int length);
	bool receive(sf::TcpSocket& socket, std::u32string& data, char& coding, char& playerId);
	game::state joinWait();
	game::state LeaderWait();
	game::state inGame();
	void checkCommands();
public:
	game(sf::RenderWindow& win, sf::SoundBuffer& sndbuff, sf::Font font, sf::String lobbyId, sf::String nick, bool newlobby);
	message::code connect();
	void run();
	~game();
};

