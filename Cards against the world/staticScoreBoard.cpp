#include "staticScoreBoard.h"

void staticScoreBoard::updateScore(int id)
{
	dates[id].score++;
	dates[id].text.setString(dates[id].nickname + std::to_string(dates[id].score));
}

staticScoreBoard::staticScoreBoard(sf::RenderWindow & win) : window(win)
{
}

staticScoreBoard::~staticScoreBoard()
{
}
