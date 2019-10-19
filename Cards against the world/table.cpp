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
	int centr = window.getSize().x / 2;
	int centry = 700;
	numberOfCards = numberOfCards_;
	float idff = float(numberOfCards_) / 2;
	float width = slots.front().getXSize();
	float heigh = slots.front().getYSize();
	float spacing = 20;
	float all = numberOfCards * width + spacing * (numberOfCards_ - 1);
	all /= 2;
	all = centr - all;
	for (int i = 0; i < numberOfCards_; i++) {
		positions.push_back({ i * (width + spacing) + all, float(centry) });
		slots[i].setPosition(positions.back().x, positions.back().y, 20);
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
	slots[chosen.x].setTextUtf8(deck.getCard(newId, false));
}

void table::replaceChosenSecound(int newId)
{
	slots[chosen.y].setId(newId);
	slots[chosen.y].setTextUtf8(deck.getCard(newId, false));
}

table::~table()
{
}
