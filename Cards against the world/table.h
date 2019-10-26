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
	bool selected = false;//did you select card
	bool hide;
	void resetOne(int x) { if (x != -1) slots[x].resetChosen(); }
public:
	void hideF(bool hid = true) { hide = hid; }
	void function();
	void draw();
	void setDouble(bool doubl_) { doubl = doubl_; }
	table(sf::RenderWindow& win);
	void init(int numberOfCards);
	bool setCards(std::vector<int> initCards);
	int getFirst() { return slots[chosen.x].getId(); }
	int getSecound() { return slots[chosen.y].getId(); }
	void replaceChosenFirst(int newId);
	void replaceChosenSecound(int newId);
	void resetChosen() { resetOne(chosen.x); resetOne(chosen.y); chosen = { -1, -1 }; selected = false; }
	bool selectedCards() { return selected; }
	void choseRandom();

	~table();
};

