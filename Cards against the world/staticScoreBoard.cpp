#include "staticScoreBoard.h"
#include <algorithm>

void staticScoreBoard::updateScore(int id)//set last winner also
{
	isChosing = true;
	isLastWinner = true;
	for (auto it = dates.begin(); it != dates.end(); it++) {
		if (it->id == id) {
			it->score++;
			it->text.setString(it->nickname + std::to_string(it->score));
			lastWinner = it;
			updateCHosingAndWinner();
			return;
		}
	}
	
}

void staticScoreBoard::setChosing(int id)
{
	for (auto it = dates.begin(); it != dates.end(); it++) {
		if (it->id == id){
			chosing = it;
			updateCHosingAndWinner();
			return;
		}
	}
}
void updateCHosingAndWinner(){
	if(chosing == lastWinner){
		int posx = lastWinner.text.getPosition().x + lastWinner.text.getGlobalBounds().width;
		int posy = lastWinner.text.getPosition().y;
		choserSprite.setPosition(posx, posy);
		winner.setPosition(posx + choserSprite.getGlobalBounds().width, posy);
	} else {
		int posx = lastWinner.text.getPosition().x + lastWinner.text.getGlobalBounds().width;
		int posy = lastWinner.text.getPosition().y;
		winner.setPosition(posx, posy);
		
		posx = chosing.text.getPosition().x + chosing.text.getGlobalBounds().width;
		posy = chosing.text.getPosition().y;
		choserSprite.setPosition(posx, posy);
	}
}
void staticScoreBoard::setPosition(int x, int y)
{
	for (int i = 0; i < dates.size(); i++) {
		int yVal =  y + i*(1.1*charSize)
		dates.at(i).text.setPosition(x, yVal);
		dates.at(i).check.setPosition(x - 50, yVal);
	}
}

staticScoreBoard::staticScoreBoard(sf::RenderWindow& win) : window(win) {}

void staticScoreBoard::init(int size, std::map<int, sf::String>& players, sf::Font& font)
{
	charSize = size;
	base = sf::Color::White;
	for (auto& x : players) {
		dates.emplace_back(x.first, 0, x.second + " ");
		dates.back().text.setString(dates.back().nickname + "0");
		dates.back().text.setFillColor(base);
		dates.back().text.setFont(font);
		dates.back().text.setCharacterSize(size);
		dates.back().check.setTexture(checkText);
	}
	sort(dates.begin(), dates.end(), [](data & x1, data & x2) { return x1.id < x2.id; });
	winner.setTexturre(lasWinnerText);
	choserSprite.setTexture(choserText);
}

void staticScoreBoard::check(int id)
{
	for (auto& x : dates) {
		if (x.id == id){
			markId = id;
			x.checked = true;
			break;
		}
	}
}

void staticScoreBoard::resetCheck(int id)
{
	for (auto& x : dates) {
		x.checked = false;
	}
}
void rotateMainPlayer(int id){
	for (auto it = dates.begin(); it != dates.end() ; it++) {
		if(it->id == id){
			std::rotate(dates.begin(); it, dates.end());
			break;
		}
	}
}

staticScoreBoard::~staticScoreBoard()
{
}
