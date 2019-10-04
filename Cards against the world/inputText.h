#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include"Settings.h"
extern Settings setting;
class Mark {
private:

	std::size_t begin;
	sf::Text& text;
	std::size_t lastUpdate;
	std::size_t end;
	sf::RectangleShape box;
	sf::Vector2f dim;
	sf::RenderWindow& window;
public:
	bool activated;
	Mark(sf::Text& ref, sf::RenderWindow& win) : text(ref), window(win) { box.setFillColor(sf::Color::Blue);  activated = false; };
	void setHeigh(float heigh_) { dim.y = heigh_; }
	void clear() { dim.x = 0; box.setSize(dim); activated = false; };
	void hook(std::size_t b) { begin = b, lastUpdate = b; box.setPosition(text.findCharacterPos(b)); }
	void update(std::size_t e) {
		if (lastUpdate == e) return;
		activated = true;
		end = e;
		dim.x = text.findCharacterPos(end).x - box.getPosition().x;
		box.setSize(dim);
		lastUpdate = end;
	}
	void draw() { if (activated)window.draw(box); }
	std::size_t getBeg() { return begin; }
	std::size_t getEnd() { return end; }
};
class inputText
{
private:
	enum buttonState { isPressed, isNotPressed };
	enum positionState { isOn, isNotOn };
	Mark mark;
	sf::Text textOutput;
	sf::RectangleShape coursor;
	sf::RenderWindow& window;
	std::size_t coursorPosition;
	std::size_t coursorLastPosition;
	sf::Vector2f size;
	sf::Vector2f pos;
	int limit;
	sf::Sound click;
	bool focused;
	bool blink;
	bool clicked;
	bool activated;
	sf::Clock timer;
	sf::String lastChar;
	sf::String text;
	positionState positionSt;
	positionState lastPositionSt;
	buttonState buttonSt;
	buttonState lastButtonSt;
	bool isOnButton();
	void checkCoursorPosition();
	void setCoursorPosition(std::size_t i);
	void checkBlink();
	void findMark();
	int checkSpecialCharacters(wchar_t t);
	void setTextPosition(int x, int y) { textOutput.setPosition(x, y);  textOutput.setScale(setting.xScale, setting.yScale); };
	wchar_t translate(sf::Event::KeyEvent key);
public:
	inputText(sf::RenderWindow& win, sf::SoundBuffer& click, int charLimit);
	void setBounds(sf::Vector2f pos_, sf::Vector2f size_) { pos = pos_; size = size_; }
	bool function(bool clear = false);
	void checkState();
	bool addChar(sf::Event::KeyEvent h);
	void setString(sf::String y) { text = y; textOutput.setString(y); }
	void draw() { mark.draw(); window.draw(textOutput); if (blink) window.draw(coursor); }
	void setPosition(int x, int y);
	void setColor(sf::Color x = sf::Color::Black) { textOutput.setFillColor(x); }
	void setFont(sf::Font& x) { textOutput.setFont(x); }
	void setSize(int x) {
		textOutput.setCharacterSize(x); coursor.setSize(sf::Vector2f(2, x)); coursor.setFillColor(sf::Color(128, 128, 128, 255)); mark.setHeigh(x);
	}
	sf::String& getText() { return text; }
	void clear() { text.clear(); textOutput.setString(""); }
	void setSoundVolume(double vol) { click.setVolume(vol * 100); }
	~inputText();

};

