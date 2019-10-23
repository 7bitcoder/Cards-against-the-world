#pragma once
#include "Deck.h"
#include "card.h"
#include <vector>
#include <SFML/Graphics.hpp>
class table
{
private:
	sf::RenderWindow& window;
	std::vector<card> slots;
	sf::Vector2i chosen;
	int numberOfCards;
	bool doubl;
	void resetChosen() { resetOne(chosen.x); resetOne(chosen.y); chosen = { -1, -1 }; }
	void resetOne(int x) { if (x != -1) slots[x].resetChosen(); }
public:
	sf::Vector2i getChosenId();
	void function();
	void draw();
	void setDouble(bool doubl_) { doubl = doubl_; }
	table(sf::RenderWindow& win, int numberOfCards);
	bool init(std::vector<int> initCards);
	void replaceChosenFirst(int newId);
	void replaceChosenSecound(int newId);

	~table();
};

