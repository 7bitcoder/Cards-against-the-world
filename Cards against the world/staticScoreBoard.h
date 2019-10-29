#pragma once
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
struct data: public sf::Drawable
{
	sf::Sprite check;
	bool checked = false;
	int id;
	int score;
	std::string nickname;
	sf::Text text;
	data() {};
	data(int id_, int sco, std::string str) :id(id_), score(sco), nickname(str) {};
	void draw ( sf::RenderTarget & target, sf::RenderStates  states ) const{
	if(checked)
		target.draw(check);
	target.draw(text);
	}
};
class staticScoreBoard
{
private:
	std::vector<int> positions;
	int charSize;
	sf::Sprite winner;
	sf::Sprite choserSprite;
	sf::RenderWindow& window;
	std::vector<data> dates;
	sf::Color base;
	bool isChosing = false;
	bool isLastWinner = false;
	std::vector<data>::iterator chosing;
	std::vector<data>::iterator lastWinner;
	void updateCHosingAndWinner();
public:
	static sf::Texture checkText;
	static sf::Texture choserText;
	static sf::Texture lasWinnerText;
	void updateScore(int id);
	void draw() { for (auto& x : dates) window.draw(x); if (isChosing) window.draw(choserSprite); if (isLastWinner) window.draw(winner); }
	void setColor(sf::Color col) { base = col; }
	void setChosing(int id);
	void setPosition(int x, int y);
	staticScoreBoard(sf::RenderWindow& win);
	void init(int size, std::map<int, sf::String>& players, sf::Font& font);
	void check(int id);
	void rotateMainPlayer(int id);
	void resetCheck();
	~staticScoreBoard();
};

