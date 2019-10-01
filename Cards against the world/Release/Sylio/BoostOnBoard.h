#pragma once
#include <SFML/Graphics.hpp>
#include "Boost.h"
//				white/yellow/green/blue
enum boostMode { one , all, rest, random, global };
class BoostOnBoard: public sf::Sprite
{
private:
	int boost;
	boostMode mode;

public:
	void setBoost(int b, boostMode mode_ = boostMode::one) { boost = b;  mode = mode_; }
	int getBoost() {return boost; }
	boostMode getMode() { return mode; }
	BoostOnBoard();
	~BoostOnBoard();
};

