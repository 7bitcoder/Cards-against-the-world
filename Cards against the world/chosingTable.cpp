#include "chosingTable.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include "table.h"

int chosingTable::getChosenPlayerId()
{
	return  slots[chosen].playerId;
}
void chosingTable::update()
{
	if (!stop)
		if (hiding) {
			sf::Time diff = clock.getElapsedTime() - last;
			if (doubl) {
				dist = slots.at(0).card_.getPosition().y - defaultDouble;
				float vel = goingUpF(maxDistDouble);
				float move = vel * alpha * diff.asMilliseconds();
				for (auto& x : slots) {
					x.card_.move(0, move);
					x.nick.move(0, move);
				}
				if (slots.at(0).card_.getPosition().y > 1100) {
					stop = true;
					for (int i = 0; i < doubleHidePositions.size(); i++) {
						slots[i].card_.setPosition(doubleHidePositions.at(i).x, doubleHidePositions.at(i).y);
						slots[i].nick.setPosition(doubleHidePositions.at(i) + nickOffset);
					}
					hidden = true;
				}
			}
			else {
				dist = slots.at(0).card_.getPosition().y - defaultNormal;
				float vel = goingUpF(maxDist);
				float move = vel * alpha * diff.asMilliseconds();
				for (auto& x : slots) {
					x.card_.move(0, move);
					x.nick.move(0, move);
				}
				if (slots.at(0).card_.getPosition().y > 1100) {
					stop = true;
					for (int i = 0; i < normalHidePositions.size(); i++) {
						slots[i].card_.setPosition(normalHidePositions.at(i).x, normalHidePositions.at(i).y);
						slots[i].nick.setPosition(normalHidePositions.at(i) + nickOffset);
					}
					hidden = true;
				}
			}
		}
		else {
			sf::Time diff = clock.getElapsedTime() - last;
			if (doubl) {
				dist = slots.at(0).card_.getPosition().y - defaultDouble;
				float vel = -goingUpF(maxDistDouble);
				float move = vel * alpha * diff.asMilliseconds();
				for (auto& x : slots) {
					x.card_.move(0, move);
					x.nick.move(0, move);
				}
				if (slots.at(0).card_.getPosition().y < defaultDouble) {
					stop = true;
					for (int i = 0; i < doublePositions.size(); i++) {
						slots[i].card_.setPosition(doublePositions.at(i).x, doublePositions.at(i).y);
						slots[i].nick.setPosition(doublePositions.at(i) + nickOffset);
					}
					hidden = false;
				}
			}
			else {
				dist = slots.at(0).card_.getPosition().y - defaultNormal;
				float vel = -goingUpF(maxDist);
				float move = vel * alpha * diff.asMilliseconds();
				for (auto& x : slots) {
					x.card_.move(0, move);
					x.nick.move(0, move);
				}
				if (slots.at(0).card_.getPosition().y < defaultNormal) {
					stop = true;
					for (int i = 0; i < normalPositions.size(); i++) {
						slots[i].card_.setPosition(normalPositions.at(i).x, normalPositions.at(i).y);
						slots[i].nick.setPosition(normalPositions.at(i) + nickOffset);
					}
					hidden = false;
				}
			}
		}
}
void chosingTable::function()
{
	if (!block) {
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
						selected = true;
					}
					else {
						resetOne(chosen);
						chosen = i;
						slots[i].card_.setChosen();
						selected = true;
					}
				}
			}
		}
	}
}

void chosingTable::setChosen(int id)
{
	for (int i = 0; i < slots.size(); i++) {
		if (slots.at(i).playerId = id)
		{
			resetChosen();
			chosen = i;
			slots[i].card_.setChosen();
			if (doubl)
				slots.at(i + 1).card_.setChosen(true);
			showNicks = true;
			return;
		}
	}
}

void chosingTable::draw()
{

	int max = doubl ? numberOfCards * 2 : numberOfCards;
	for (int i = 0; i < max; i++) {
		window.draw(slots[i].card_);
		if (showNicks)
			window.draw(slots[i].nick);
	}

}
void chosingTable::init(int numberOfCards_, sf::Font & font) {
	for (int i = 0; i < 2 * numberOfCards_; i++) {
		slots.emplace_back(card::kind::white, 0, 0);
	}
	for (auto& x : slots) {
		x.card_.setCharSize(22);
		x.card_.setOffest(20);
		x.nick.setFillColor(sf::Color::Black);
		x.nick.setFont(font);
	}
	float spacing = 15;
	numberOfCards = numberOfCards_;
	float width = slots.front().card_.getXSize();
	float heigh = slots.front().card_.getYSize();
	float wide = width + 2;
	float row1 = 800;
	float row2 = 800 - spacing - heigh;
	float row3 = 800 - spacing * 2 - 2 * heigh;
	float basic = row1 - (row1 - row2) / 2;
	int centr = (window.getSize().x - 190 * 1.8) / 2;
	int inRow = 5;

	auto calculatePositions = [centr](int nmb, float width, int spacing) {
		std::vector<float> calc;
		float start = centr - ((nmb * width) + (nmb - 1) * spacing) / 2;
		for (int i = 0; i < nmb; i++) {
			calc.push_back(start + i * (width + spacing));
		}
		return calc;
	};
	std::vector<float> vec, vec2;
	switch (numberOfCards_) {
	case 3:
		vec = calculatePositions(3, width, spacing);
		normalPositions = { {vec[0],basic},{vec[1], basic},{vec[2],basic} };//UUU
		vec = calculatePositions(3, width * 2, spacing);
		doublePositions = { {vec[0],basic}, {vec[0] + wide ,basic},{vec[1], basic},{vec[1] + wide, basic},{vec[2],basic},{vec[2] + wide,basic} };///UUU
		break;
	case 4:
		vec = calculatePositions(4, width, spacing);
		normalPositions = { {vec[0],basic},{vec[1], basic},{vec[2],basic}, {vec[3],basic} };///UUUU
		vec = calculatePositions(2, width * 2, spacing);
		doublePositions = { {vec[0],row2} , {vec[0] + wide ,row2},{vec[1], row2}, {vec[1] + wide, row2},
		{vec[0],row1}, {vec[0] + wide ,row1},{vec[1],row1}, {vec[1] + wide,row1} };///UU
		break;																		 ///UU
	case 5:
		vec2 = calculatePositions(5, width, spacing);
		normalPositions = { {vec2[0],basic },{vec2[1], basic},{vec2[2],basic}, {vec2[3],basic}, {vec2[4],basic} };///UUUUU
		vec2 = calculatePositions(3, width * 2, spacing);
		vec = calculatePositions(2, width * 2, spacing);
		doublePositions = { {vec[0],row2},{vec[0] + wide ,row2},{vec[1], row2},{vec[1] + wide, row2},
		{vec2[0],row1}, { vec2[0] + wide,row1 },{vec2[1],row1},{vec2[1] + wide ,row1},{vec2[2],row1}, {vec2[2] + wide,row1} };//UU
		break;																													 //UUU
	case 6:
		vec = calculatePositions(6, width, spacing);
		normalPositions = { {vec[0],basic},{vec[1], basic},{vec[2],basic}, {vec[3],basic},{vec[4],basic},{vec[5],basic} };//UUUUUU
		vec = calculatePositions(3, width * 2, spacing);
		doublePositions = { {vec[0],row2},{vec[0] + wide,row2},{vec[1], row2},{vec[1] + wide, row2},{vec[2], row2},{vec[2] + wide, row2},
		{vec[0],row1},{vec[0] + wide,row1},{vec[1],row1},{vec[1] + wide,row1},{vec[2], row1},{vec[2] + wide, row1} };//UUU
		break;																										   ///UUU	
	case 7:
		vec2 = calculatePositions(4, width, spacing);
		vec = calculatePositions(3, width, spacing);																				  //UUU
		normalPositions = { {vec[0],row2},{vec[1], row2},{vec[2],row2}, {vec2[0],row1},{vec2[1],row1},{vec2[2],row1},{vec2[3],row1} };//UUUU
		vec2 = calculatePositions(3, width * 2, spacing);
		vec = calculatePositions(1, width * 2, spacing);
		doublePositions = { {vec[0],row3},{vec[0] + wide,row3},{vec2[0], row2},{vec2[0] + wide, row2},{vec2[1], row2},{vec2[1] + wide, row2},					 /// U
		{vec2[2], row2},{vec2[2] + wide, row2},{vec2[0],row1},{vec2[0] + wide,row1},{vec2[1],row1},{vec2[1] + wide,row1},{vec2[2], row1},{vec2[2] + wide, row1} };//UUU
		break;																																						 ///UUU
	}
	chosen = -1;
	defaultNormal = normalPositions.at(0).y;
	maxDist = 1100 - defaultNormal;
	for (auto& x : normalPositions) {
		normalHidePositions.push_back({ x.x, x.y + maxDist });
	}
	defaultDouble = doublePositions.at(0).y;
	maxDistDouble = 1100 - defaultDouble;
	for (auto& x : doublePositions) {
		doubleHidePositions.push_back({ x.x, x.y + maxDistDouble });
	}
	for (auto& x : slots)
		x.card_.setIndexOffest({ 164, 222 }, 20);
}

void chosingTable::hideF(bool hid)
{
	hiding = hid;
	clock.restart();
	last = clock.getElapsedTime();
	stop = false;
	uint8_t val = hid ? 128 : 255;
	for (int i = 0; i < slots.size(); i++) {
		if (!hid && chosen != -1 && (i == chosen || (doubl && i == slots.at(chosen).related))) {
			slots.at(i).card_.setBacgrounded(200);
		}
		else
			slots.at(i).card_.setBacgrounded(val);
	}
}

chosingTable::chosingTable(sf::RenderWindow & win, Deck & deck_) :deck(deck_), window(win), chosen(-1), doubl(false)
{
	std::srand(std::time(nullptr));
}
bool chosingTable::setCards(std::vector<sf::Vector2i> & initCards, std::vector<sf::String>& nicks, bool doubleMode) {
	resetChosen();
	doubl = doubleMode;
	if (doubl) {
		for (int i = 0; i < numberOfCards * 2; i++) {
			slots[i].card_.setId(initCards[i].x);
			slots[i].playerId = initCards[i].y;
			slots[i].related = i % 2 ? i - 1 : i + 1;
			slots[i].card_.setTextUtf8(deck.getCard(initCards[i].x, false));
			slots[i].card_.setPosition(doubleHidePositions[i].x, doubleHidePositions[i].y);
			slots[i].nick.setPosition(doubleHidePositions[i] + nickOffset);
			slots[i].nick.setString(nicks[i]);
		}
	}
	else {
		for (int i = 0; i < numberOfCards; i++) {
			slots[i].card_.setId(initCards[i].x);
			slots[i].playerId = initCards[i].y;
			slots[i].card_.setTextUtf8(deck.getCard(initCards[i].x, false));
			slots[i].card_.setPosition(normalHidePositions[i].x, normalHidePositions[i].y);
			slots[i].nick.setPosition(normalHidePositions[i] + nickOffset);
			slots[i].nick.setString(nicks[i]);
		}
	}
	chosen = -1;
	focused = slots.end();
	return true;
}

void chosingTable::choseRandom()
{
	resetChosen();
	std::vector<int> cardsVectorPos(doubl ? numberOfCards * 2 : numberOfCards);
	int pos = std::rand() % cardsVectorPos.size();
	chosen = pos;
	slots[pos].card_.setChosen();//first
	selected = true;
	slots[slots[pos].related].card_.setChosen();
}

chosingTable::~chosingTable()
{
}
