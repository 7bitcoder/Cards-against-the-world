#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
class CheckBox
{
private:
	enum buttonState { isPressed, isNotPressed };
	enum positionState { isOn, isNotOn };
	sf::Texture& activated;
	sf::Texture& disActivated;
	sf::Sprite box;
	sf::RenderWindow& window;
	sf::Sound click;
	//sf::Sound switchS;
	positionState positionSt;
	positionState lastPositionSt;
	buttonState buttonSt;
	buttonState lastButtonSt;
	bool activate;
public:
	CheckBox(sf::RenderWindow& win, sf::Texture& activated, sf::Texture& disactivated, sf::SoundBuffer& click_);
	int buttonFunction();//return true if clicked out
	void checkState();
	void setSoundVolume(double v) { click.setVolume(v * 100); }
	void draw() { window.draw(box); }
	bool getActivate() { return activate; }
	void setActivate() { activate = true; box.setTexture(activated); }
	void setPosition(int x, int y) { box.setPosition(x, y); }
	void setAlpha(int a) { box.setColor(sf::Color(255, 255, 255, a)); }
	bool isOnButton();
	void setScale(double x, double y) { box.setScale(x, y); }
	~CheckBox();
};

