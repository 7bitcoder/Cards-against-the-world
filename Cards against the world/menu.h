#include"settings.h"
#include"Button.h"
#include<string>
#include<iostream>
#include<filesystem>



class Menu
{
private:
	sf::Texture backgroundTexture;
	sf::Sprite background;
	sf::RenderWindow& window;
	std::string& version;
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


public:
	Menu(sf::RenderWindow& win, std::string& ver);
	st mainMenuUpdate();
	//st settingsUupdate();
	st ConnectToLobby();
	st choseGameMode();
	~Menu();
};

