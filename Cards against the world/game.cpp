#include"game.h"
#include <intrin.h>
#include <winsock2.h>
#include <uchar.h>
#include <iostream>
#include "inputText.h"
#include "Button.h"
#include "Rounds.h"
#include"lobbyPlayers.h"


namespace codes {
	/*protokól komunikacji
	1 bajt  CODE
	0 - ZWYKŁE dane czyli raw data z sizem podanym w Size
	1 - UNICODE message size w bajtach podany w Size
	2 - ERROR kod erroru podany w playerId
	KOmendy :
	z kodem podanym  w playerId oraz ew danymi w Size
	2 bajtach SIZE
	short długość wiadomości zawsze podana w bajtach lub dodatkowe dane
	1 bajt PLAYERID
	 id gracza wysyłającego jeśli server to 0 , 1 to broadcast reszta to id gracza lub dodatkowe dane
	4+ bajty wiadomość
	*/
	const char raw = 0;//raw  data
	const char unicode = 1;//unicode string 
	const char error = 2;//error
	const char newPlayer = 3;// for others get id newplayer
	const char getId = 4;// get new player id;
	const char getLobbyInfo = 5;// get lobby nicks itp
	const char disconnect = 6;// disconnect player of this id 
	const char Ready = 7;// - Ready wcisniecie ready i wyslanie do servera i wszyatkich  zmiana na zielony
	const char notReady = 8;// - notReady to samo pu tylko not ready zmiana na czerowny
	const char start = 9;// - start leader jeśli wszyscy ready to zmiana stanu na grę LEADER
	const char dequeUpdate = 10;// - update talii //aktualnie jeszcze nie LEADER
	const char timeUpdate = 11;// - update czas gry LEADER
	const char notAllPlayersAreReady = 12; // not all pleyrs arre ready
	const char sendWhiteDequeLen = 13;//send white deque len;
	const char sendBlackDequeLen = 14;
	const char sendRandomTenCards = 15; //send init cards to players
	const char sendChoserId = 16;//send info to all plaers that id of player is id of choser
	const char sendBlackCard = 17;//send black card to players
	const char sendChosenWhiteCards = 18;//send one or 2 chosen cards from normal players to choser
	const char randomWhiteCardsRequest = 19;//when player time is up sent it to him
	const char getRandomWhiteCards = 20;//get random cards from request ^
	const char choserWinnerId = 21;//server get from choser winner
	const char randomWinnerRequest = 22;//if time is up request for random one
	const char getRandomWinner = 23;//response ^
	const char sendWinner = 24;//send to all player winner to update points;
	const char gameIsOver = 25;//time is up or rounds
	const char shuffleWhiteDeque = 26;//shuffle deque LEADER concept if cards are reapiting then shuffle it
	const char playerIsNotResponsing = 27;//if pleyer is not responsing send it to rest and stop game
	const char notEnoughPlayers = 28;//min 4 players required
	const char getNewWhiteCards = 29;//get new white cards from server;

}//TODO naprawnie deadlocka broadcast disconnect
game::game(sf::RenderWindow& win, sf::SoundBuffer& sndbuff, sf::Font font_, sf::String lobbyId_, sf::String nick, bool newlobby_) : window(win), Chat(win, sndbuff, 150, 12, font)
, font(font_), clickBuff(sndbuff), clock(font), normalTable(win), chosingTabl(win), score(win), black(card::kind::black)
{
	newLobby = newlobby_;
	lobbyId = lobbyId_;
	nickname = nick;
	state_ = state::lobby;
	Chat.setValues(sf::Vector2f((1920 - 650), 450), 20, 600);
	Chat.send("Welcome to lobby \'" + lobbyId + "\'", sf::Color::Yellow);
	if (!backgroundTexture.loadFromFile("PNG/background.jpg"))
		throw std::exception("png background file missing");
	background.setTexture(backgroundTexture);
	if (!block.loadFromFile("PNG/green_button00.png"))
		throw std::exception("png file missing");

	if (!blockPressed.loadFromFile("PNG/green_button01.png"))
		throw std::exception("png file missing");

	if (!offButton.loadFromFile("PNG/green_button04.png"))
		throw std::exception("png file missing");

	if (!switchBuff.loadFromFile("Sounds/switch2.ogg"))
		throw std::exception("Sound file missing");

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
		players[playerId] = nickname;
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
				players[playerID].clear();
				for (auto x : rec)
					players[playerID] += x;
			}
			else { ; }
			//todo
		}
	}
	return message::connected;
}

void game::run()
{
	switch (state_) {
	case state::lobby:
		state_ = newLobby ? LeaderWait() : joinWait();
		break;
	case state::init:
		state_ = initF();
		break;
	case state::newRound:
		state_ = newRoundF();
		break;
	case state::choser:
		state_ = choserF();
		break;
	case state::normal:
		state_ = normalF();
		break;
	case state::exit:
		return;
	default:
		break;
	}
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
	lobbySocket.disconnect();
	chatSocket.disconnect();
	entranceSocket.disconnect();
}
game::state game::joinWait() {
	sf::Texture roundsOn, VolumePointner, VolumeSliderLine;

	if (!roundsOn.loadFromFile("PNG/rounds_on_button11.png"))
		throw std::exception("png file missing");

	if (!VolumePointner.loadFromFile("PNG/grey_sliderUp.png"))
		throw std::exception("png file missing");

	if (!VolumeSliderLine.loadFromFile("PNG/grey_sliderHorizontal.png"))
		throw std::exception("png file missing");
	//back.setSmooth(true);

	background.setTexture(backgroundTexture);
	int linex = 1920 / 2;
	int liney = 1080 / 2;

	Button ready(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	ready.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 100) * setting.yScale);
	ready.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	ready.setTitle("READY");
	ready.setSoundVolume(setting.SoundVolume);
	ready.setColor(sf::Color::White);

	Button goBack(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	goBack.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 200) * setting.yScale);
	goBack.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	goBack.setTitle("BACK");
	goBack.setSoundVolume(setting.SoundVolume);
	goBack.setColor(sf::Color::White);

	std::experimental::filesystem::path deck_ = "taliaRocka.txt";
	if (!deck.load(deck_))
		Chat.send("Could not load default deck: " + deck_.filename().string() + " validate txt file", sf::Color::Yellow);
	else
		Chat.send("Default Deck: " + deck_.filename().string() + " loaded", sf::Color::Yellow);

	lobbyPlayers lobbyClients(window, font, 30);
	lobbyClients.setPosition(50 * setting.xScale, (50) * setting.yScale);
	for (auto& x : players) {
		lobbyClients.addPlayer(x.first, x.second);
	}
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
			ready.checkState();
			Chat.checkSideBarState();
			if (Chat.function() && event.type == sf::Event::KeyPressed) {
				if (Chat.addChar(event.key)) {
					auto text = Chat.getText();
					if (!text.empty())
						if (!Send(text, chatSocket))
						{
							;//TODO
						}
				}

			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
				Chat.scrolled(event.mouseWheelScroll.delta);
			}
			else if (goBack.buttonFunction())
				return game::state::exit;
			else if (ready.buttonFunction()) {
				if (ready_) {
					addMessagePrefix(buff, 1, codes::notReady, playerId);
					ready_ = false;
					ready.setTitle("READY");
				}
				else {
					addMessagePrefix(buff, 1, codes::Ready, playerId);
					ready_ = true;
					ready.setTitle("NOT READY");
				}
				if (lobbySocket.send(buff, 4) != sf::Socket::Done)//TODO
				{
					//TODO
				}
			}
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
					out.insert(0, players[playerID] + ":");
					Chat.send(out, sf::Color::Black);
				}
				else {
					out.insert(0, "Server:");
					Chat.send(out, sf::Color::Yellow);
				}

			}
		}
		int len = getCommand(lobbySocket, coding, playerID);
		if (len)//check commands
		{
			sf::String out;
			switch (coding) {
			case codes::newPlayer:
				str = getString(lobbySocket, len);
				for (auto& x : str)
					out += x;
				players[playerID] = out;
				Chat.send(players[playerID] + " joined lobby", sf::Color::Yellow);
				lobbyClients.addPlayer(playerID, out);
				break;
			case codes::disconnect: //check playerID
				Chat.send(players[playerID] + " disconnected", sf::Color::Yellow);
				lobbyClients.del(playerID);
				break;
			case codes::Ready://ready
				lobbyClients.setReady(playerID);
				break;
			case codes::notReady://not ready
				lobbyClients.setNotReady(playerID);
				break;
			case codes::start:
				Chat.send("Game started", sf::Color::Yellow);
				return state::init;
				break;
			default:
				break;
			}
		}

		window.clear(sf::Color::Black);
		window.draw(background);
		Chat.draw();
		goBack.draw();
		ready.draw();
		lobbyClients.draw();
		window.display();
	}
}
game::state game::LeaderWait()
{
	sf::Texture roundsOn, VolumePointner, VolumeSliderLine;

	if (!roundsOn.loadFromFile("PNG/rounds_on_button11.png"))
		throw std::exception("png file missing");

	if (!VolumePointner.loadFromFile("PNG/grey_sliderUp.png"))
		throw std::exception("png file missing");

	if (!VolumeSliderLine.loadFromFile("PNG/grey_sliderHorizontal.png"))
		throw std::exception("png file missing");
	//back.setSmooth(true);

	background.setTexture(backgroundTexture);
	int linex = 1920 / 2;
	int liney = 1080 / 2;

	Button start(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	start.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 100) * setting.yScale);
	start.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	start.setTitle("START");
	start.setSoundVolume(setting.SoundVolume);
	start.setColor(sf::Color::White);

	Button apply(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	apply.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney)* setting.yScale);
	apply.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	apply.setTitle("APPLY");
	apply.setSoundVolume(setting.SoundVolume);
	apply.setColor(sf::Color::White);

	Button goBack(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	goBack.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 200) * setting.yScale);
	goBack.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	goBack.setTitle("BACK");
	goBack.setSoundVolume(setting.SoundVolume);
	goBack.setColor(sf::Color::White);

	Rounds rounds(window, roundsOn, VolumePointner, VolumeSliderLine, clickBuff);
	rounds.setFont(font);
	rounds.setPosition((linex - 50) * setting.xScale, 300 * setting.yScale);
	rounds.setScale(setting.xScale, setting.yScale);
	rounds.setSliderPosition(linex - 150, (500 - 30));
	rounds.setSoundVolume(setting.SoundVolume);

	lobbyPlayers lobbyClients(window, font, 30);
	lobbyClients.setPosition(50 * setting.xScale, (50) * setting.yScale);
	lobbyClients.addPlayer(playerId, nickname);
	for (auto& x : players) {
		lobbyClients.addPlayer(x.first, x.second);
	}

	bool allertFlag = false;
	sf::Clock timer;
	chatSocket.setBlocking(false);
	lobbySocket.setBlocking(false);

	std::experimental::filesystem::path deck_ = "taliaRocka.txt";
	if (!deck.load(deck_))
		Chat.send("Could not load default deck: " + deck_.filename().string() + " validate txt file", sf::Color::Yellow);
	else
		Chat.send("Default Deck: " + deck_.filename().string() + " loaded", sf::Color::Yellow);
	//todo
	std::u32string str;
	char coding = 0, playerID;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		event.type = sf::Event::GainedFocus;
		do {
			rounds.checkState();
			goBack.checkState();
			apply.checkState();
			start.checkState();
			Chat.checkSideBarState();
			if (Chat.function() && event.type == sf::Event::KeyPressed) {
				if (Chat.addChar(event.key)) {
					auto text = Chat.getText();
					if (!text.empty())
						if (!Send(text, chatSocket))
						{
							;//TODO
						}
				}

			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
				Chat.scrolled(event.mouseWheelScroll.delta);
			}
			else if (rounds.function());
			else if (goBack.buttonFunction())
				return game::state::exit;
			else if (apply.buttonFunction()) {

			}
			else if (start.buttonFunction()) {
				addMessagePrefix(buff, static_cast<uint16_t>(deck.getBlackDeckSize()), codes::sendBlackDequeLen, playerId);
				if (lobbySocket.send(buff, 4) != sf::Socket::Done)//TODO
				{
					//TODO
				}
				addMessagePrefix(buff, static_cast<uint16_t>(deck.getWhiteDeckSize()), codes::sendWhiteDequeLen, playerId);
				if (lobbySocket.send(buff, 4) != sf::Socket::Done)//TODO
				{
					//TODO
				}
				addMessagePrefix(buff, 1, codes::start, playerId);
				if (lobbySocket.send(buff, 4) != sf::Socket::Done)//TODO
				{
					//TODO
				}
			}
		} while (window.pollEvent(event));
		if (receive(chatSocket, str, coding, playerID))
		{
			if (coding == 1)
			{
				sf::String out;
				for (auto& x : str)
					out += x;
				if (playerID) {
					out.insert(0, players[playerID] + ":");
					Chat.send(out, sf::Color::Black);
				}
				else {
					out.insert(0, "Server:");
					Chat.send(out, sf::Color::Yellow);
				}

			}
		}
		int len = getCommand(lobbySocket, coding, playerID);
		if (len)//check commands
		{
			sf::String out;
			switch (coding) {
			case codes::newPlayer:
				str = getString(lobbySocket, len);
				for (auto& x : str)
					out += x;
				players[playerID] = out;
				Chat.send(players[playerID] + " joined lobby", sf::Color::Yellow);
				lobbyClients.addPlayer(playerID, out);
				break;
			case codes::disconnect: //check playerID
				Chat.send(players[playerID] + " disconnected", sf::Color::Yellow);
				lobbyClients.del(playerID);
				break;
			case codes::Ready://ready
				lobbyClients.setReady(playerID);
				break;
			case codes::notReady://not ready
				lobbyClients.setNotReady(playerID);
				break;
			case codes::notAllPlayersAreReady:
				Chat.send("Not all players are ready", sf::Color::Yellow);
				break;
			case codes::start:
				Chat.send("Game started", sf::Color::Yellow);
				return state::init;
				break;
			case codes::notEnoughPlayers:
				Chat.send("Minimum four players required to start the game", sf::Color::Yellow);
				break;
			default:
				break;
			}
		}
		window.clear(sf::Color::Black);
		window.draw(background);
		Chat.draw();
		goBack.draw();
		apply.draw();
		rounds.draw();
		start.draw();
		lobbyClients.draw();
		window.display();
	}
}
game::state game::initF()
{
	normalTable.init(10);
	normalTable.hideF();
	normalTable.resetChosen();

	chosingTabl.init(players.size() - 1);
	chosingTabl.hideF();
	chosingTabl.resetChosen();

	Chat.setValues(sf::Vector2f((1920 - 650), 50), 20, 600);

	score.init(25, 200, players.size());
	score.setColor(sf::Color::White);
	score.setPosition(50, 50, players, font);//set Pos and add players

	black.setOffest(20);
	black.setPosition(1920 - 400, 500);
	black.setCharSize(30);
	black.setId(0);
	black.setTextUtf8("");

	int linex = 1920;

	sf::Texture tmp;//temporary 
	tmp.loadFromFile("PNG/tmp.png");

	clock.setTexture(tmp);
	clock.setTitle("Time:");
	clock.setPosition(linex / 2 - 150, 10);
	clock.setTimer(3, 0);
	clock.stop();
	clock.setSize(60);
	clock.setDeadline(0, 20);

	Chat.send("Waiting for server", sf::Color::Yellow);
	return state::newRound;
}

game::state game::newRoundF()
{
	int linex = 1920;
	int liney = 1080;

	Button confirm(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	confirm.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 200) * setting.yScale);
	confirm.setScale(setting.xScale, 1 * setting.yScale);
	confirm.setTitle("CONFIRM");
	confirm.setSoundVolume(setting.SoundVolume);
	confirm.setColor(sf::Color::White);

	Button quit(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	quit.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 100) * setting.yScale);
	quit.setScale(setting.xScale, 1 * setting.yScale);
	quit.setTitle("EXIT");
	quit.setSoundVolume(setting.SoundVolume);
	quit.setColor(sf::Color::White);


	sf::Event event;
	event.type = sf::Event::GainedFocus;


	char coding, playerID;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		while (window.pollEvent(event)) {
			quit.checkState();
			confirm.checkState();
			Chat.checkSideBarState();
			if (Chat.function() && event.type == sf::Event::KeyPressed) {
				if (Chat.addChar(event.key)) {
					auto text = Chat.getText();
					if (!text.empty())
						if (!Send(text, chatSocket))
						{
							;//TODO
						}
				}
			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
				Chat.scrolled(event.mouseWheelScroll.delta);
			}
			if (quit.buttonFunction())
				return state::exit;//todo put ensure pop allert
			if (confirm.buttonFunction()) {

			}
			else;
		}
		int len = getCommand(lobbySocket, coding, playerID);
		if (len)//check commands
		{
			sf::String out;
			switch (coding) {
			case codes::getRandomWhiteCards: //from server
			{
				std::vector<int> cards_;
				for (int i = 0; i < 10; i++) {//decoding 10 cards from buff
					uint16_t* ptr = (uint16_t*)(buff + 4 + i * 2);
					uint16_t id = ntohs(*ptr);
					cards_.push_back(id);
				}
				normalTable.setCards(cards_);
			}
			break;
			case codes::sendBlackCard:
			{
				uint16_t* ptr = (uint16_t*)(buff + 4);
				uint16_t id = ntohs(*ptr);
				black.setTextUtf8(deck.getCard(id, true));
				doubleMode = deck.getDouble(id);
			}
			break;
			case codes::sendChoserId:
				if (playerId == playerID) {// if choserid is equal to your id
					normalTable.hideF();
					chosingTabl.hideF(false);//unhide
					score.setChosing(playerID);
					chosingTabl.resetChosen();
					chosingTabl.setDouble(doubleMode);
					return state::choser;
				}
				else {
					normalTable.hideF(false);
					chosingTabl.hideF();
					score.setChosing(playerID);
					normalTable.resetChosen();
					normalTable.setDouble(doubleMode);
					return state::normal;
				}
				break;
			case codes::gameIsOver:
				Chat.send("Game is over", sf::Color::Yellow);
				//return state::exit;
				break;
			default:
				break;
			}
			score.update();
			window.clear(sf::Color::Black);
			window.draw(background);
			Chat.draw();
			score.draw();
			confirm.draw();
			quit.draw();
			window.display();
		}
	}
}

game::state game::choserF()
{
	int linex = 1920;
	int liney = 1080;

	Button confirm(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	confirm.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 200) * setting.yScale);
	confirm.setScale(setting.xScale, 1 * setting.yScale);
	confirm.setTitle("CONFIRM WINNER");
	confirm.setSoundVolume(setting.SoundVolume);
	confirm.setColor(sf::Color::White);

	Button quit(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	quit.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 100) * setting.yScale);
	quit.setScale(setting.xScale, 1 * setting.yScale);
	quit.setTitle("EXIT");
	quit.setSoundVolume(setting.SoundVolume);
	quit.setColor(sf::Color::White);

	sf::Event event;

	clock.setTimer(3, 0);
	clock.start();

	char coding, playerID;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		while (window.pollEvent(event)) {
			quit.checkState();
			confirm.checkState();
			Chat.checkSideBarState();
			if (Chat.function() && event.type == sf::Event::KeyPressed) {
				if (Chat.addChar(event.key)) {
					auto text = Chat.getText();
					if (!text.empty())
						if (!Send(text, chatSocket))
						{
							;//TODO
						}
				}
			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
				Chat.scrolled(event.mouseWheelScroll.delta);
			}
			if (quit.buttonFunction())
				return state::exit;//todo put ensure pop allert
			if (confirm.buttonFunction()) {

				if (!chosingTabl.selectedCards()) {//if you did not select cards
					Chat.send("You did not select card" + doubleMode ? "s" : "", sf::Color::Yellow);
				}
				else {
					char winnerId = chosingTabl.getChosenPlayerId();
					addMessagePrefix(buff, 1, codes::choserWinnerId, winnerId);
					if (lobbySocket.send(buff, 4) != sf::Socket::Done)//TODO
					{
						//TODO
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				chosingTabl.function();
			}
			else;
		}
		int len = getCommand(lobbySocket, coding, playerID);
		if (len)//check commands
		{
			sf::String out;
			switch (coding) {
			case codes::randomWinnerRequest:
			{
				if (!chosingTabl.selectedCards()) {
					Chat.send("Time is up and you did/'t chose card//s random will be chosen", sf::Color::Yellow);
					normalTable.choseRandom();
				}
				else
					Chat.send("Time is up", sf::Color::Yellow);
				char winnerId = chosingTabl.getChosenPlayerId();
				addMessagePrefix(buff, 1, codes::choserWinnerId, winnerId);
				if (lobbySocket.send(buff, 4) != sf::Socket::Done)//TODO
				{
					//TODO
				}
			}
			break;
			case codes::sendWinner:
				Chat.send("Round winner is " + players.at(playerID), sf::Color::Yellow);
				score.updateScore(playerID);
				return state::newRound;
				break;
			case codes::gameIsOver:
				Chat.send("Game is over", sf::Color::Yellow);
				//return state::exit;
				break;
			default:
				break;
			}
			score.update();
			if (clock.run()) {
			}
			window.clear(sf::Color::Black);
			window.draw(background);
			Chat.draw();
			score.draw();
			confirm.draw();
			quit.draw();
			chosingTabl.draw();
			window.draw(clock);
			window.draw(black);
			window.display();
		}
	}
}

game::state game::normalF()
{
	int linex = 1920;
	int liney = 1080;

	Button confirm(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	confirm.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 200) * setting.yScale);
	confirm.setScale(setting.xScale, 1 * setting.yScale);
	confirm.setTitle("CONFIRM CARDS");
	confirm.setSoundVolume(setting.SoundVolume);
	confirm.setColor(sf::Color::White);

	Button quit(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	quit.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 100) * setting.yScale);
	quit.setScale(setting.xScale, 1 * setting.yScale);
	quit.setTitle("EXIT");
	quit.setSoundVolume(setting.SoundVolume);
	quit.setColor(sf::Color::White);

	sf::Event event;

	clock.setTimer(3, 0);
	clock.start();

	char coding, playerID;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		while (window.pollEvent(event)) {
			quit.checkState();
			confirm.checkState();
			Chat.checkSideBarState();
			if (Chat.function() && event.type == sf::Event::KeyPressed) {
				if (Chat.addChar(event.key)) {
					auto text = Chat.getText();
					if (!text.empty())
						if (!Send(text, chatSocket))
						{
							;//TODO
						}
				}
			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
				Chat.scrolled(event.mouseWheelScroll.delta);
			}
			if (quit.buttonFunction())
				return state::exit;//todo put ensure pop allert
			if (confirm.buttonFunction()) {
				if (!normalTable.selectedCards()) {//if you did not select cards
					Chat.send("You did not select card" + doubleMode ? "s" : "", sf::Color::Yellow);
				}
				else {
					int len = 2;
					uint16_t first, secound, coded;
					first = normalTable.getFirst();
					coded = htons(first);
					memcpy(buff + 4, (char*)& coded, 2);
					if (doubleMode) {
						len += 2;
						secound = normalTable.getSecound();
						coded = htons(secound);
						memcpy(buff + 6, (char*)& coded, 2);
					}
					addMessagePrefix(buff, len, codes::sendChosenWhiteCards, playerId);
					if (lobbySocket.send(buff, 4 + len) != sf::Socket::Done)//TODO
					{
						//TODO
					}
					Chat.send("You have confirmed cards id: " + std::to_string(first) + (doubleMode ? (", " + std::to_string(secound)) : ""), sf::Color::Yellow);
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				normalTable.function();
			}
		}
		int len = getCommand(lobbySocket, coding, playerID);
		if (len)//check commands
		{
			sf::String out;
			switch (coding) {
			case codes::randomWhiteCardsRequest:
			{
				if (!normalTable.selectedCards()) {
					Chat.send("Time is up and you did/'t chose card//s random will be chosen", sf::Color::Yellow);
					normalTable.choseRandom();
				}
				else
					Chat.send("Time is up", sf::Color::Yellow);
				int len = 2;
				uint16_t first, secound, coded;
				first = normalTable.getFirst();
				coded = htons(first);
				memcpy(buff + 4, (char*)& coded, 2);
				if (doubleMode) {
					len += 2;
					secound = normalTable.getSecound();
					coded = htons(secound);
					memcpy(buff + 6, (char*)& coded, 2);
				}
				addMessagePrefix(buff, len, codes::sendChosenWhiteCards, playerId);
				if (lobbySocket.send(buff, 4 + len) != sf::Socket::Done)//TODO
				{
					//TODO
				}
				Chat.send("You have confirmed cards id: " + std::to_string(first) + (doubleMode ? (", " + std::to_string(secound)) : ""), sf::Color::Yellow);
			}
				break;
			case codes::getNewWhiteCards:
			{

			}
				break;
			case codes::sendWinner:
				Chat.send("Round winner is " + players.at(playerID), sf::Color::Yellow);
				score.updateScore(playerID);
				return state::newRound;
				break;
			case codes::gameIsOver:
				Chat.send("Game is over", sf::Color::Yellow);
				//return state::exit;
				break;
			default:
				break;
			}
			score.update();
			if (clock.run()) {
			}
			window.clear(sf::Color::Black);
			window.draw(background);
			Chat.draw();
			score.draw();
			confirm.draw();
			quit.draw();
			normalTable.draw();
			chosingTabl.draw();
			window.draw(clock);
			window.draw(black);
			window.display();
		}
	}
}

