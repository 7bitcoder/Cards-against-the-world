#pragma once
#include<SFML/Audio.hpp>
#include<string>
#include "Settings.h"
extern Settings setting;
class Music
{
private:
	sf::Music menuMusic;
	sf::Music gameMusic;
public:
	Music();
	bool setGameMusic(std::string dir);
	void playGameMusic() { gameMusic.play(); }
	void playMenuMusic() { menuMusic.play(); }
	void stopGameMusic() { gameMusic.stop(); }
	void stopMenuMusic() { menuMusic.stop(); }
	void setVolume(double vol) { menuMusic.setVolume(vol); gameMusic.setVolume(vol); }
	double getVolume() { return menuMusic.getVolume(); }
	~Music();
};

