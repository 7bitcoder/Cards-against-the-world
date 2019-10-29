#pragma once
#include"table.h"
#include"chosingTable.h"
#include <SFML/Graphics.hpp>
class toggleTables :public sf::Drawable
{
private:
	bool inProgress = false;
	bool showNormal = true;//hide chosing if true
	void set() { normal.hideF(!showNormal); chosing.hideF(showNormal); }
public:
	table& normal;
	chosingTable& chosing;
	void toggle() { showNormal = !showNormal; set(); }
	void showNormalTable() { showNormal = true; set(); }
	void showChosingTable() { showNormal = false; set(); }
	void update() {}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { if (showNormal) { normal.draw(); chosing.draw(); } else { chosing.draw(); normal.draw();} }
	toggleTables();
	~toggleTables();
};

