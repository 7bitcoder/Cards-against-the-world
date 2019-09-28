#pragma once
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
#include "Player.h"
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
	int charSize;
	sf::RenderWindow& window;
	std::vector<data> dates;
	std::vector<std::vector<data>::iterator> rank;
	std::queue<std::vector<std::vector<data>::iterator>> states;
	std::queue<int> main;
	float vel;
	bool updating;
public:
	void update();
	void updateScore(int id, Player& players);
	void draw();
	void move(int id);
	void setPosition(int x, int y, std::vector<Player>& players, sf::Font& font);
	ScoreBoard( sf::RenderWindow& win, int size, float vel_);
	~ScoreBoard();
};

