#pragma once
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
struct data
{
	sf::Sprite check;
	bool checked = false;
	bool chosen = false;
	bool lastWinner = false;
	int id;
	int score;
	std::string nickname;
	sf::Text text;
	data() {};
	data(int id_, int sco, std::string str) :id(id_), score(sco), nickname(str) {};
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
	sf::Color marked;
	int markId;
	std::vector<data>::iterator chosing;
public:
	static sf::Texture checkText;
	static sf::Texture choserText;
	static sf::Texture lasWinnerText;
	void updateScore(int id);
	void draw() { for (auto& x : dates) window.draw(x.text); }
	void setColor(sf::Color col) { base = col; }
	void setChosing(int id);
	void setPosition(int x, int y);
	staticScoreBoard(sf::RenderWindow& win);
	void init(int size, std::map<int, sf::String>& players, sf::Font& font);
	void mark(int id, sf::Color col);
	~staticScoreBoard();
};

