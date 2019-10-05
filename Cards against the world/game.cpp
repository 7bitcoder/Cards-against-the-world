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
	if (!Send(sendStr, entranceSocket, false))
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

bool game::Send(std::u32string s, sf::TcpSocket& socket, bool addSize)
{
	int offset = 0;
	if (addSize) {
		int len = s.size();
		addMessageLen(buff, len);

		offset = 2;
	}
	socketUtils::code(s, buff + offset);
	if (socket.send(buff, s.size() * 4 + ++offset) != sf::Socket::Done)
	{
		return false;
	}
	return true;
}
std::u32string game::Receive(sf::TcpSocket& socket)
{
	std::size_t received;
	if (socket.receive(buff, 2, received) != sf::Socket::Done)
	{
		return std::u32string();
	}
	int length = getMessageLen(buff) * 4;
	int count = 0;
	while (count < length) {
		if (socket.receive(buff + count, length, received) != sf::Socket::Done)
		{
			return std::u32string();
		}
		count += received;
		length -= received;
	}
	return decode(buff, length);
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
	sf::Texture base;
	//sf::Sprite background;
	sf::Texture mar;
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

	if (!mar.loadFromFile("PNG/grey_sliderRight.png"))
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

	chat Chat(window, clickBuff, 150, 20, font);
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
				Chat.addChar(event.key);
			}
			else if (false) {
				std::u32string tmp;
				/*auto basic = lobbyId.getText();
				for (int i = 0; i < basic.getSize(); i++) {
					tmp.push_back(basic[i]);
				}
				Send(tmp, chatSocket, true);*/
			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
				Chat.scrolled(event.mouseWheelScroll.delta);
			}
			else if (goBack.buttonFunction())
				return;
			else;
			std::size_t received;
			std::u32string str = Receive(chatSocket);
			/*if (str != std::u32string())
			{
				sf::String out;
				for (auto& x : str)
					out += x;
				nickname.setString(out);
			}*/
		} while (window.pollEvent(event));

		window.clear(sf::Color::Black);
		window.draw(background);
		Chat.draw();
		goBack.draw();
		window.display();
	}
}
