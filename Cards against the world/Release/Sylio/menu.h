#include"settings.h"
#include"Music.h"
#include"Button.h"
#include<string>
#include<iostream>
#include<filesystem>
#include"list.h"
#include"Slider.h"
#include"Background.h"
#include "PopAlert.h"
#include "inputText.h"
#include "setPlayer.h"
#include "Rounds.h"



class Menu
{
private:
	Background background;
	sf::RenderWindow& window;
	std::string& version;
	sf::Font font;
	sf::Font font2;
	sf::SoundBuffer clickBuff;
	sf::SoundBuffer switchBuff;
	sf::Texture back;
	sf::Texture base;
	//sf::Sprite background;
	sf::Texture mar;
	sf::Texture block;
	sf::Texture blockPressed;
	sf::Texture offButton;
	sf::Texture VolumeSliderLine;
	sf::Texture VolumePointner;
	sf::Texture listBlock;
	sf::Texture whiteBox;
	sf::Texture checkOff;
	sf::Shader backgroundShader;

	sf::Texture nickNameOffGreen;
	sf::Texture nickNameOnGreen;
	sf::Texture controlOnGreen;
	sf::Texture controlOffGreen;
	sf::Texture checkOnGreen;

	sf::Texture nickNameOffBlue;
	sf::Texture nickNameOnBlue;
	sf::Texture controlOnBlue;
	sf::Texture controlOffBlue;
	sf::Texture checkOnBlue;

	sf::Texture nickNameOffYellow;
	sf::Texture nickNameOnYellow;
	sf::Texture controlOnYellow;
	sf::Texture controlOffYellow;
	sf::Texture checkOnYellow;

	sf::Texture nickNameOffRed;
	sf::Texture nickNameOnRed;
	sf::Texture controlOnRed;
	sf::Texture controlOffRed;
	sf::Texture checkOnRed;

	sf::Texture nickNameOffPink;
	sf::Texture nickNameOnPink;
	sf::Texture controlOnPink;
	sf::Texture controlOffPink;
	sf::Texture checkOnPink;

	sf::Texture nickNameOffPurple;
	sf::Texture nickNameOnPurple;
	sf::Texture controlOnPurple;
	sf::Texture controlOffPurple;
	sf::Texture checkOnPurple;

	sf::Texture nickNameOffOliwe;
	sf::Texture nickNameOnOliwe;
	sf::Texture controlOnOliwe;
	sf::Texture controlOffOliwe;
	sf::Texture checkOnOliwe;

	sf::Texture nickNameOffSkye;
	sf::Texture nickNameOnSkye;
	sf::Texture controlOnSkye;
	sf::Texture controlOffSkye;
	sf::Texture checkOnSkye;

	sf::Texture roundsOff;
	sf::Texture roundsOn;

	sf::Texture	nicknameInfo;

	sf::Texture	controlsInfo;

	sf::Texture	roundsInfo;

public:
	Menu(sf::RenderWindow& win, std::string& ver);
	st mainMenuUpdate();
	st settingsUupdate();
	st pauseUpdate();
	st normalGameUpdate();
	Background& getBack() { return background; }
	~Menu();
};

