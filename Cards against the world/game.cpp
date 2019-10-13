#include"game.h"
#include <intrin.h>
#include <winsock2.h>
#include <uchar.h>
#include <iostream>
#include "inputText.h"
#include "Button.h"
#include "chat.h"
game::game(sf::RenderWindow& win, sf::String lobbyId_, sf::String nick, bool newlobby_) : window(win)
{
	newLobby = newlobby_;
	lobbyId = lobbyId_;
	nickname = nick;
}

message::code game::connect()
{
	std::u32string sendStr;
	sendStr += code + U'\0';
	if (newLobby)
		sendStr += U'y';
	else
		sendStr += U'n';
	std::u32string tmp;
	for (int i = 0; i < nickname.getSize(); i++) {
		tmp.push_back(nickname[i]);
	}
	sendStr += U'\0';
	sendStr += tmp;
	while (sendStr.size() < 54) sendStr.push_back(U'\0');
	tmp.clear();
	for (int i = 0; i < lobbyId.getSize(); i++) {
		tmp.push_back(lobbyId[i]);
	}
	sendStr += tmp + U'\0';
	int j = 0;
	sf::Socket::Status status = entranceSocket.connect(address, portToConnect);
	if (status != sf::Socket::Done)
	{
		return message::unableToRechServer;
	}
	if (!Send(sendStr, entranceSocket))
	{
		return message::unableToSendData;
	}
	// TCP socket:
	char coding = 1;
	char playerID;
	std::u32string rec;
	if (!receive(entranceSocket, rec, coding, playerID))
	{
		return message::unableToGetData;
	}
	if (coding != 0) {
		entranceSocket.disconnect(); // dodaj wszędzie
		if (coding == 2)//error
			return playerID; //error code;
		else
			return message::unKnownError;
	}
	int Lobbyport = atoi(buff + 4);
	std::cout << Lobbyport << std::endl;
	status = lobbySocket.connect(address, Lobbyport);
	if (status != sf::Socket::Done)
	{
		return message::unableToRechServer;
	}
	if (newLobby) {
		if (!receive(lobbySocket, rec, coding, playerID))
		{
			return message::unableToGetData;
		}
	}
	else {
		if (!receive(entranceSocket, rec, coding, playerID))
		{
			return message::unableToGetData;
		}
	}
	if (coding != 0) {
		entranceSocket.disconnect(); // dodaj wszędzie
		if (coding == 2)//error
			return playerID; //error code;
		else
			return message::unKnownError;
	}
	int chatPort = atoi(buff + 4);
	entranceSocket.disconnect();
	status = chatSocket.connect(address, chatPort);
	if (status != sf::Socket::Done)
	{
		return message::unableToRechServer;
	}
	if (!Send(sendStr, chatSocket))
	{
		return message::unableToSendData;
	}

	//get id or start game in main lobby thread
	if (newLobby) {
		playerId = 1;
		Send(U"1", lobbySocket);//synh with serv
	}
	else {
		std::size_t received;
		if (!Send(sendStr, lobbySocket))
		{
			return message::unableToSendData;
		}
		if (lobbySocket.receive(buff, 4, received) != sf::Socket::Done)
		{
			return message::unableToGetData;
		}
		getMessagePrefix(buff, coding, playerID);
		if (coding != 4) {//get id
			lobbySocket.disconnect();
			return message::unableToGetData;
		}
		playerId = playerID;
		//get all info about others players 
		if (lobbySocket.receive(buff, 4, received) != sf::Socket::Done)
		{
			return message::unableToGetData;
		}
		getMessagePrefix(buff, coding, playerID);
		int data = playerID - 1;
		if (coding != 5) {//get id
			lobbySocket.disconnect();
			return message::unableToGetData;
		}
		while (data--) {
			if (receive(lobbySocket, rec, coding, playerID)) {
				players[playerID].nick.clear();
				for (auto x : rec)
					players[playerID].nick += x;
			}
			else { ; }
			//todo
		}
	}
	return message::connected;
}

bool game::Send(std::u32string s, sf::TcpSocket & socket)
{
	int len = s.size() * 4;
	addMessagePrefix(buff, len, 1, playerId);
	socketUtils::code(s, buff + 4);
	if (socket.send(buff, len + 4) != sf::Socket::Done)
	{
		return false;
	}
	return true;
}
int game::getCommand(sf::TcpSocket & socket, char& coding, char& playerId)
{
	std::size_t received;
	if (socket.receive(buff, 4, received) != sf::Socket::Done)
	{
		return 0;
	}
	return getMessagePrefix(buff, coding, playerId);
}
std::u32string game::getString(sf::TcpSocket & socket, int length) {
	std::size_t received;
	int len = length / 4;
	int count = 0;
	while (count < length) {
		if (socket.receive(buff + 4 + count, length, received) != sf::Socket::Done)
		{
			return false;
		}
		count += received;
		length -= received;
	}
	return decode(buff + 4, len);
}
bool game::receive(sf::TcpSocket & socket, std::u32string & data, char& coding, char& playerId)
{
	std::size_t received;
	if (socket.receive(buff, 4, received) != sf::Socket::Done)
	{
		return false;
	}
	int length = getMessagePrefix(buff, coding, playerId);
	int len = length / 4;
	int count = 0;
	while (count < length) {
		if (socket.receive(buff + 4 + count, length, received) != sf::Socket::Done)
		{
			return false;
		}
		count += received;
		length -= received;
	}
	if (coding != 2)
		data = decode(buff + 4, len);
	return true;
}
game::~game()
{
}

void game::test()
{
	sf::Texture backgroundTexture;
	sf::Sprite background;
	sf::Font font;
	sf::Font font2;
	sf::SoundBuffer clickBuff;
	sf::SoundBuffer switchBuff;
	//sf::Sprite background;
	sf::Texture block;
	sf::Texture blockPressed;
	sf::Texture offButton;
	sf::Texture whiteBox;
	sf::Texture checkOff;
	sf::Texture textBox;
	sf::Shader backgroundShader;
	if (!clickBuff.loadFromFile("Sounds/click1.ogg"))
		throw std::exception("Sound file missing");

	if (!switchBuff.loadFromFile("Sounds/switch2.ogg"))
		throw std::exception("Sound file missing");

	if (!font2.loadFromFile("Fonts/NunitoSans-Regular.ttf"))
		throw std::exception("font file missing");

	if (!font.loadFromFile("Fonts/NunitoSans-Bold.ttf"))
		throw std::exception("font file missing");

	if (!block.loadFromFile("PNG/green_button00.png"))
		throw std::exception("png file missing");

	if (!blockPressed.loadFromFile("PNG/green_button01.png"))
		throw std::exception("png file missing");

	if (!offButton.loadFromFile("PNG/green_button04.png"))
		throw std::exception("png file missing");

	if (!textBox.loadFromFile("PNG/grey_button06.png"))
		throw std::exception("png file missing");

	if (!backgroundTexture.loadFromFile("PNG/background.jpg"))
		throw std::exception("png background file missing");
	//back.setSmooth(true);
	if (!whiteBox.loadFromFile("PNG/grey_panel.png"))
		throw std::exception("png file missing");

	background.setTexture(backgroundTexture);
	int linex = 1920 / 2;
	int liney = 1080 / 2;

	Button goBack(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	goBack.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 200) * setting.yScale);
	goBack.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	goBack.setTitle("BACK");
	goBack.setSoundVolume(setting.SoundVolume);
	goBack.setColor(sf::Color::White);

	chat Chat(window, clickBuff, 150, 12, font);
	Chat.setValues(sf::Vector2f(20, 450), 20, 600);
	Chat.send("Welcome to lobby \'" + lobbyId + "\'", sf::Color::Yellow);
	bool allertFlag = false;
	sf::Clock timer;
	chatSocket.setBlocking(false);
	lobbySocket.setBlocking(false);

	std::u32string str;
	char coding = 0, playerID;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		event.type = sf::Event::GainedFocus;
		do {
			goBack.checkState();
			Chat.checkSideBarState();
			if (Chat.function() && event.type == sf::Event::KeyPressed) {
				if (Chat.addChar(event.key)) {
					if (!Send(Chat.getText(), chatSocket))
					{
						;//TODO
					}
				}

			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
				Chat.scrolled(event.mouseWheelScroll.delta);
			}
			else if (goBack.buttonFunction())
				return;
			else;
			std::size_t received;


		} while (window.pollEvent(event));
		if (receive(chatSocket, str, coding, playerID))
		{
			if (coding == 1)
			{
				sf::String out;
				for (auto& x : str)
					out += x;
				if (playerID) {
					out.insert(0, players[playerID].nick + ":");
					Chat.send(out, sf::Color::Black);
				}
				else {
					out.insert(0, "Server:");
					Chat.send(out, sf::Color::Yellow);
				}

			}
		}
		int len = getCommand(lobbySocket, coding, playerID);
		if (len)
		{
			if (coding == 3)
			{
				sf::String out;
				str = getString(lobbySocket, len);
				for (auto& x : str)
					out += x;
				players[playerID].nick = out;
				Chat.send(players[playerID].nick + " joined lobby", sf::Color::Yellow);
			}
			if (coding == 6) {//check playerID
				Chat.send(players[playerID].nick + " disconnected", sf::Color::Yellow);
			}
		}


		/*tutaj funkcja -> sprawdzenie atomica jesli set to
		mutex lock i odczytaj dane w kolejki mutex unlock


		kod wątku pobierania danyc spij na oczekiwaniu danych jesli nadejdą to compute getprefix itd
		lock mutex wbij do kolejki i set flat bool atomic unlock mutex
		*/
		window.clear(sf::Color::Black);
		window.draw(background);
		Chat.draw();
		goBack.draw();
		window.display();
	}
}
