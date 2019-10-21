#include "chosingTable.h"


#include "table.h"

int chosingTable::getChosenPlayerId()
{
	return  slots[chosen].playerId;
}

void chosingTable::function()
{
	static bool full = false;
	sf::Vector2i pos = sf::Mouse::getPosition(window);
	for (int i = 0; i < slots.size(); i++) {
		if (slots[i].card_.isOn(sf::Vector2f(pos.x, pos.y))) {
			if (chosen != i) {
				if (doubl) {
					resetChosen();
					chosen = i;
					int first = i < slots[i].related ? i : slots[i].related;
					int secound = i > slots[i].related ? i : slots[i].related;
					slots[first].card_.setChosen();
					slots[secound].card_.setChosen(true);

				}
				else {
					resetOne(chosen);
					chosen = i;
					slots[i].card_.setChosen();
				}
			}
		}
	}
}

void chosingTable::draw()
{
	for (auto& x : slots)
		window.draw(x.card_);
}

chosingTable::chosingTable(sf::RenderWindow& win, int numberOfCards_) : window(win), slots(2 * numberOfCards_, { card(card::kind::white), 0, 0})
{
	for (auto& x : slots) {
		x.card_.setCharSize(20);
	}
	int beg = 250;
	int centry = 800;
	int centr = (window.getSize().x / 2) - 190 * 1.8;
	numberOfCards = numberOfCards_;
	float idff = float(numberOfCards_) / 2;
	float width = slots.front().card_.getXSize();
	float heigh = slots.front().card_.getYSize();
	float spacing = 15;
	int inRow = 5;
	for (int rows = 0; rows < 2; rows++) {
		for (int i = 0; i < inRow; i++) {
			if (normalPositions.size() == numberOfCards_)
				continue;
			normalPositions.push_back({ beg + i * (width + spacing), centry - rows * (heigh + spacing) });
		}
	}
	float doubleSpacing = 8;
	for (int rows = 0; rows < 2; rows++) {
		for (int i = 0; i < inRow; i++) {
			if (doublePositions.size() == 2 * numberOfCards_)
				continue;
//			doublePositions.push_back({ beg + i * (width + spacing), centry - rows * (heigh + spacing) });
	//		doublePositions.push_back({ beg + i * (width + spacing), centry - rows * (heigh + spacing) });
		}
	}
}
bool chosingTable::init(std::vector<int> initCards, bool doubleMode) {
	resetChosen();
/*	doubl = doubleMode;
	for (int i = 0; i < initCards.size(); i++) {
		chosen.x = i;
		replaceChosenFirst(initCards[i]);
		slots[i + rows * inRow].card_.setOffest(20);
		slots[i + rows * inRow].card_.setPosition(normalPositions.back().x, normalPositions.back().y);
	}
	chosen.x = chosen.y = -1;*/
}

chosingTable::~chosingTable()
{
}
