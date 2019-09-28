#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>


enum st { play = 1, settings, quit, mainMenu, choseGameMode, connectToLobby };
class Settings
{
public:
	bool newLobby;
	double xScale;
	double yScale;
	bool pause = false;
	double SoundVolume;
	double MusicVolume;
	int rounds;
	std::string defaultMusic;
	int posDefaultMusic;
	Settings();
	void save();
	~Settings();
};

