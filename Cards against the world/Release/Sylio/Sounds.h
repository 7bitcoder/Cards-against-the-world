#pragma once
#ifndef SOUNDS_H
#define SOUNDS_H
#include "Settings.h"
#include<string>
#include<SFML/Audio.hpp>
extern Settings setting;
class Sounds
{
private:
	sf::SoundBuffer clickBuffer;
	sf::Sound clickSound;
	sf::SoundBuffer switchBuffer;
	sf::Sound switchSound;
public:
	Sounds(std::string click_, std::string switch_);
	void playClick() { sf::Sound(clickBuffer).play(); }
	void playSwitch() { sf::Sound(switchBuffer).play(); }
	void setVolume(float vol);
	~Sounds();
};


#endif // !SOUNDS_H

