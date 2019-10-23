#include "table.h"

sf::Vector2i table::getChosenId()
{
	return { slots[chosen.x].getId(),slots[chosen.y].getId() };
}

void table::function()
{
	static bool full = false;
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
				}
			}
			else if (!slots[i].isChosen()) {
				resetOne(chosen.x);
				chosen.x = i;
				slots[i].setChosen();

			}
		}
	}
}

void table::draw()
{
	for (auto& x : slots)
		window.draw(x);
}

table::table(sf::RenderWindow & win, int numberOfCards_) : window(win), slots(numberOfCards_, card(card::kind::white))
{
	for (auto& x : slots) {
		x.setCharSize(20);
	}
	int beg = 250;
	int centr = (window.getSize().x / 2) - 190 * 1.8;
	numberOfCards = numberOfCards_;
	float idff = float(numberOfCards_) / 2;
	float width = slots.front().getXSize();
	float heigh = slots.front().getYSize();
	float spacing = 15;
	int inRow = 5;
	int centry = 800 - heigh - spacing;
	for (int rows = 0; rows < 2; rows++) {
		for (int i = 0; i < inRow; i++) {
			slots[i + rows * inRow].setOffest(20);
			slots[i + rows * inRow].setPosition(beg + i * (width + spacing), centry + rows * (heigh + spacing));
		}
	}
	resetChosen();
}
bool table::init(std::vector<int> initCards) {
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
	slots[chosen.x].setTextUtf8(deck.getCard(newId, true));
}

void table::replaceChosenSecound(int newId)
{
	slots[chosen.y].setId(newId);
	slots[chosen.y].setTextUtf8(deck.getCard(newId, false));
}

table::~table()
{
}
