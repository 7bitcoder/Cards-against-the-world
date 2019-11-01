#include "table.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
void table::update()
{
	if (!stop)
		if (hiding) {
			sf::Time diff = clock.getElapsedTime() - last;
			dist = slots.at(0).getPosition().y - defaultNormal;
			float vel = goingUpF(maxDist);
			float move = vel * alpha * diff.asMilliseconds();
			for (auto& x : slots) {
				x.move(0, move);
			}
			if (slots.at(0).getPosition().y > 1100) {
				stop = true;
				for (int i = 0; i < slots.size(); i++)
					slots.at(i).setPosition(hidePositions.at(i).x, hidePositions.at(i).y);
				hidden = true;
			}
		}
		else {
			sf::Time diff = clock.getElapsedTime() - last;
			dist = slots.at(0).getPosition().y - defaultNormal;
			float vel = -goingUpF(maxDist);
			float move = vel * alpha * diff.asMilliseconds();
			for (auto& x : slots) {
				x.move(0, move);
			}
			if (slots.at(0).getPosition().y < defaultNormal) {
				stop = true;
				for (int i = 0; i < slots.size(); i++)
					slots.at(i).setPosition(positions.at(i).x, positions.at(i).y);
			}
		}
}
void table::hideF(bool hid)
{
	hiding = hid;
	clock.restart();
	last = clock.getElapsedTime();
	stop = false;
	uint8_t val = hid ? 128 : 255;
	for (int i = 0; i < slots.size(); i++) {
		if ((i == chosen.x || i == chosen.y) && !hid)
			slots.at(i).setBacgrounded(200);
		else
			slots.at(i).setBacgrounded(val);
	}
}
void table::function()
{
	if (!block) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		for (int i = 0; i < slots.size(); i++) {
			if (slots[i].isOn(sf::Vector2f(pos.x, pos.y))) {
				if (doubl) {
					if (chosen.x != -1 && chosen.y != -1)
						resetChosen();
					if (chosen.x == -1) {
						chosen.x = i;
						slots[i].setChosen();//first
					}
					else if (chosen.y == -1 && chosen.x != i) {
						chosen.y = i;
						slots[i].setChosen(true); //secound
						selected = true;
					}
				}
				else if (!slots[i].isChosen()) {
					resetOne(chosen.x);
					chosen.x = i;
					slots[i].setChosen();
					selected = true;
				}
			}
		}
	}
}

void table::draw()
{
	for (auto& x : slots)
		window.draw(x);
}

void table::init(int numberOfCards)
{
	for (int i = 0; i < numberOfCards; i++) {
		slots.emplace_back(card::kind::white);
	}
	for (auto& x : slots) {
		x.setCharSize(22);
	}
	int beg = 250;
	int centr = (window.getSize().x / 2) - 190 * 1.8;
	numberOfCards = 10;
	float idff = float(numberOfCards) / 2;
	float width = slots.front().getXSize();
	float heigh = slots.front().getYSize();
	float spacing = 15;
	int inRow = 5;
	int centry = 800 - heigh - spacing;


	for (int rows = 0; rows < 2; rows++) {
		for (int i = 0; i < inRow; i++) {
			slots.at(i + rows * inRow).setOffest(20);
			positions.emplace_back(beg + i * (width + spacing), centry + rows * (heigh + spacing));
		}
	}
	defaultNormal = positions.at(0).y;
	maxDist = 1100 - defaultNormal;
	int i = 0;
	for (auto& x : positions) {
		hidePositions.emplace_back(x.x, x.y + maxDist);
		slots.at(i).setIndexOffest({ 164, 222 }, 20);
		slots.at(i).setPosition(x.x, x.y);
		i++;
	}
	resetChosen();
}

table::table(sf::RenderWindow & win, Deck & deck_) : window(win), deck(deck_)
{
	std::srand(std::time(nullptr));
}
bool table::setCards(std::vector<int> initCards) {
	if (initCards.size() != slots.size())
		return false;
	for (int i = 0; i < initCards.size(); i++) {
		chosen.x = i;
		replaceChosenFirst(initCards[i]);
	}
	chosen.x = chosen.y = -1;
}
void table::replaceChosenFirst(int newId)
{
	slots[chosen.x].setId(newId);
	slots[chosen.x].setTextUtf8(deck.getCard(newId, false));
}

void table::replaceChosenSecound(int newId)
{
	slots[chosen.y].setId(newId);
	slots[chosen.y].setTextUtf8(deck.getCard(newId, false));
}

void table::choseRandom()
{
	resetChosen();
	std::vector<int> cardsVectorPos{ 0,1,2,3,4,5,6,7,8,9 };
	int pos = std::rand() % 10;
	chosen.x = pos;
	slots[pos].setChosen();//first
	selected = true;
	cardsVectorPos.erase(cardsVectorPos.begin() + pos);
	if (doubl) {
		pos = std::rand() % 9;
		chosen.y = pos;
		slots[pos].setChosen();//secound
	}
}

table::~table()
{
}
