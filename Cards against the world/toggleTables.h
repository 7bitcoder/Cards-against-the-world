#pragma once
#include"table.h"
#include"chosingTable.h"
#include <SFML/Graphics.hpp>
#include "SFML/Window.hpp"
#include <SFML/Audio.hpp>

class toggleTables : public sf::Drawable
{
private:

	sf::Sprite back;
	sf::Sprite circle;
	sf::Sound click;
	bool block_ = false;
	bool inProgress = false;
	bool forceNormal = false;
	bool forceChsosing = false;
	bool showNormal = true;//hide chosing if true
	void set() { normal->hideF(!showNormal); chosing->hideF(showNormal); inProgress = true; }
	bool toggle() { showNormal = !showNormal; set(); return true; }
public:
	void setSpeed(float speed) { normal->setAlpha(speed); chosing->setAlpha(speed); }
	sf::Text title;
	bool function();
	table * normal;
	chosingTable* chosing;
	void forcewNormalTable();
	void forceChosingTable();
	void block() { block_ = true; circle.setColor(sf::Color(255,255,255,128)); back.setColor(sf::Color(255, 255, 255, 128));}
	void unBlock() { block_ = false; circle.setColor(sf::Color(255, 255, 255, 255)); back.setColor(sf::Color(255, 255, 255, 255));}
	void blockNormal() { normal->setBlock(true); chosing->setBlock(false); }
	void blockChosing() { normal->setBlock(false); chosing->setBlock(true); }
	void setTitle(sf::String str) { title.setString(str); }
	void setPosition(sf::Vector2f pos);
	void setSound(sf::SoundBuffer& snd) { click.setBuffer(snd); }
	void update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	toggleTables() {};
	void setTables(table* tabl, chosingTable* chos) { normal = tabl; chosing = chos; }
	void setTextures(sf::Texture& back_, sf::Texture& circle_) {
		back.setTexture(back_);
		circle.setTexture(circle_);
	}
	~toggleTables();
};

