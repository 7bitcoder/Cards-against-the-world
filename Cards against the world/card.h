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
	bool chosen;
	int offset;
	float charSize;
	card::kind kind_;
	sf::Sprite back;
	sf::Text text;
	sf::Text test;
	sf::Text chose;
	sf::Text number; //id
	sf::String string;
	void compute();
public:
	void setId(int id);
	int getId() { return id; }
	bool isOn(sf::Vector2f pos);
	void setPosition(int x, int y, int offset);
	void setTextUtf8(std::string str);//get
	float getXSize() { return back.getGlobalBounds().width; }
	float getYSize() { return back.getGlobalBounds().height; }
	bool isChosen() { return chosen; }
	void setChosen(bool secound = false);
	void resetChosen();
	void setCharSize(float size) { charSize = size; test.setCharacterSize(charSize); text.setCharacterSize(charSize); }
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(back); target.draw(text); target.draw(number); target.draw(chose); }
	static void setWhiteTexture(std::string path) { if (!whiteTempl.loadFromFile(path)) throw std::exception("Cannot load cart template\n"); }
	static void setBlackTexture(std::string path) { if (!blackTempl.loadFromFile(path)) throw std::exception("Cannot load cart template\n"); }
	static void setFont(std::string path) { if (!font.loadFromFile(path)) throw std::exception("Cannot load cart template\n"); }
	card(card::kind kind__);
	~card();
};

