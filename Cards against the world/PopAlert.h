#pragma once
#include<SFML/Graphics.hpp>
#include"Button.h"
#include"settings.h"

class PopAlert
{
private:
	sf::RenderWindow& window;
	sf::Sprite box;
	sf::Text text;
	Button ok;
	bool drawable;
public:
	PopAlert(sf::RenderWindow& win_, std::string text_, sf::Texture& box_, sf::Texture& pres, sf::Texture& rel, sf::Texture& off, sf::SoundBuffer& click_, sf::SoundBuffer& switch_, sf::Font& font_);
	void checkState() { ok.checkState(); };
	bool function() { return ok.buttonFunction(); };
	void show() { drawable = true; }
	void hide() { drawable = false; }
	void draw() {
		if (drawable) { window.draw(box); window.draw(text); ok.draw(); }
	}
	void setText(std::string text_) { text.setString(text_); }
	~PopAlert();
};

