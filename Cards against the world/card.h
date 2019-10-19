#pragma once
#include <SFML/Graphics.hpp>
class card : public sf::Drawable
{
public:
	enum kind { black, white };
private:
	static sf::Texture whiteTempl;
	static sf::Texture blackTempl;
	static sf::Font font;
	int id;
	int offset;
	float charSize;
	card::kind kind_;
	sf::Sprite back;
	sf::Text text;
	sf::Text test;
	sf::Text number; //id
	sf::String string;
	void compute();
public:
	void setId(int id);
	void setPosition(int x, int y, int offset);
	void setTextUtf8(std::string str);//get
	void setCharSize(float size) { charSize = size; test.setCharacterSize(charSize); text.setCharacterSize(charSize); }
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(back); target.draw(text); target.draw(number); }
	static void setWhiteTexture(std::string path) { if (!whiteTempl.loadFromFile(path)) throw std::exception("Cannot load cart template\n"); }
	static void setBlackTexture(std::string path) { if (!blackTempl.loadFromFile(path)) throw std::exception("Cannot load cart template\n"); }
	static void setFont(std::string path) { if (!font.loadFromFile(path)) throw std::exception("Cannot load cart template\n"); }
	card(card::kind kind__);
	~card();
};

