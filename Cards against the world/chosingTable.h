#pragma once
#include <SFML/Graphics.hpp>
#include "card.h"
#include "Deck.h"

class chosingTable
{
private:
	struct data
	{
		card card_;
		int playerId;
		int related;
	};
	sf::RenderWindow& window;
	std::vector<sf::Vector2f> normalPositions;
	std::vector<sf::Vector2i> doublePositions;
	std::vector<data> slots;
	int chosen;
	int numberOfCards;
	bool doubl;
	bool hide = false;
	void resetChosen() { resetOne(chosen); resetOne(slots[chosen].related); chosen = -1; }
	void resetOne(int x) { if (x != -1) slots[x].card_.resetChosen(); }
public:
	chosingTable(sf::RenderWindow& win, int numberOfCards);
	~chosingTable();
	int getChosenPlayerId(); // get player id of chosen cards
	void function();
	void draw();
	void setDrawable(bool t) { hide = !t; }
	void setDouble(bool doubl_) { doubl = doubl_; }
	bool init(std::vector<int> initCards, bool doubleMode);
};

