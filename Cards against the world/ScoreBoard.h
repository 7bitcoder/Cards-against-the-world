#pragma once
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
struct data
{
	int score;
	std::string nickname;
	bool activte;
	sf::Text text;
	sf::Clock timer;
};
class ScoreBoard
{
private:
	std::vector<int> positions;
	float diff;
	int posx;
	int posy;
	int charSize;
	sf::RenderWindow& window;
	std::vector<data> dates;
	std::vector<std::vector<data>::iterator> rank;
	std::queue<std::vector<std::vector<data>::iterator>> states;
	std::queue<int> main;
	sf::Color base;
	float vel;
	int chosing = 0;
	bool updating;
	void move(int id);
public:
	void update();
	void updateScore(int id);
	void draw();
	void setColor(sf::Color col) { base = col; }
	void setChosing(int id) { dates[chosing].text.setFillColor(base); chosing = id; dates[id].text.setFillColor(sf::Color::Yellow); }
	void setPosition(int x, int y, std::map<int, sf::String>& players, sf::Font& font);
	ScoreBoard(sf::RenderWindow& win, int size, float vel_, int numberOfPlayers);
	~ScoreBoard();
};

