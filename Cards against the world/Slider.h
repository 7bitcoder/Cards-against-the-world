#pragma once
#include"settings.h"
#include<SFML/Graphics.hpp>
#include"Sounds.h"
class Slider:public sf::RectangleShape
{
private:
	enum pointnerState { isOn = 1, isNotOn, isPressed, isNotPressed, apperedOn, isClickedOn, isClickedOut, apperedOut, notClickedOutButCoursorIsOut };
	sf::RenderWindow& window;
	sf::Sound click;
	pointnerState state;
	bool isAlredyOnButton;
	bool mouseAlredyPressed;
	bool isAlredyOut;
	int oldVal;
	int actualval;
	sf::Vector2f bounds;
	double X;
	double Step;
	int pressBegin;
	float pressScrollPos;
	void setPos() { this->setPosition(X, bounds.y - actualval*Step); }
public:
	void setStep(double s) { Step = s; }
	void checkState();
	int sliderFunction();
	void setDownBound(double down) { bounds.y = down; }
	void setUpBound(double up) { bounds.x = up; }
	Slider(sf::RenderWindow& win, sf::SoundBuffer& click_);
	void setXcoord(float x) { X = x; }
	void setSoundVolume(double v) { click.setVolume(v * 100); }
	double getValue() { return actualval; }
	void move(bool up) { actualval += up ? -1 : 1; oldVal = actualval; setPos(); }
	void reset() { this->setPosition(X, bounds.y); actualval = oldVal = 0; };
	~Slider();
};

