#pragma once
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
struct data
{
	int id;
	int score;
	std::string nickname;
	sf::Text text;
	data() {};
	data(int sco, std::string str, bool ac) :score(sco), nickname(str) {};
};
class staticScoreBoard
{
private:
	std::vector<int> positions;
	float diff;
	int posx;
	int posy;
	int charSize;
	sf::RenderWindow& window;
	std::vector<data> dates;
	sf::Color base;
	float vel;
	int chosing = 0;
	bool updating;
public:
	void updateScore(int id);
	void draw();
	void setColor(sf::Color col) { base = col; }
	void setChosing(int id) { dates[chosing].text.setFillColor(base); chosing = id; dates[id].text.setFillColor(sf::Color::Yellow); }
	void setPosition(int x, int y, std::map<int, sf::String>& players, sf::Font& font);
	staticScoreBoard(sf::RenderWindow& win);
	void init(int size, float vel_);
	~staticScoreBoard();
};

