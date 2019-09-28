#include "players.h"

players::players(std::array<std::array<int, 1920>, 1080>& hitbox_, sf::RenderWindow& win, int& ymax_, int& ymin_, int& xmax_, int& xmin_, int& thicc_, sf::RenderTexture& board_)
	:window(win),
	hitbox(hitbox_),
	board(board_),
	ymax(ymax_),
	xmax(xmax_),
	xmin(xmin_),
	ymin(ymin_),
	thicc(thicc_)
{


}



players::~players()
{
}

