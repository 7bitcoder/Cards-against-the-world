#pragma once
#include "SFML/Graphics.hpp"
class lobbyPlayers
{
private:
	struct player {
		int id;
		sf::Text nick;
		sf::CircleShape controll;
	};
	sf::Vector2f begin;
	sf::Font& font;
	sf::RenderWindow& window;
	int charSize;
	std::vector<player> players;
	std::vector<player>::iterator find(int id);
	void refractorization();
public:
	void del(int id);
	void setReady(int id);
	void setNotReady(int id);
	void setPosition(int x, int y);
	void addPlayer(int id, sf::String nick);
	void draw() { for (auto& x : players) { window.draw(x.controll); window.draw(x.nick); } }
	lobbyPlayers(sf::RenderWindow& win, sf::Font& font_, int charSize_) : window(win), font(font_), charSize(charSize_) {};
	~lobbyPlayers();
};

