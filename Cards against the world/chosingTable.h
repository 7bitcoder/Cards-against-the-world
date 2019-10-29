#pragma once
#include <SFML/Graphics.hpp>
#include "card.h"
#include "Deck.h"

class chosingTable//number of cards minus one player chosing one !!!!
{
public:
	struct data
	{
		card card_;
		int playerId;
		int related;
		data(card::kind car, int pl, int re) : card_(car), playerId(pl), related(re) {}
	};
private:
	sf::Clock clock;
	sf::Time last;
	Deck& deck;
	sf::RenderWindow& window;
	std::vector<sf::Vector2f> normalPositions;
	std::vector<sf::Vector2f> doublePositions;
	std::vector<sf::Vector2f> normalHidePositions;
	std::vector<sf::Vector2f> doubleHidePositions;
	std::vector<data> slots;
	std::vector<data>::iterator focused;
	bool hiding = false;
	int chosen = -1;
	int numberOfCards;
	bool selected = false;
	bool doubl;
	bool hide = false;
	float dist;
	float defaultDouble;//default pos when not hiding
	float defaultNormal;
	float maxDistDouble;
	float maxDist;
	void resetOne(int x) { if (x != -1) slots[x].card_.resetChosen(); }
	float goingUpF(float maxDist) { return std::cosf(dist * (maxDist / (M_PI / 2 - 0.01))); }
	float goingDownF(float maxDist) { return std::cosf(dist * (maxDist / (M_PI / 2 - 0.01))); }
public:
	void update();
	void resetChosen() { if (chosen != -1) { resetOne(chosen); resetOne(slots[chosen].related); this->chosen = -1; selected = false; } }
	bool selectedCards() { return selected; }
	void hideF(bool hid = true) { hiding = hid; clock.restart(); last = clock.getElapsedTime(); }
	chosingTable(sf::RenderWindow & win, Deck & deck_);
	~chosingTable();
	int getChosenPlayerId(); // get player id of chosen cards
	void function();
	void draw();
	void init(int numberOfCards_);
	void setDrawable(bool t) { hide = !t; }
	void setDouble(bool doubl_) { doubl = doubl_; }
	bool setCards(std::vector<sf::Vector2i> initCards, bool doubleMode);//vector template <card id, player id> id double then cards for one player should be togeder
	void choseRandom();
};

