#pragma once
#include "inputText.h"
#include "Slider.h"
class Rounds :public inputText, public Slider
{
private:
	int number;
	int min;
	int max;
public:
	Rounds(sf::RenderWindow& win, sf::Texture& box_, sf::Texture& boxOff_, sf::Texture& pointner_, sf::Texture& line_, sf::SoundBuffer& click_);
	int addNumber(char t);
	int getNumber();
	bool function();
	void updateText();
	void setBoxPosition(int x, int y) { inputText::setPosition(x, y); }
	void setSliderPosition(int x, int y) { Slider::setPosition(x, y, double(number - min) / double(max - min)); }
	void draw() { Slider::draw(); inputText::draw(); }
	void checkState() { Slider::checkState(); inputText::checkState(); }
	void setSoundVolume(double  vol) { inputText::setSoundVolume(vol); Slider::setSoundVolume(vol); }
	void setScale(double x, double y) { inputText::setScale(x, y); }
	~Rounds();
};

