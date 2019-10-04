#include "chat.h"


chat::chat(sf::RenderWindow& win, sf::SoundBuffer& click_, int charLimit)
	: window(win), textBar(win, click_, charLimit)
{

}

void chat::setValues(sf::Vector2f position, int charSize, int chatlines, int widen)
{
	int outline = 10;
	box.setPosition(position);
	box.setSize(sf::Vector2f(widen, chatlines * (charSize + 5)));
	box.setFillColor(sf::Color(240, 240, 240, 128));
	box.setOutlineColor(sf::Color(195, 195, 195, 255));
	box.setOutlineThickness(outline);
	sf::Vector2f topp(position.x - outline, position.y - charSize);
	
	top.setPosition(topp);
	top.setSize(sf::Vector2f(widen + 2 * outline, charSize));
	top.setFillColor(sf::Color(195, 195, 195, 255));
	
	inputBox.setPosition(sf::Vector2f(position.x, position.y + outline + box.getSize().y));
	inputBox.setSize(sf::Vector2f(widen, charSize + 10));
	inputBox.setFillColor(sf::Color(240, 240, 240, 255));
	inputBox.setOutlineColor(sf::Color(195, 195, 195, 255));
	inputBox.setOutlineThickness(outline);
}

chat::~chat()
{

}
