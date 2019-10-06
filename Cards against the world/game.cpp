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

ConnectErrors game::connect()
{
	std::u32string sendStr(100, '\0');
	sendStr.insert(0, code);
	if (newLobby)
		sendStr.insert(21, U"y");
	else
		sendStr.insert(21, U"n");
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
	// TCP socket:
	char coding = 1;
	char playerID;
	std::u32string rec;
	if (!receive(entranceSocket, rec, coding, playerID))
	{
		return ConnectErrors::unableToGetData;
	}
	if (coding != 0)
		int j = 2; //TODO return b³¹d polaczenia z serverem 
	int Lobbyport = atoi(buff + 4);
	std::cout << Lobbyport << std::endl;
	status = lobbySocket.connect(address, Lobbyport);
	if (status != sf::Socket::Done)
	{
		return ConnectErrors::unableToRechServer;
	}
	if (newLobby) {
		if (!receive(lobbySocket, rec, coding, playerID))
		{
			return ConnectErrors::unableToGetData;
		}
	}
	else {
		if (!receive(entranceSocket, rec, coding, playerID))
		{
			return ConnectErrors::unableToGetData;
		}
	}
	if (coding != 0)
		int j = 2; //TODO return b³¹d polaczenia z serverem 
	int chatPort = atoi(buff + 4);
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
	if (!receive(chatSocket, rec, coding, playerID))
	{
		return ConnectErrors::unableToGetData;
	}
	std::cout << "done\n";
}

bool game::Send(std::u32string s, sf::TcpSocket& socket)
{
	int len = s.size() * 4;
	addMessagePrefix(buff, len, 1, playerId);
	socketUtils::code(s, buff + 4);
	if (socket.send(buff, len + 5) != sf::Socket::Done)
	{
		return false;
	}
	return true;
}
bool game::receive(sf::TcpSocket& socket, std::u32string& data, char& coding, char& playerId)
{
	std::size_t received;
	if (socket.receive(buff, 4, received) != sf::Socket::Done)
	{
		return false;
	}
	int length = getMessagePrefix(buff, coding, playerId);
	int count = 0;
	while (count < length) {
		if (socket.receive(buff + 4 + count, length, received) != sf::Socket::Done)
		{
			return false;
		}
		count += received;
		length -= received;
	}
	if (coding == 1)
		data = decode(buff + 4, length);
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
	bool allertFlag = false;
	sf::Clock timer;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		event.type = sf::Event::GainedFocus;
		do {
			chatSocket.setBlocking(false);
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
			std::u32string str;
			char coding = 0, playerID;
			if (!receive(chatSocket, str, coding, playerID))
			{
				;//todo
			}
			if (coding == 1)
			{
				sf::String out;
				for (auto& x : str)
					out += x;
				Chat << out;
			}
		} while (window.pollEvent(event));

		window.clear(sf::Color::Black);
		window.draw(background);
		Chat.draw();
		goBack.draw();
		window.display();
	}
}
