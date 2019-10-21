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

chosingTable::chosingTable(sf::RenderWindow & win, int numberOfCards_) : window(win), slots(2 * numberOfCards_, { card(card::kind::white), 0, 0 })
{
	for (auto& x : slots) {
		x.card_.setCharSize(20);
		x.card_.setOffest(20);
	}
	float spacing = 8;

	numberOfCards = numberOfCards_;
	float width = slots.front().card_.getXSize();
	float heigh = slots.front().card_.getYSize();
	float row1 = 800;
	float row2 = 800 - spacing - heigh;
	float row3 = 800 - spacing * 2 - 2 * heigh;
	int centr = (window.getSize().x - 190 * 1.8) / 2;
	int inRow = 5;

	auto calculatePositions = [centr](int nmb, float width, int spacing) {
		std::vector<float> calc;
		float start = centr - ((nmb * width) + (nmb - 1) * spacing) / 2 - width / 2;
		for (int i = 0; i < nmb; i++) {
			calc.push_back(start + i * (width + spacing));
		}
		return calc;
	};
	std::vector<float> vec, vec2;
	switch (numberOfCards_) {
	case 3:
		vec = calculatePositions(3, width, spacing);
		row1 /= 2;
		normalPositions = { {vec[0],row1},{vec[1], row1},{vec[2],row1} };//UUU
		vec = calculatePositions(3, width * 2, spacing);
		doublePositions = { {vec[0],row1}, {vec[0] + width ,row1},{vec[1], row1},{vec[1] + width, row1},{vec[2],row1},{vec[2] + width,row1} };///UUU
		break;
	case 4:
		vec = calculatePositions(4, width, spacing);
		normalPositions = { {vec[0],row1 / 2},{vec[1], row1 / 2},{vec[2],row1 / 2}, {vec[3],row1 / 2} };///UUUU
		vec = calculatePositions(2, width * 2, spacing);
		doublePositions = { {vec[0],row2} , {vec[0] + width ,row2},{vec[1], row2}, {vec[1] + width, row2},
		{vec[0],row1}, {vec[0] + width ,row1},{vec[1],row1}, {vec[1] + width,row1} };///UU
		break;																		 ///UU
	case 5:
		vec2 = calculatePositions(5, width, spacing);
		normalPositions = { {vec[0],row1 / 2 },{vec[1], row1 / 2},{vec2[2],row1 / 2}, {vec2[3],row1 / 2}, {vec2[3],row1 / 2} };///UUUUU
		vec2 = calculatePositions(3, width * 2, spacing);
		vec = calculatePositions(2, width * 2, spacing);
		doublePositions = { {vec[0],row2},{vec[0] + width ,row2},{vec[1], row2},{vec[1] + width, row2},
		{vec2[0],row1}, { vec2[0] + width,row1 },{vec2[1],row1},{vec2[1] + width ,row1},{vec2[2],row1}, {vec2[2] + width,row1} };//UU
		break;																													 //UUU
	case 6:
		vec = calculatePositions(6, width, spacing);
		normalPositions = { {vec[0],row1 / 2},{vec[1], row1 / 2},{vec[2],row1 / 2}, {vec[3],row1 / 2},{vec[3],row1 / 2},{vec[3],row1 / 2} };//UUUUUU
		vec = calculatePositions(2, width * 2, spacing);
		doublePositions = { {vec[0],row2},{vec[0] + width,row2},{vec[1], row2},{vec[1] + width, row2},{vec[2], row2},{vec[2] + width, row2},
		{vec[0],row1},{vec[0] + width,row1},{vec[1],row1},{vec[1] + width,row1},{vec[2], row1},{vec[2] + width, row1} };//UUU
		break;																										   ///UUU	
	case 7:
		vec2 = calculatePositions(4, width, spacing);
		vec = calculatePositions(3, width, spacing);																				  //UUU
		normalPositions = { {vec[0],row2},{vec[1], row2},{vec[2],row2}, {vec2[0],row1},{vec2[1],row1},{vec2[2],row1},{vec2[3],row1} };//UUUU
		vec2 = calculatePositions(3, width * 2, spacing);
		vec = calculatePositions(1, width * 2, spacing);
		doublePositions = { {vec[0],row3},{vec[0] + width,row3},{vec2[0], row2},{vec2[0] + width, row2},{vec2[1], row2},{vec2[1] + width, row2},					 /// U
		{vec2[2], row2},{vec2[2] + width, row2},{vec2[0],row1},{vec2[0] + width,row1},{vec2[1],row1},{vec2[1] + width,row1},{vec2[2], row1},{vec2[2] + width, row1} };//UUU
		break;																																						 ///UUU
	}
}
bool chosingTable::init(std::vector<sf::Vector2i> initCards, bool doubleMode) {
	resetChosen();
	doubl = doubleMode;
	if (doubl) {
		for (int i = 0; i < numberOfCards * 2; i += 2) {
			slots[i].card_.setId(initCards[i].x);
			slots[i].playerId = initCards[i].y;
			slots[i].related = i % 2 ? i + 1 : i - 1;
			slots[i].card_.setTextUtf8(deck.getCard(initCards[i].x, false));
			slots[i].card_.setPosition(doublePositions[i].x, doublePositions[i].y);
		}
	}
	else {
		for (int i = 0; i < numberOfCards; i++) {
			slots[i].card_.setId(initCards[i].x);
			slots[i].playerId = initCards[i].y;
			slots[i].card_.setTextUtf8(deck.getCard(initCards[i].x, false));
			slots[i].card_.setPosition(normalPositions[i].x, normalPositions[i].y);
		}
	}
	chosen = -1;
	return true;
}

chosingTable::~chosingTable()
{
}
