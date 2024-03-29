#include "menu.h"
#include "inputText.h"
#include "PopAlert.h"
#include"game.h"
#include "table.h"
#include "chat.h"
#include "timer.h"
#include "chosingTable.h"
#include "toggleTables.h"

Menu::Menu(sf::RenderWindow& win, std::string& ver_) : window(win), version(ver_)
{

	if (!clickBuff.loadFromFile("Sounds/click1.ogg"))
		throw std::exception("Sound file missing");

	if (!switchBuff.loadFromFile("Sounds/switch2.ogg"))
		throw std::exception("Sound file missing");

	if (!font2.loadFromFile("Fonts/NunitoSans-Regular.ttf"))
		throw std::exception("font file missing");

	if (!font.loadFromFile("Fonts/NunitoSans-Bold.ttf"))
		throw std::exception("font file missing");

	if (!font3.loadFromFile("Fonts/Lato-Regular.ttf"))
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

	card::setFont("Fonts/Lato-Regular.ttf");
	card::setBlackTexture("PNG/newBlack.png");
	card::setWhiteTexture("PNG/newWhite.png");

	background.setTexture(backgroundTexture);
}

st Menu::mainMenuUpdate()
{
	int linex = 1920 / 2;
	int liney = 1080 / 2;


	Button play(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	play.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney - 100) * setting.yScale);
	play.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	play.setTitle("PLAY GAME");
	play.setSoundVolume(setting.SoundVolume);
	play.setColor(sf::Color::White);

	Button settings(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	settings.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney)* setting.yScale);
	settings.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	settings.setTitle("SETTINGS");
	settings.setSoundVolume(setting.SoundVolume);
	settings.setColor(sf::Color::White);

	Button quit(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	quit.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 100) * setting.yScale);
	quit.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	quit.setTitle("QUIT");
	quit.setSoundVolume(setting.SoundVolume);
	quit.setColor(sf::Color::White);
	sf::Event event;
	event.type = sf::Event::GainedFocus;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop

		do {
			play.checkState();
			settings.checkState();
			quit.checkState();
			if (play.buttonFunction())
				return st::choseGameMode;
			else if (settings.buttonFunction())
				return st::settings;// return st::settings;
			else if (quit.buttonFunction())
				return st::quit;
			else;

		} while (window.pollEvent(event));

		window.clear(sf::Color::Black);

		window.draw(background);
		play.draw();
		settings.draw();
		quit.draw();
		window.display();
	}
}
st Menu::choseGameMode()
{
	int linex = 1920 / 2;
	int liney = 1080 / 2;


	Button createLobby(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	createLobby.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney - 100) * setting.yScale);
	createLobby.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	createLobby.setTitle("CREATE NEW LOBBY");
	createLobby.setSoundVolume(setting.SoundVolume);
	createLobby.setColor(sf::Color::White);

	Button joinLobby(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	joinLobby.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney)* setting.yScale);
	joinLobby.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	joinLobby.setTitle("JOIN LOBBY");
	joinLobby.setSoundVolume(setting.SoundVolume);
	joinLobby.setColor(sf::Color::White);

	Button goBack(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	goBack.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 100) * setting.yScale);
	goBack.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	goBack.setTitle("BACK");
	goBack.setSoundVolume(setting.SoundVolume);
	goBack.setColor(sf::Color::White);


	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		do {
			createLobby.checkState();
			joinLobby.checkState();
			goBack.checkState();
			if (createLobby.buttonFunction()) {
				setting.newLobby = true;
				return st::connectToLobby;
			}
			else if (joinLobby.buttonFunction()) {
				setting.newLobby = false;
				return st::connectToLobby;
			}
			else if (goBack.buttonFunction())
				return st::mainMenu;
			else;
		} while (window.pollEvent(event));

		window.clear(sf::Color::Black);

		window.draw(background);
		createLobby.draw();
		joinLobby.draw();
		goBack.draw();
		window.display();
	}
}
st Menu::test()
{
	int linex = 1920;
	int liney = 1080;

	Deck deck;

	if (!deck.load("test.txt")) { ; }
	//todo

	Button next(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	next.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 200) * setting.yScale);
	next.setScale(setting.xScale, 1 * setting.yScale);
	next.setTitle("NEXT");
	next.setSoundVolume(setting.SoundVolume);
	next.setColor(sf::Color::White);

	Button quit(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	quit.setPosition((linex - 190 * 1.8) * setting.xScale, (liney - 100) * setting.yScale);
	quit.setScale(setting.xScale, 1 * setting.yScale);
	quit.setTitle("BACK");
	quit.setSoundVolume(setting.SoundVolume);
	quit.setColor(sf::Color::White);


	/*std::vector <int> xd = { 0,1,2,3,4,5,6,7, 8, 9 };
	table tabl(window, 10);

	if (!tabl.init(xd))
		;//todo
	tabl.setDouble(true);
	*/
	std::vector<sf::Vector2i> xd = { {0,0},{1,0},{2,1},{3,1},{4,2},{5,2},{6,3},{7,3},{8,4},{9,4},{10,5},{11,5},{12,6},{13,6} };
	std::vector<int> xdd = { 0,1,2,3,4,5,6,7,8,9 };
	bool doubl = false;
	int siz = 6;
	table normalTable(window, deck);
	normalTable.init(10);
	normalTable.resetChosen();
	normalTable.setCards(xdd);

	chosingTable chosingTabl(window, deck);
	chosingTabl.init(xd.size() / 2, font2);
	chosingTabl.setNickOffset({ 19,215 }, 18);
	chosingTabl.resetChosen();
	std::vector<sf::String> nicks(xd.size(), "nick");
	chosingTabl.setCards(xd, nicks, true);
	chosingTabl.showNick();

	sf::Texture fir;
	fir.loadFromFile("PNG/grey_boxTick.png");

	sf::Texture sec;
	sec.loadFromFile("PNG/grey_circle.png");

	toggleTables toggle;
	toggle.setTables(&normalTable, &chosingTabl);
	toggle.setTitle("Show your cards");
	toggle.title.setFillColor(sf::Color::White);
	toggle.title.setFont(font);
	toggle.title.setCharacterSize(20);
	toggle.setTextures(sec, fir);
	toggle.setPosition({ 100, 500 });
	toggle.setSound(clickBuff);

	toggle.forcewNormalTable();
	toggle.setSpeed(0.02);
	if (!staticScoreBoard::checkText.loadFromFile("PNG/check-mark.png"))
		throw std::exception("png file missing");
	if (!staticScoreBoard::choserText.loadFromFile("PNG/bookmarklet.png"))
		throw std::exception("png file missing");
	if (!staticScoreBoard::lasWinnerText.loadFromFile("PNG/laurel-crown.png"))
		throw std::exception("png file missing");

	chat Chat(window, clickBuff, 150, 12, font);
	Chat.setValues(20, 600);
	Chat.setPosition(sf::Vector2f((1920 - 650), 50));

	staticScoreBoard score(window);
	score.setColor(sf::Color::White);
	std::map<int, sf::String> pla;
	pla[0] = "tom";
	pla[1] = "john";
	pla[2] = "marry";
	pla[3] = "ham";

	pla[4] = "xDDDDDDDDDD";
	pla[5] = "sylwek";
	pla[6] = "jack";
	pla[7] = "muck";
	score.init(30, pla, font);
	score.setColor(sf::Color::White);
	score.setPosition(50, 80);
	score.rotateMainPlayer(3);

	card black(card::kind::black);
	black.setOffest(30);
	black.setIndexOffest({ 212,288 }, 27);
	black.setPosition(1920 - 400, 500);
	black.setCharSize(26);
	black.setId(0);
	black.setTextUtf8("W mieszkaniu znanego dziennikarza znaleziono __.");
	sf::Texture tmp;
	tmp.loadFromFile("PNG/tmp.png");

	timer clock(font);
	clock.setTitle("Time:");
	clock.setPosition(50, 10);
	clock.setSize(30);
	clock.setTimer(0, 10);
	clock.setDeadline(0, 5);
	clock.start();

	sf::Event event;
	event.type = sf::Event::GainedFocus;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		while (window.pollEvent(event)) {
			quit.checkState();
			next.checkState();
			Chat.checkSideBarState();
			if (event.type == sf::Event::MouseButtonReleased && toggle.function()) {
				;
			}
			else if (Chat.function() && event.type == sf::Event::KeyPressed) {
				if (Chat.addChar(event.key)) {
					auto text = Chat.getText();
					if (text.size() > 1) {
						char choseee = char(text[0]);
						if (choseee == 'q') {
							toggle.forceChosingTable();
							continue;
						}
						else if (choseee == 'w') {
							toggle.forcewNormalTable();
							continue;
						}
						else if (choseee == 'b') {
							toggle.block();
							continue;
						}
						else if (choseee == 'u') {
							toggle.unBlock();
							continue;
						}
						char x = char(text[1]);
						std::string dd;
						dd += x;
						int index = -1;
						try {
							index = std::stoi(dd);
						}
						catch (...) {}
						if (index < 0 || index > pla.size() - 1)
							continue;
						if (choseee == 's') {
							score.updateScore(index);
						}
						else if (choseee == 'c')
							score.check(index);
						else {
							score.setChosing(index);
						}
					}

				}
			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
				Chat.scrolled(event.mouseWheelScroll.delta);
			}
			else if (quit.buttonFunction())
				return st::quit;
			else if (next.buttonFunction()) {
				toggle.forcewNormalTable();
			}
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				toggle.function();
				normalTable.function();
				chosingTabl.function();
			}
			else;
		}
		toggle.update();
		if (clock.run()) {
			clock.setTimer(0, 10);
			clock.start();
		}
		window.clear(sf::Color::Black);
		window.draw(background);
		Chat.draw();
		score.draw();
		next.draw();
		quit.draw();
		window.draw(toggle);
		window.draw(clock);
		window.draw(black);
		window.display();
	}
}
st Menu::ConnectToLobby()
{
	int linex = 1920 / 2;
	int liney = 1080 / 2;
	PopAlert alert(window, "asdasd", whiteBox, blockPressed, block, offButton, clickBuff, switchBuff, font);
	sf::Sprite textBox1, textBox2;

	textBox1.setTexture(textBox);
	textBox1.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney - 100) * setting.yScale);
	textBox1.setScale(1.8 * setting.xScale, 1 * setting.yScale);

	textBox2.setTexture(textBox);
	textBox2.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney)* setting.yScale);
	textBox2.setScale(1.8 * setting.xScale, 1 * setting.yScale);

	inputText lobbyId(window, clickBuff, 20);
	lobbyId.setBounds(sf::Vector2f(textBox1.getGlobalBounds().left, textBox1.getGlobalBounds().top), sf::Vector2f(textBox1.getGlobalBounds().width, textBox1.getGlobalBounds().height));
	lobbyId.setString("Lobby id");
	lobbyId.setPosition((linex - 190 * 1.8 / 2) * setting.xScale + 10, (liney - 100) * setting.yScale + 10);
	lobbyId.setColor();
	lobbyId.setFont(font);
	lobbyId.setSize(25);

	inputText nickname(window, clickBuff, 20);
	nickname.setBounds(sf::Vector2f(textBox2.getGlobalBounds().left, textBox2.getGlobalBounds().top), sf::Vector2f(textBox2.getGlobalBounds().width, textBox2.getGlobalBounds().height));
	nickname.setString("Nickname");
	nickname.setPosition((linex - 190 * 1.8 / 2) * setting.xScale + 10, (liney)* setting.yScale + 10);
	nickname.setColor();
	nickname.setFont(font);
	nickname.setSize(25);

	Button connect(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	connect.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 100) * setting.yScale);
	connect.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	connect.setTitle("CONNECT");
	connect.setSoundVolume(setting.SoundVolume);
	connect.setColor(sf::Color::White);

	Button goBack(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	goBack.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 200) * setting.yScale);
	goBack.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	goBack.setTitle("BACK");
	goBack.setSoundVolume(setting.SoundVolume);
	goBack.setColor(sf::Color::White);

	bool allertFlag = false;
	sf::Clock timer;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		event.type = sf::Event::GainedFocus;
		do {
			if (!allertFlag) {
				connect.checkState();
				goBack.checkState();
				if (nickname.function() && event.type == sf::Event::KeyPressed) {
					if (nickname.addChar(event.key)) {
						std::wcout << nickname.getText().toWideString() << std::endl;
					}
				}
				if (lobbyId.function() && event.type == sf::Event::KeyPressed) {
					if (lobbyId.addChar(event.key)) {
						std::wcout << lobbyId.getText().toWideString() << std::endl;
					}
				}
				else if (connect.buttonFunction()) {
					game gameLobby(window, clickBuff, font, lobbyId.getText(), nickname.getText(), setting.newLobby);
					auto msg = gameLobby.connect();
					if (msg == message::connected) {
						//ok play
						gameLobby.run();
					}
					else {
						alert.setText(message::getConnectError(msg));//TO wyswietlanie texstu jak w czasie zrobienie klasy
						allertFlag = true;
						alert.show();
					}
				}
				else if (goBack.buttonFunction())
					return st::choseGameMode;
				else;
			}
			else {
				alert.checkState();
				if (alert.function())
				{
					allertFlag = false;
					alert.hide();
				}
			}
		} while (window.pollEvent(event));

		window.clear(sf::Color::Black);
		window.draw(background);
		window.draw(textBox1);
		window.draw(textBox2);
		nickname.draw();
		lobbyId.draw();
		connect.draw();
		goBack.draw();
		alert.draw();
		window.display();
	}
}
/*st Menu::settingsUupdate()
{
	PopAlert alert(window, "asdasd", whiteBox, blockPressed, block, offButton, clickBuff, switchBuff, font);

	int linex = 1920 / 2 - 190 * 1.8 / 2;
	int liney = 1080 / 2 - 50;

	sf::Event event;


	sf::Text VolMusicText;
	VolMusicText.setString("Music Volume");
	VolMusicText.setFont(font);
	VolMusicText.setCharacterSize(25);
	VolMusicText.setPosition((linex + 20) * setting.xScale, (liney - 400 + 8) * setting.yScale);// .getPosition().x + 20, VolumeMusic.getPosition().y + 8);
	VolMusicText.setScale(setting.xScale, setting.yScale);

	sf::Text VolSounndEffectsTest;
	VolSounndEffectsTest.setString("Sound Effects Volume");
	VolSounndEffectsTest.setFont(font);
	VolSounndEffectsTest.setCharacterSize(25);
	VolSounndEffectsTest.setPosition((linex + 20) * setting.xScale, (liney - 200 + 8) * setting.yScale); //(VolumeSoundEffects.getPosition().x + 20, VolumeSoundEffects.getPosition().y + 8);
	VolSounndEffectsTest.setScale(setting.xScale, setting.yScale);

	sf::Text ChoseMusicText;
	ChoseMusicText.setString("Chose music file");
	ChoseMusicText.setFont(font);
	ChoseMusicText.setCharacterSize(25);
	ChoseMusicText.setPosition((linex + 20) * setting.xScale, (liney - 50 + 8) * setting.yScale); //(ChoseMusic.getPosition().x + 20, ChoseMusic.getPosition().y + 8);
	ChoseMusicText.setScale(setting.xScale, setting.yScale);

	sf::Text fileError;
	fileError.setString("could not open file");
	fileError.setFont(font);
	fileError.setCharacterSize(15);
	fileError.setPosition(-200, -200); //(ChoseMusic.getPosition().x + 20, ChoseMusic.getPosition().y + 8);
	fileError.setFillColor(sf::Color::Red);
	fileError.setScale(setting.xScale, setting.yScale);

	Slider musicSlider(window, VolumePointner, VolumeSliderLine, clickBuff);
	musicSlider.setPosition(linex, liney - 300, (music.getVolume() / 30));//1- music /2 -sound
	musicSlider.setSoundVolume(setting.MusicVolume);

	Slider soundSlider(window, VolumePointner, VolumeSliderLine, clickBuff);
	soundSlider.setPosition(linex, liney - 100, setting.SoundVolume);
	soundSlider.setSoundVolume(setting.SoundVolume);

	List list;
	std::filesystem::path directory = "Music";

	int i = 0;
	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		//std::cout << entry.path().generic_string() << " to tutaj"<< std::endl;
		list.pushBack(window, blockPressed, listBlock, listBlock, clickBuff, switchBuff, font);
		list.setPosition(linex * setting.xScale, (liney + 50 + i * 50) * setting.yScale);
		list.setScale(1.8 * setting.xScale, 1 * setting.yScale);
		list.setText(entry.path().filename().generic_string());
		i++;
		if (i == 5)
			break;
	}
	list.setSoundVolume(setting.SoundVolume);
	list.chooseDefault();

	Button goBack(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	goBack.setPosition(linex * setting.xScale, setting.yScale * (liney + 200 + 4 * 58));
	goBack.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	goBack.setTitle("Back");
	goBack.setSoundVolume(setting.SoundVolume);


	Button musicApply(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	musicApply.setPosition(linex * setting.xScale, (liney + 100 + 4 * 58) * setting.yScale);
	musicApply.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	musicApply.setTitle("apply");
	musicApply.setSoundVolume(setting.SoundVolume);

	bool alertFlag = false;
	double volume;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (!alertFlag)
			{
				goBack.checkState();
				musicApply.checkState();
				musicSlider.checkState();
				soundSlider.checkState();
				list.checkState();
				if (musicApply.buttonFunction())
				{
					bool try_ = music.setGameMusic(list.getFileDirect());
					if (!try_)
					{
						alert.setText("nie udalo sie odczytac \npliku, moze uzyj spotify :)");
						alertFlag = true;
						musicApply.normalization();
						alert.show();
					}
				}
				else if (goBack.buttonFunction())
				{
					if (setting.pause)
						return st::pause;
					else
						return st::mainMenu;
				}
				else if (musicSlider.sliderFunction())
				{
					setting.MusicVolume = musicSlider.getValue();
					music.setVolume(musicSlider.getValue() * 30);
				}
				else if (soundSlider.sliderFunction())
				{
					setting.SoundVolume = soundSlider.getValue();
					musicSlider.setSoundVolume(setting.SoundVolume);
					soundSlider.setSoundVolume(setting.SoundVolume);
					goBack.setSoundVolume(setting.SoundVolume);
					list.setSoundVolume(setting.SoundVolume);
					musicApply.setSoundVolume(setting.SoundVolume);
				}
				else;
			}
			else
			{
				alert.checkState();
				if (alert.function())
				{
					alertFlag = false;
					alert.hide();
				}
			}
		}
		window.clear(sf::Color::Black);

		background.draw();

		goBack.draw();
		musicSlider.draw();
		soundSlider.draw();
		list.draw();
		musicApply.draw();

		window.draw(fileError);
		window.draw(ChoseMusicText);
		window.draw(VolMusicText);
		window.draw(VolSounndEffectsTest);
		alert.draw();

		window.display();
	}
	return st::mainMenu;
}

*/
Menu::~Menu()
{
}
