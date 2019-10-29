#pragma once
#include "Deck.h"
#include "card.h"
#include <vector>
#include <SFML/Graphics.hpp>


class table
{
private:
	sf::Clock clock;
	sf::Time last;
	Deck& deck;
	sf::RenderWindow& window;
	std::vector<card> slots;
	sf::Vector2i chosen;
	std::vector<sf::Vector2f> positions;
	std::vector<sf::Vector2f> hidePositions;
	int numberOfCards;
	bool doubl;
	bool selected = false;//did you select card
	bool hide;
	bool hiding = false;
	bool stop = true;
	float dist;
	float defaultDouble;//default pos when not hiding
	float defaultNormal;
	float maxDistDouble;
	float maxDist;
	void resetOne(int x) { if (x != -1) slots[x].resetChosen(); }
	float goingUpF(float maxDist) { return std::sinf((dist) * ((M_PI / 2 - 0.05) / maxDist) + 0.05); }
public:
	bool end() { return stop; }
	void update();
	void hideF(bool hid = true) { hiding = hid; clock.restart(); last = clock.getElapsedTime(); stop = false; }
	void function();
	void draw();
	void setDouble(bool doubl_) { doubl = doubl_; }
	table(sf::RenderWindow & win, Deck & deck);
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

