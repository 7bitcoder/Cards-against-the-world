#include "menu.h"

Menu::Menu(sf::RenderWindow& win, std::string& ver_) : window(win), version(ver_), background(win)
{

	if (!clickBuff.loadFromFile("../Sounds/click1.ogg"))
		throw std::exception("Sound file missing");

	if (!switchBuff.loadFromFile("../Sounds/switch2.ogg"))
		throw std::exception("Sound file missing");

	if (!font2.loadFromFile("../Font/kenvector_future_thin.ttf"))
		throw std::exception("font file missing");

	if (!font.loadFromFile("../Font/kenvector_future.ttf"))
		throw std::exception("font file missing");

	if (!checkOff.loadFromFile("../PNG/grey_box.png"))
		throw std::exception("png file missing");

	if (!checkOnBlue.loadFromFile("../PNG/blue_boxCheckmark.png"))
		throw std::exception("png file missing");

	if (!controlOnBlue.loadFromFile("../PNG/blue_button06.png"))
		throw std::exception("png file missing");

	if (!controlOffBlue.loadFromFile("../PNG/blue_button11.png"))
		throw std::exception("png file missing");

	if (!nickNameOnBlue.loadFromFile("../PNG/blue_button13.png"))
		throw std::exception("png file missing");

	if (!nickNameOffBlue.loadFromFile("../PNG/blue_button04.png"))
		throw std::exception("png file missing");

	if (!checkOnGreen.loadFromFile("../PNG/green_boxCheckmark.png"))
		throw std::exception("png file missing");

	if (!controlOnGreen.loadFromFile("../PNG/green_button06.png"))
		throw std::exception("png file missing");

	if (!controlOffGreen.loadFromFile("../PNG/green_button11.png"))
		throw std::exception("png file missing");

	if (!nickNameOnGreen.loadFromFile("../PNG/green_button13.png"))
		throw std::exception("png file missing");

	if (!nickNameOffGreen.loadFromFile("../PNG/green_button04.png"))
		throw std::exception("png file missing");


	if (!checkOnYellow.loadFromFile("../PNG/yellow_boxCheckmark.png"))
		throw std::exception("png file missing");

	if (!controlOnYellow.loadFromFile("../PNG/yellow_button06.png"))
		throw std::exception("png file missing");

	if (!controlOffYellow.loadFromFile("../PNG/yellow_button11.png"))
		throw std::exception("png file missing");

	if (!nickNameOnYellow.loadFromFile("../PNG/yellow_button13.png"))
		throw std::exception("png file missing");

	if (!nickNameOffYellow.loadFromFile("../PNG/yellow_button04.png"))
		throw std::exception("png file missing");


	if (!checkOnPink.loadFromFile("../PNG/pink_boxCheckmark.png"))
		throw std::exception("png file missing");

	if (!controlOnPink.loadFromFile("../PNG/pink_button06.png"))
		throw std::exception("png file missing");

	if (!controlOffPink.loadFromFile("../PNG/pink_button11.png"))
		throw std::exception("png file missing");

	if (!nickNameOnPink.loadFromFile("../PNG/pink_button13.png"))
		throw std::exception("png file missing");

	if (!nickNameOffPink.loadFromFile("../PNG/pink_button04.png"))
		throw std::exception("png file missing");


	if (!checkOnOliwe.loadFromFile("../PNG/oliwe_boxCheckmark.png"))
		throw std::exception("png file missing");

	if (!controlOnOliwe.loadFromFile("../PNG/oliwe_button06.png"))
		throw std::exception("png file missing");

	if (!controlOffOliwe.loadFromFile("../PNG/oliwe_button11.png"))
		throw std::exception("png file missing");

	if (!nickNameOnOliwe.loadFromFile("../PNG/oliwe_button13.png"))
		throw std::exception("png file missing");

	if (!nickNameOffOliwe.loadFromFile("../PNG/oliwe_button04.png"))
		throw std::exception("png file missing");


	if (!checkOnSkye.loadFromFile("../PNG/skye_boxCheckmark.png"))
		throw std::exception("png file missing");

	if (!controlOnSkye.loadFromFile("../PNG/skye_button06.png"))
		throw std::exception("png file missing");

	if (!controlOffSkye.loadFromFile("../PNG/skye_button11.png"))
		throw std::exception("png file missing");

	if (!nickNameOnSkye.loadFromFile("../PNG/skye_button13.png"))
		throw std::exception("png file missing");

	if (!nickNameOffSkye.loadFromFile("../PNG/skye_button04.png"))
		throw std::exception("png file missing");


	if (!checkOnRed.loadFromFile("../PNG/red_boxCheckmark.png"))
		throw std::exception("png file missing");

	if (!controlOnRed.loadFromFile("../PNG/red_button03.png"))
		throw std::exception("png file missing");

	if (!controlOffRed.loadFromFile("../PNG/red_button08.png"))
		throw std::exception("png file missing");

	if (!nickNameOnRed.loadFromFile("../PNG/red_button10.png"))
		throw std::exception("png file missing");

	if (!nickNameOffRed.loadFromFile("../PNG/red_button01.png"))
		throw std::exception("png file missing");


	if (!checkOnPurple.loadFromFile("../PNG/purple_boxCheckmark.png"))
		throw std::exception("png file missing");

	if (!controlOnPurple.loadFromFile("../PNG/purple_button03.png"))
		throw std::exception("png file missing");

	if (!controlOffPurple.loadFromFile("../PNG/purple_button08.png"))
		throw std::exception("png file missing");

	if (!nickNameOnPurple.loadFromFile("../PNG/purple_button10.png"))
		throw std::exception("png file missing");

	if (!nickNameOffPurple.loadFromFile("../PNG/purple_button01.png"))
		throw std::exception("png file missing");



	if (!block.loadFromFile("../PNG/grey_button15.png"))
		throw std::exception("png file missing");

	if (!blockPressed.loadFromFile("../PNG/grey_button00.png"))
		throw std::exception("png file missing");

	if (!offButton.loadFromFile("../PNG/grey_button03.png"))
		throw std::exception("png file missing");

	if (!back.loadFromFile("../stars_space_sky_glitter_116409_1920x108022.jpg"))
		throw std::exception("png background file missing");
	//back.setSmooth(true);

	if (!whiteBox.loadFromFile("../PNG/grey_panel.png"))
		throw std::exception("png file missing");

	if (!base.loadFromFile("../base3.png"))
		throw std::exception("png file missing");

	if (!mar.loadFromFile("../PNG/grey_sliderRight.png"))
		throw std::exception("png file missing");

	if (!VolumeSliderLine.loadFromFile("../PNG/grey_sliderHorizontal.png"))
		throw std::exception("png file missing");

	if (!VolumePointner.loadFromFile("../PNG/grey_sliderUp.png"))
		throw std::exception("png file missing");

	if (!listBlock.loadFromFile("../PNG/grey_button06.png"))
		throw std::exception("png file missing");


	if (!roundsOff.loadFromFile("../PNG/rounds_off__button06.png"))
		throw std::exception("png file missing");

	if (!roundsOn.loadFromFile("../PNG/rounds_on_button11.png"))
		throw std::exception("png file missing");


	if (!nicknameInfo.loadFromFile("../PNG/nickname.png"))
		throw std::exception("png file missing");

	if (!controlsInfo.loadFromFile("../PNG/controls.png"))
		throw std::exception("png file missing");

	if (!roundsInfo.loadFromFile("../PNG/rounds.png"))
		throw std::exception("png file missing");

	if (!backgroundShader.loadFromFile("../shaders/shaders/fragment/material-specular.frag", sf::Shader::Fragment))
		throw std::exception("shader file missing");

	background.setShader(backgroundShader);
	background.set(back, base, 0, 0.002);

}

st Menu::mainMenuUpdate()
{
	int linex = 1920 / 2;
	int liney = 1080 / 2;

	sf::Text title;
	title.setFont(font);
	title.setCharacterSize(140);
	title.setString("Sylio");
	title.setPosition((linex - 250) * setting.xScale, (liney - 300) * setting.yScale);
	title.setFillColor(sf::Color::White);
	title.setScale(setting.xScale, setting.yScale);

	sf::Text ver;
	ver.setFont(font);
	ver.setCharacterSize(15);
	ver.setString(version);
	ver.setPosition((linex * 2 - 100) * setting.xScale, (liney * 2 - 15) * setting.yScale);
	ver.setScale(setting.xScale, setting.yScale);

	Button normalGame(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	normalGame.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney - 100) * setting.yScale);
	normalGame.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	normalGame.setTitle("normal game");
	normalGame.setSoundVolume(setting.SoundVolume);


	Button multiplayerGame(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	multiplayerGame.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney)* setting.yScale);
	multiplayerGame.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	multiplayerGame.setTitle("multipleyer game");
	multiplayerGame.setSoundVolume(setting.SoundVolume);


	Button settings(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	settings.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 100) * setting.yScale);
	settings.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	settings.setTitle("settings");
	settings.setSoundVolume(setting.SoundVolume);


	Button quit(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	quit.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 200) * setting.yScale);
	quit.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	quit.setTitle("quit");
	quit.setSoundVolume(setting.SoundVolume);



	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			normalGame.checkState();
			multiplayerGame.checkState();
			settings.checkState();
			quit.checkState();
			if (normalGame.buttonFunction())
				return st::normalGame;
			else if (multiplayerGame.buttonFunction());
			else if (settings.buttonFunction())
				return st::settings;
			else if (quit.buttonFunction())
				return st::quit;
			else;
		}

		window.clear(sf::Color::Black);

		background.draw();
		window.draw(ver);
		window.draw(title);

		normalGame.draw();
		multiplayerGame.draw();
		settings.draw();
		quit.draw();
		window.display();
	}
}
st Menu::settingsUupdate()
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
	std::filesystem::path directory = "../Music";

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
st Menu::pauseUpdate()
{
	int linex = 1920 / 2;
	int liney = 1080 / 2;

	sf::Text title;
	title.setFont(font);
	title.setCharacterSize(60);
	title.setString("Pause");
	title.setPosition((linex - 120) * setting.xScale, (liney - 100) * setting.yScale);
	title.setScale(setting.xScale, setting.yScale);
	title.setFillColor(sf::Color::White);

	Button playGame(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	playGame.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, liney * setting.yScale);
	playGame.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	playGame.setTitle("play");
	playGame.setSoundVolume(setting.SoundVolume);


	Button settings(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	settings.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 100) * setting.yScale);
	settings.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	settings.setTitle("settings");
	settings.setSoundVolume(setting.SoundVolume);


	Button mainMenu(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	mainMenu.setPosition((linex - 190 * 1.8 / 2) * setting.xScale, (liney + 200) * setting.yScale);
	mainMenu.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	mainMenu.setTitle("main menu");
	mainMenu.setSoundVolume(setting.SoundVolume);



	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			playGame.checkState();
			settings.checkState();
			mainMenu.checkState();
			if (playGame.buttonFunction())
			{
				return st::playNormalGame;
			}
			else if (settings.buttonFunction())
				return st::settings;
			else if (mainMenu.buttonFunction())
			{
				setting.pause = false;
				return st::mainMenu;
			}
			else;
		}

		window.clear(sf::Color::Black);

		background.draw();
		window.draw(title);

		playGame.draw();
		settings.draw();
		mainMenu.draw();
		window.display();
	}
}
st Menu::normalGameUpdate()
{
	setting.playersSettings.clear();

	PopAlert alert(window, "asdasd", whiteBox, blockPressed, block, offButton, clickBuff, switchBuff, font2);

	int linex = 1920 / 2 - 190 * 1.8 / 2;
	int liney = 1080 / 2 - 50;

	sf::Event event;


	Button goBack(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	goBack.setPosition(linex * setting.xScale, (liney + 300) * setting.yScale);
	goBack.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	goBack.setTitle("Back");
	goBack.setSoundVolume(setting.SoundVolume);


	Button play(window, blockPressed, block, offButton, clickBuff, switchBuff, font);
	play.setPosition(linex * setting.xScale, (liney + 200) * setting.yScale);
	play.setScale(1.8 * setting.xScale, 1 * setting.yScale);
	play.setTitle("play");
	play.setSoundVolume(setting.SoundVolume);

	Rounds rounds(window, roundsOff, roundsOn, VolumePointner, VolumeSliderLine, clickBuff);
	rounds.setFont(font);
	rounds.setBoxPosition((linex + 100) * setting.xScale, 300 * setting.yScale);
	rounds.setSliderPosition(linex, (500 - 30));
	rounds.setSize(60);
	rounds.setTextPosition((linex + 120) * setting.xScale, (300 + 20) * setting.yScale);
	rounds.setSoundVolume(setting.SoundVolume);
	rounds.setString("");
	rounds.addNumber('2');
	rounds.addNumber('0');
	rounds.setScale(setting.xScale, setting.yScale);
	rounds.setColor(sf::Color::Black);

	sf::Sprite nicknameInfoS;
	nicknameInfoS.setTexture(nicknameInfo);
	nicknameInfoS.setPosition(60 * setting.xScale, 25 * setting.yScale);


	sf::Sprite controlsInfoS;
	controlsInfoS.setTexture(controlsInfo);
	controlsInfoS.setPosition(400 * setting.xScale, 1 * setting.yScale);

	sf::Sprite roundsInfoS;
	roundsInfoS.setTexture(roundsInfo);
	roundsInfoS.setPosition(800 * setting.xScale, 120 * setting.yScale);

	setPlayer::clearForbidden();
	int row = 90;
	int off = 100;
	int col = 150;

	std::vector<setPlayer> players;
	players.push_back(std::move(setPlayer(window, nickNameOnBlue, nickNameOffBlue, controlOnBlue, controlOffBlue, checkOnBlue, checkOff, clickBuff, font2, "player1")));
	players.back().setPosition(col, row + off);
	players.back().setSoundVolume(setting.SoundVolume);
	players.back().setColor(sf::Color(53, 186, 243, 255));

	players.push_back(std::move(setPlayer(window, nickNameOnGreen, nickNameOffGreen, controlOnGreen, controlOffGreen, checkOnGreen, checkOff, clickBuff, font2, "player2")));
	players.back().setPosition(col, row * 2 + off);
	players.back().setSoundVolume(setting.SoundVolume);
	players.back().setColor(sf::Color(136, 224, 96, 255));

	players.push_back(std::move(setPlayer(window, nickNameOnYellow, nickNameOffYellow, controlOnYellow, controlOffYellow, checkOnYellow, checkOff, clickBuff, font2, "player3")));
	players.back().setPosition(col, row * 3 + off);
	players.back().setSoundVolume(setting.SoundVolume);
	players.back().setColor(sf::Color(255, 217, 72, 255));

	players.push_back(std::move(setPlayer(window, nickNameOnRed, nickNameOffRed, controlOnRed, controlOffRed, checkOnRed, checkOff, clickBuff, font2, "player4")));
	players.back().setPosition(col, row * 4 + off);
	players.back().setSoundVolume(setting.SoundVolume);
	players.back().setColor(sf::Color(250, 129, 50, 255));

	players.push_back(std::move(setPlayer(window, nickNameOnPink, nickNameOffPink, controlOnPink, controlOffPink, checkOnPink, checkOff, clickBuff, font2, "player5")));
	players.back().setPosition(col, row * 5 + off);
	players.back().setSoundVolume(setting.SoundVolume);
	players.back().setColor(sf::Color(255, 99, 255, 255));

	players.push_back(std::move(setPlayer(window, nickNameOnPurple, nickNameOffPurple, controlOnPurple, controlOffPurple, checkOnPurple, checkOff, clickBuff, font2, "player6")));
	players.back().setPosition(col, row * 6 + off);
	players.back().setSoundVolume(setting.SoundVolume);
	players.back().setColor(sf::Color(186, 5, 255, 255));

	players.push_back(std::move(setPlayer(window, nickNameOnOliwe, nickNameOffOliwe, controlOnOliwe, controlOffOliwe, checkOnOliwe, checkOff, clickBuff, font2, "player7")));
	players.back().setPosition(col, row * 7 + off);
	players.back().setSoundVolume(setting.SoundVolume);
	players.back().setColor(sf::Color(141, 141, 0, 255));

	players.push_back(std::move(setPlayer(window, nickNameOnSkye, nickNameOffSkye, controlOnSkye, controlOffSkye, checkOnSkye, checkOff, clickBuff, font2, "player8")));
	players.back().setPosition(col, row * 8 + off);
	players.back().setSoundVolume(setting.SoundVolume);
	players.back().setColor(sf::Color(25, 255, 255, 255));

	setPlayer * activated = &players[0];
	bool alertFlag = false;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (!alertFlag)
			{
				goBack.checkState();
				play.checkState();
				players[1].checkState();
				players[2].checkState();
				players[3].checkState();
				players[4].checkState();
				players[5].checkState();
				players[6].checkState();
				players[7].checkState();
				players[0].checkState();
				rounds.checkState();
				if (setPlayer::getFocus() && event.type == sf::Event::KeyPressed)
				{
					activated->setChar(event.key.code);
				}
				if (rounds.getFocuse())
				{
					if (event.type == sf::Event::TextEntered)
					{
						rounds.addNumber(static_cast<char>(event.text.unicode));
					}
					else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
					{
						rounds.updateText();
						rounds.disActivate();
					}
				}
				if (goBack.buttonFunction())
					return st::mainMenu;
				else if (play.buttonFunction())
				{
					bool empty = false;
					bool emptyControlls = false;
					setting.rounds = rounds.getNumber();
					for (auto& player : players)
					{
						if (player.getActive())
						{
							std::string str = player.getNickname();
							sf::Keyboard::Key right = player.getRightKey();
							sf::Keyboard::Key left = player.getLeftKey();
							sf::Color color = player.getColor();
							if (str.empty() || right == sf::Keyboard::Unknown || left == sf::Keyboard::Unknown)
							{
								if (str.empty())
									empty = true;
								if (right == sf::Keyboard::Unknown || left == sf::Keyboard::Unknown)
									emptyControlls = true;
								break;
							}
							setting.playersSettings.push_back({ color, str, left, right });
						}
					}
					if (setting.playersSettings.size() < 2 || empty || emptyControlls)
					{
						std::string msg;
						if (setting.playersSettings.size() < 2) msg += "minimum two players required \nto start the game\n";
						if (empty)  msg += "you didn't choose your nickname\n";
						if (emptyControlls) msg += "you didn't choose your controls\n";
						alert.setText(msg);
						alertFlag = true;
						alert.show();
						setting.playersSettings.clear();
					}
					else
						return st::playNormalGame;
					/*for (auto& x : setting.playersSettings)
				{
					std::cout << x.nickname << "  hh " << int(x.left) << "  hh " << int(x.right) << "\n\n";
				}*/

				}
				else if (players[1].function())
				{
					activated = &players[1];
				}
				else if (players[2].function())
				{
					activated = &players[2];
				}
				else if (players[3].function())
				{
					activated = &players[3];
				}
				else if (players[4].function())
				{
					activated = &players[4];
				}
				else if (players[5].function())
				{
					activated = &players[5];
				}
				else if (players[6].function())
				{
					activated = &players[6];
				}
				else if (players[7].function())
				{
					activated = &players[7];
				}
				else if (players[0].function())
				{
					activated = &players[0];
				}
				else if (rounds.function());
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
		play.draw();
		for (auto& x : players)
			x.draw();
		rounds.draw();
		alert.draw();
		window.draw(nicknameInfoS);
		window.draw(controlsInfoS);
		window.draw(roundsInfoS);
		window.display();
	}
}
Menu::~Menu()
{
}
