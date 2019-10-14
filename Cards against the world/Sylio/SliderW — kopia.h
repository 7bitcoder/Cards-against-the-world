#pragma once
#include"settings.h"
#include<SFML/Graphics.hpp>
#include"Sounds.h"
#include"Music.h"
extern Settings setting;
extern Music music;
class Slider
{
private:
	enum pointnerState { isOn = 1, isNotOn, isPressed, isNotPressed, apperedOn, isClickedOn, isClickedOut, apperedOut, notClickedOutButCoursorIsOut };
	sf::Sprite pointner;
	sf::Sprite line;
	sf::RenderWindow& window;
	sf::Sound click;
	pointnerState state;
	bool isAlredyOnButton;
	bool mouseAlredyPressed;
	bool isAlredyOut;
	bool isOnPointner();
	double oldVal;
	double actualval;
public:
	void checkState();
	bool sliderFunction();
	Slider(sf::RenderWindow& win, sf::Texture& pointner_, sf::Texture& line_, sf::SoundBuffer& click_);
	void setPosition(int x, int y, double scale);
	void draw() { window.draw(line); window.draw(pointner); };
	void setSlider(double x);
	void setSoundVolume(double v) { click.setVolume(v * 100); }
	double getValue() { return actualval; }
	~Slider();
};

