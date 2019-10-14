#pragma once
#include "inputText.h"
#include "SliderW.h"
class Rounds : public SliderW
{
private:
	sf::RenderWindow& window;
	sf::Text nmb;
	sf::Sprite back;
	int number;
	int min;
	int max;
public:
	Rounds(sf::RenderWindow& win, sf::Texture& box_, sf::Texture& pointner_, sf::Texture& line_, sf::SoundBuffer& click_);
	int getNumber();
	bool function();
	void setPosition(int x, int y) { nmb.setPosition(x + 20, y + 20); back.setPosition(x, y); }
	void setSliderPosition(int x, int y) { SliderW::setPosition(x, y, double(number - min) / double(max - min)); }
	void draw() { SliderW::draw(); window.draw(back); window.draw(nmb); }
	void checkState() { SliderW::checkState(); }
	void setSoundVolume(double  vol) { SliderW::setSoundVolume(vol); }
	void setScale(float x, float y) { nmb.setScale(x, y); back.setScale(x, y); }
	void setFont(sf::Font & font) { nmb.setFont(font); nmb.setFillColor(sf::Color::Black); }
	~Rounds();
};

