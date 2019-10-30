#include"game.h"
#include <intrin.h>
#include <winsock2.h>
#include <uchar.h>
#include <iostream>
#include "inputText.h"
#include "Button.h"
#include "Rounds.h"
#include"lobbyPlayers.h"
#include "toggleTables.h"

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
	const char sendChosenToChoser = 30;//send cards to choser

}//TODO naprawnie deadlocka broadcast disconnect
game::game(sf::RenderWindow& win, sf::SoundBuffer& sndbuff, sf::Font font_, sf::String lobbyId_, sf::String nick, bool newlobby_) :deck(), window(win), Chat(win, sndbuff, 150, 12, font)
, font(font_), clickBuff(sndbuff), clock(font), normalTable(win, deck), chosingTabl(win, deck), score(win), black(card::kind::black) 
{
	newLobby = newlobby_;
	lobbyId = lobbyId_;
	nickname = nick;
	state_ = state::lobby;
	Chat.setValues(20, 600);
	Chat.setPosition(sf::Vector2f((1920 - 650), 50));
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
	if (!clockBack.loadFromFile("PNG/tmp.png"))
		throw std::exception("Sound file missing");
	
	if (!circle.loadFromFile("PNG/grey_boxTick.png"))
		throw std::exception("Sound file missing");

	if (!noCircle.loadFromFile("PNG/grey_circle.png"))
		throw std::exception("Sound file missing");


	if (!staticScoreBoard::checkText.loadFromFile("PNG/check-mark.png"))
		throw std::exception("png file missing");
	if (!staticScoreBoard::choserText.loadFromFile("PNG/bookmarklet.png"))
		throw std::exception("png file missing");
	if (!staticScoreBoard::lasWinnerText.loadFromFile("PNG/laurel-crown.png"))
		throw std::exception("png file missing");
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
		players[playerId].clear();
		players[playerId] = nickname;
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
	while (true) {
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
			break;
		default:
			break;
		}
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
	if (socket.receive(buff, 4, received) != sf::Socket::Done && received != 4)
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
			case codes::timeUpdate:
				gameTime = len;
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

	std::experimental::filesystem::path deck_ = "taliaRocka.txt";
	if (!deck.load(deck_))
		Chat.send("Could not load default deck: " + deck_.stem().string() + " validate txt file", sf::Color::Yellow);
	else
		Chat.send("Default Deck: " + deck_.filename().string() + " loaded", sf::Color::Yellow);

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
	for (auto& x : players) {
		lobbyClients.addPlayer(x.first, x.second);
	}

	bool allertFlag = false;
	chatSocket.setBlocking(false);
	lobbySocket.setBlocking(false);


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
				addMessagePrefix(buff, rounds.getNumber(), codes::timeUpdate, playerId);
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
			case codes::timeUpdate:
				gameTime = len;
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


	chosingTabl.init(players.size() - 1);
	chosingTabl.resetChosen();

	score.init(30, players, font);
	score.setColor(sf::Color::White);
	score.rotateMainPlayer(playerId);
	score.setPosition(50, 100);//set Pos and add players

	black.setOffest(20);
	black.setPosition(1920 - 400, 500);
	black.setCharSize(30);
	black.setId(0);
	black.setTextUtf8("");

	int linex = 1920;

	clock.setTexture(clockBack);
	clock.setTitle("Time:");
	clock.setPosition(50, 10);
	clock.setTimer(gameTime, 0);
	clock.start();
	clock.setSize(30);
	clock.setDeadline(1, 0);

	toggle.setTables(&normalTable, &chosingTabl);
	toggle.setTitle("Show your cards");
	toggle.title.setFillColor(sf::Color::White);
	toggle.title.setFont(font);
	toggle.title.setCharacterSize(20);
	toggle.setTextures(circle, noCircle);
	toggle.setPosition({ 100, 500 });
	toggle.setSound(clickBuff);
	toggle.setSpeed(0.02);

	Chat.send("Waiting for server", sf::Color::Yellow);
	return state::newRound;
}

game::state game::newRoundF()
{
	int linex = 1920;
	int liney = 1080;

	normalTable.resetChosen();
	chosingTabl.resetChosen();
	toggle.forcewNormalTable();
	toggle.block();

	score.resetCheck();

	Button confirm(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	confirm.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 200) * setting.yScale);
	confirm.setScale(setting.xScale*1.5, 1 * setting.yScale);
	confirm.setTitle("CONFIRM");
	confirm.setSoundVolume(setting.SoundVolume);
	confirm.setColor(sf::Color::White);

	Button quit(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	quit.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 100) * setting.yScale);
	quit.setScale(setting.xScale * 1.5, 1 * setting.yScale);
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
		int len = getCommand(lobbySocket, coding, playerID);//only code get also rest
		if (len)//check commands
		{
			sf::String out;
			switch (coding) {
			case codes::sendRandomTenCards: //from server
			{
				std::size_t received;
				if (lobbySocket.receive(buff, 20, received) != sf::Socket::Done && received != 20)//get rest data (10 cards in 20 bytes)
				{
					;//todo
				}
				normalTable.setCards(decodeCards(buff, 10)); // decode cards from message
				toggle.forcewNormalTable();
			}
			break;
			case codes::sendBlackCard://get black card from server
			{
				std::size_t received;
				if (lobbySocket.receive(buff, 2, received) != sf::Socket::Done && received != 2)//get rest data (10 cards in 20 bytes)
				{
					;//todo
				}
				uint16_t id = decodeCard(buff);
				black.setTextUtf8(deck.getCard(id, true));
				doubleMode = deck.getDouble(id);
			}
			break;
			case codes::sendChoserId:
				score.setChosing(playerID);
				if (playerId == playerID) {// if choser id is equal to your id
					score.setChosing(playerID);
					chosingTabl.resetChosen();
					chosingTabl.setDouble(doubleMode);
					toggle.blockChosing();
					return state::choser;
				}
				else {
					score.setChosing(playerID);
					normalTable.resetChosen();
					normalTable.setDouble(doubleMode);
					toggle.blockChosing();
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
		}
		toggle.update();
		window.clear(sf::Color::Black);
		window.draw(background);
		Chat.draw();
		window.draw(toggle);
		score.draw();
		confirm.draw();
		quit.draw();
		window.display();
	}
}

game::state game::choserF()
{
	int linex = 1920;
	int liney = 1080;

	Button confirm(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	confirm.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 200) * setting.yScale);
	confirm.setScale(setting.xScale * 1.5, 1 * setting.yScale);
	confirm.setTitle("CONFIRM WINNER");
	confirm.setSoundVolume(setting.SoundVolume);
	confirm.setColor(sf::Color::White);

	Button quit(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	quit.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 100) * setting.yScale);
	quit.setScale(setting.xScale * 1.5, 1 * setting.yScale);
	quit.setTitle("EXIT");
	quit.setSoundVolume(setting.SoundVolume);
	quit.setColor(sf::Color::White);

	sf::Event event;

	enum intState { wait, run };
	intState state_ = wait;

	std::vector<sf::Vector2i> cards_;

	int gotCards = 0;
	Chat.send("You are chosing cards, wait for other players", sf::Color::Yellow);

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
				if (state_ == intState::run) {
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
			}
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				normalTable.function();
				chosingTabl.function();
				toggle.function();
			}
			else;
		}
		int len = getCommand(lobbySocket, coding, playerID);
		if (len)//check commands
		{
			switch (coding) {
			case codes::sendChosenWhiteCards://got cards from other players one or two
			{
				std::size_t received;
				int howMany = doubleMode ? 4 : 2;
				if (lobbySocket.receive(buff, howMany, received) != sf::Socket::Done && received != howMany)//
				{
					;//todo
				}
				cards_.emplace_back(decodeCard(buff), playerID);// first 
				if (doubleMode) {
					cards_.emplace_back(decodeCard(buff + 2), playerID);//sec
				}
				gotCards++;
				score.check(playerID);
				if (gotCards == players.size() - 1) {// got all cards from players accept you (choser)
					Chat.send("All players sent cards, chose winner", sf::Color::Yellow);
					chosingTabl.setCards(cards_, doubleMode);
					toggle.forceChosingTable();
					toggle.blockNormal();
					toggle.unBlock();
					state_ = intState::run;
				}
			}
			break;
			case codes::sendWinner:
				Chat.send("Round winner is " + players.at(playerID), sf::Color::Yellow);
				score.updateScore(playerID);
				return state::newRound;
				break;
			case codes::playerIsNotResponsing:
				Chat.send("Player " + players.at(playerID) + " is not responsing", sf::Color::Yellow);
				break;
			case codes::gameIsOver:
				Chat.send("Game is over", sf::Color::Yellow);
				//return state::exit;
				break;
			default:
				break;
			}
		}
		toggle.update();
		if (clock.run()) {
		}
		window.clear(sf::Color::Black);
		window.draw(background);
		Chat.draw();
		score.draw();
		confirm.draw();
		quit.draw();
		window.draw(toggle);
		window.draw(clock);
		window.draw(black);
		window.display();
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

	enum intState { wait, run };
	intState state_ = intState::run;

	sf::Event event;
	Chat.send("Chose card//s", sf::Color::Yellow);
	std::vector<sf::Vector2i> cards_;
	int gotCards = 0;

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
				if (state_ == intState::run) {
					if (!normalTable.selectedCards()) {//if you did not select cards
						Chat.send("You did not select card" + doubleMode ? "s" : "", sf::Color::Yellow);
					}
					else {
						int len = 2;
						uint16_t first, secound, coded;
						first = normalTable.getFirst();
						codeCard(buff + 4, first);
						if (doubleMode) {
							len += 2;
							secound = normalTable.getSecound();
							codeCard(buff + 6, secound);
						}
						addMessagePrefix(buff, len, codes::sendChosenWhiteCards, playerId);
						if (lobbySocket.send(buff, 4 + len) != sf::Socket::Done)//TODO
						{
							//TODO
						}
						Chat.send("You have confirmed cards id: " + std::to_string(first) + (doubleMode ? (", " + std::to_string(secound)) : ""), sf::Color::Yellow);
						state_ = intState::wait;
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				normalTable.function();
				chosingTabl.function();
				toggle.function();
			}
		}
		int len = getCommand(lobbySocket, coding, playerID);
		if (len)//check commands
		{
			sf::String out;
			switch (coding) {
			case codes::sendChosenWhiteCards:
			{
				std::size_t received;
				int howMany = doubleMode ? 4 : 2;
				if (lobbySocket.receive(buff, howMany, received) != sf::Socket::Done && received != howMany)//
				{
					;//todo
				}
				cards_.emplace_back(decodeCard(buff), playerID);// first 
				if (doubleMode) {
					cards_.emplace_back(decodeCard(buff + 2), playerID);//sec
				}
				gotCards++;
				score.check(playerID);
				if (gotCards == players.size() - 1) {// got all cards from players accept you (choser)
					Chat.send("All players sent cards, chose winner", sf::Color::Yellow);
					chosingTabl.setCards(cards_, doubleMode);
					toggle.forceChosingTable();
					toggle.blockNormal();
					toggle.unBlock();
					state_ = intState::run;
				}
			}
				break;
			case codes::getNewWhiteCards:
			{
				std::size_t received;
				int howMany = doubleMode ? 4 : 2;
				if (lobbySocket.receive(buff, howMany, received) != sf::Socket::Done && received != howMany)//get rest data (10 cards in 20 bytes)
				{
					;//todo
				}
				gotFirst = decodeCard(buff);//first
				if (doubleMode)
					gotSec = decodeCard(buff + 2);//sec
			}
			break;
			case codes::sendWinner://got winner from server
				Chat.send("Round winner is " + players.at(playerID), sf::Color::Yellow);
				score.updateScore(playerID);
				normalTable.replaceChosenFirst(gotFirst);
				if (doubleMode)
					normalTable.replaceChosenSecound(gotSec);
				return state::newRound;
				break;
			case codes::playerIsNotResponsing:
				Chat.send("Player " + players.at(playerID) + " is not responsing", sf::Color::Yellow);
				break;
			case codes::gameIsOver:
				Chat.send("Game is over", sf::Color::Yellow);
				//return state::exit;
				break;
			default:
				break;
			}
		}
		toggle.update();
		if (clock.run()) {
		}
		window.clear(sf::Color::Black);
		window.draw(background);
		Chat.draw();
		score.draw();
		confirm.draw();
		quit.draw();
		window.draw(toggle);
		window.draw(clock);
		window.draw(black);
		window.display();
	}
}

