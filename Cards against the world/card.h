#pragma once
#include <SFML/Graphics.hpp>
class card
{
private:
	enum kind{black, white};
	static sf::RenderWindow& window;
	static sf::Texture templ;
	static sf::Font font;
	int id;
	kind kind_;
	sf::Sprite back;
	sf::Text text;
	sf::Text number; //id
	sf::String string;
	void compute();
public:
	void setId(int id);
	void setTextUtf8(std::string str);//get
	void draw() { window.draw(back); window.draw(text); }
	card();
	~card();
};

