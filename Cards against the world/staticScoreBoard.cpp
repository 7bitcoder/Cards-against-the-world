#include "staticScoreBoard.h"
#include <algorithm>

void staticScoreBoard::updateScore(int id)
{
	for (auto it = dates.begin(); it != dates.end(); it++) {
		if (it->id == id) {
			it->score++;
			it->text.setString(it->nickname + std::to_string(it->score));
			return;
		}
	}
	
}

void staticScoreBoard::setChosing(int id)
{
	chosing->text.setFillColor(chosing->id == markId ? marked : base);
	for (auto it = dates.begin(); it != dates.end(); it++) {
		if (it->id == id)
			chosing = it;
	}
	chosing->text.setFillColor(sf::Color::Green);
}

void staticScoreBoard::setPosition(int x, int y)
{
	for (int i = 0; i < dates.size(); i++) {
		dates[i].text.setPosition(x, y + i*(1.1*charSize));
	}
}

staticScoreBoard::staticScoreBoard(sf::RenderWindow& win) : window(win) {}

void staticScoreBoard::init(int size, std::map<int, sf::String>& players, sf::Font& font)
{
	charSize = size;
	base = sf::Color::White;
	for (auto& x : players) {
		dates.emplace_back(x.first, 0, x.second + " ");
		dates.back().text.setString(dates.back().nickname + "0");
		dates.back().text.setFillColor(base);
		dates.back().text.setFont(font);
		dates.back().text.setCharacterSize(size);
	}
	sort(dates.begin(), dates.end(), [](data & x1, data & x2) { return x1.id < x2.id; });
	chosing = dates.begin();
}

void staticScoreBoard::mark(int id, sf::Color col)
{
	marked = col;
	for (auto& x : dates) {
		if (x.id == id){
			markId = id;
			x.text.setFillColor(marked);
		}
	}
}

staticScoreBoard::~staticScoreBoard()
{
}
