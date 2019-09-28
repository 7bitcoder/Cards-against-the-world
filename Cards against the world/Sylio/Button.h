#pragma once
#include<SFML/Graphics.hpp>
#include"Sounds.h"
#include<iostream>

extern Settings setting;

class Button : public sf::Sprite
{
private:
	enum buttonState { isPressed, isNotPressed };
	enum positionState { isOn, isNotOn };
	std::string text;
	sf::Texture& pressed;
	sf::Texture& released;
	sf::Texture& offButton;
	sf::RenderWindow& window;
	sf::Sound click;
	sf::Sound switchS;
	sf::Text title;
	sf::Font& font;
	positionState positionSt;
	positionState lastPositionSt;
	buttonState buttonSt;
	buttonState lastButtonSt;
	bool clicked;
public:
	Button(sf::RenderWindow& win, sf::Texture& pres, sf::Texture& rel, sf::Texture& off, sf::SoundBuffer& click_, sf::SoundBuffer& switch_, sf::Font& font_);
	void clickSound();
	void switchSound();
	bool buttonFunction();//return true if clicked out
	bool settingsListFunction();
	bool empty() { return text.empty(); }
	void setTitle(std::string title_);
	void checkState();
	void setColor(sf::Color x) { title.setFillColor(x); };
	void setSoundVolume(double v) { switchS.setVolume(30 * v); click.setVolume(v * 100); }
	std::string& getString() { return text; }
	void draw() { window.draw(*this); window.draw(title); }
	void normalization() { this->setTexture(offButton); }
	bool isOnButton();
	~Button();
};

