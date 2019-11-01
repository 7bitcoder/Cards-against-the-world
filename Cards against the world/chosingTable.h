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
		sf::Text nick;
		int playerId;
		int related;
		data(card::kind car, int pl, int re) : card_(car), playerId(pl), related(re) {}
	};
private:
	float alpha;
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
	bool showNicks = false;
	bool hiding = false;
	bool block = false;
	int chosen = -1;
	int numberOfCards;
	bool selected = false;
	bool doubl;
	bool hidden = false;
	bool hide = false;
	bool stop = true;
	float dist;
	sf::Vector2f nickOffset;
	float defaultDouble;//default pos when not hiding
	float defaultNormal;
	float maxDistDouble;
	float maxDist;
	void resetOne(int x) { if (x != -1) { slots[x].card_.resetChosen(); uint8_t val = hiding ? 128 : 255; slots.at(x).card_.setBacgrounded(val); } }
	float goingUpF(float maxDist) { return std::sinf((dist) * ((M_PI / 2 - 0.05) / maxDist) + 0.05); }
	float goingDownF(float maxDist) { return std::sinf(dist * (maxDist / (M_PI / 2))); }
public:
	void setNickOffset(sf::Vector2f of, int size) { nickOffset = of; for (auto& x : slots) x.nick.setCharacterSize(size); }
	void hideNicks() { showNicks = false; }
	void showNick() { showNicks = true; }
	void setBlock(bool val) { block = val; }
	void setAlpha(float al) { alpha = al; }
	void update();
	bool end() { return stop; }
	void resetChosen() { if (chosen != -1) { resetOne(chosen); resetOne(slots[chosen].related); this->chosen = -1; selected = false; } }
	bool selectedCards() { return selected; }
	void hideF(bool hid = true);
	chosingTable(sf::RenderWindow & win, Deck & deck_);
	~chosingTable();
	int getChosenPlayerId(); // get player id of chosen cards
	void function();
	void setChosen(int id);
	void draw();
	void init(int numberOfCards_, sf::Font & font);
	void setDrawable(bool t) { hide = !t; }
	void setDouble(bool doubl_) { doubl = doubl_; }
	bool setCards(std::vector<sf::Vector2i> & initCards, std::vector<sf::String> & nicks, bool doubleMode);//vector template <card id, player id> id double then cards for one player should be togeder
	void choseRandom();
};

