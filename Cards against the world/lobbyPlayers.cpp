#include "lobbyPlayers.h"

lobbyPlayers::~lobbyPlayers()
{
}
std::vector<lobbyPlayers::player>::iterator lobbyPlayers::find(int id)
{
	for (auto it = players.begin(); it != players.end(); it++) {
		if (id == it->id) {
			return it;
		}
	}
	return players.end();
}
void lobbyPlayers::refractorization()
{
	float rad = float(charSize) / 4;
	for (int i = 0; i < players.size(); i++) {
		players[i].nick.setPosition(begin.x, begin.y + i * (charSize + 20));
		players[i].controll.setPosition(begin.x - (2 * rad + 5), players[i].nick.getPosition().y + 2 * rad);
	}
}
void lobbyPlayers::del(int id) {
	auto it = find(id);
	if (it != players.end())
		players.erase(it);
	refractorization();
}
void lobbyPlayers::setReady(int id) {
	auto it = find(id);
	if (it != players.end())
		it->controll.setFillColor(sf::Color::Green);
}

void lobbyPlayers::setNotReady(int id)
{
	auto it = find(id);
	if (it != players.end())
		it->controll.setFillColor(sf::Color::Red);
}

void lobbyPlayers::setPosition(int x, int y)
{
	begin.x = x; begin.y = y;
}

void lobbyPlayers::addPlayer(int id, sf::String nick)
{
	sf::Text text;
	text.setString(nick);
	text.setCharacterSize(charSize);
	text.setFillColor(sf::Color::White);
	text.setPosition(begin.x, begin.y + players.size() *(charSize +  20));
	text.setFont(font);

	sf::CircleShape circ;
	float rad = float(charSize) / 4;
	circ.setPosition(begin.x - (2 * rad + 5), text.getPosition().y + 2 * rad);
	circ.setRadius(rad);
	circ.setFillColor(sf::Color::Red);

	players.push_back({ id, text, circ });
}

