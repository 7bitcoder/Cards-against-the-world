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
	int delta;
	int maxDelta;
	double origin;
	double X;
	double Step;
	double pressBegin;
	float pressScrollPos;
	void setPos() { this->setPosition(X, origin + delta*Step); }
	void setSideBarPos(double d) { this->setPosition(X, d); }
public:
	void setStep(double s) { Step = s; }
	void setMaxDelta(int m) { maxDelta = m; }
	void checkState();
	int sliderFunction();
	void setOrigin(double down) { origin = down; }
	Slider(sf::RenderWindow& win, sf::SoundBuffer& click_);
	void setXcoord(float x) { X = x; }
	void setSoundVolume(double v) { click.setVolume(v * 100); }
	double getValue() { return delta; }
	void move(bool up) { delta += up ? -1 : 1; setPos(); }
	void reset() { delta = maxDelta; setPos(); };
	~Slider();
};

