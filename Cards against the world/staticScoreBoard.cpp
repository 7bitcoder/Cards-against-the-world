#include "staticScoreBoard.h"
#include <algorithm>
sf::Texture staticScoreBoard::checkText;
sf::Texture staticScoreBoard::choserText;
sf::Texture staticScoreBoard::lasWinnerText;
void staticScoreBoard::updateScore(int id)//set last winner also
{
	isChosing = true;
	isLastWinner = true;
	for (auto it = dates.begin(); it != dates.end(); it++) {
		if (it->id == id) {
			it->score++;
			it->text.setString(it->nickname + std::to_string(it->score));
			lastWinner = it;
			isLastWinner = true;
			updateCHosingAndWinner();
			return;
		}
	}

}

void staticScoreBoard::setChosing(int id)
{
	for (auto it = dates.begin(); it != dates.end(); it++) {
		if (it->id == id) {
			chosing = it;
			isChosing = true;
			updateCHosingAndWinner();
			return;
		}
	}
}
void staticScoreBoard::updateCHosingAndWinner() {
	int off = 20;
	if (chosing == lastWinner && chosing != dates.end() && lastWinner != dates.end()) {
		int posx = lastWinner->text.getPosition().x + lastWinner->text.getGlobalBounds().width;
		int posy = lastWinner->text.getPosition().y;
		choserSprite.setPosition(posx, posy - off);
		winner.setPosition(posx + choserSprite.getGlobalBounds().width - 30, posy - off -2);
	}
	else {
		if (lastWinner != dates.end()) {
			int posx = lastWinner->text.getPosition().x + lastWinner->text.getGlobalBounds().width;
			int posy = lastWinner->text.getPosition().y;
			winner.setPosition(posx, posy - off - 2);
		}
		if (chosing != dates.end()) {
			int posx = chosing->text.getPosition().x + chosing->text.getGlobalBounds().width;
			int posy = chosing->text.getPosition().y;
			choserSprite.setPosition(posx, posy - off);
		}
	}
}
void staticScoreBoard::setPosition(int x, int y)
{
	for (int i = 0; i < dates.size(); i++) {
		int yVal = y + i * (1.3 * charSize);
		dates.at(i).text.setPosition(x, yVal);
		dates.at(i).check.setPosition(x - 65, yVal - 20);
	}
}

staticScoreBoard::staticScoreBoard(sf::RenderWindow & win) : window(win) {}

void staticScoreBoard::init(int size, std::map<int, sf::String> & players, sf::Font & font)
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
	winner.setTexture(lasWinnerText);
	choserSprite.setTexture(choserText);
}

void staticScoreBoard::check(int id)
{
	for (auto& x : dates) {
		if (x.id == id) {
			x.checked = true;
			break;
		}
	}
}

void staticScoreBoard::resetCheck()
{
	for (auto& x : dates) {
		x.checked = false;
	}
}
void staticScoreBoard::rotateMainPlayer(int id) {
	for (auto it = dates.begin(); it != dates.end(); it++) {
		if (it->id == id) {
			std::rotate(dates.begin(), it, dates.end());
			break;
		}
	}
	lastWinner = dates.end();
	chosing = dates.end();
}

staticScoreBoard::~staticScoreBoard()
{
}
