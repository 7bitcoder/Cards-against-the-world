#pragma once
#include"table.h"
#include"chosingTable.h"
#include <SFML/Graphics.hpp>
class toggleTables
{
private:
	bool inProgress = false;
	bool showNormal = true;//hide chosing if true
	void set() { normal.hideF(!showNormal); chosing.hideF(showNormal); inProgress = true; }
public:
	table& normal;
	chosingTable& chosing;
	bool toggle() { if (inProgress) return false; showNormal = !showNormal; set(); return true; }
	bool showNormalTable() { if (inProgress) return false; showNormal = true; set(); return true; }
	bool showChosingTable() { if (inProgress) return false; showNormal = false; set(); return true; }
	void update() {
		if (inProgress) {
			chosing.update();
			normal.update();
			inProgress = !chosing.end() || !normal.end();
		}
	}
	void draw() { if (!showNormal) { normal.draw(); chosing.draw(); } else { chosing.draw(); normal.draw(); } }
	toggleTables(table& norm_, chosingTable& chos) : normal(norm_), chosing(chos) {};
	~toggleTables();
};

