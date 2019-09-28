#include "inputText.h"
#include <iostream>


inputText::inputText(sf::RenderWindow& win, sf::Texture& box_, sf::SoundBuffer& click_, int charLimit) : window(win), box(box_), click(click_)
{
	limit = charLimit;
	focused = false;
	coursorPosition = 0;
	spriteBox.setTexture(box);
	blink = false;
	clicked = false;
	activated = false;
}
bool inputText::function(bool clear)
{
	checkState();
	if (clicked == false && positionSt == positionState::isOn && buttonSt == buttonState::isPressed && lastButtonSt == buttonState::isNotPressed)
	{
		click.play();
		if (!activated) {
			activated = true;
			this->clear();
		}
		focused = true;
		checkCoursorPosition();
		timer.restart();
		blueMark.setSize(sf::Vector2f(0, blueMark.getSize().y));
		blueMark.setPosition(textOutput.findCharacterPos(coursorPosition));
		coursorLastPosition = coursorPosition;
		blink = true;
		clicked = true;
	}
	else if (buttonSt == buttonState::isNotPressed && lastButtonSt == buttonState::isPressed) {
		clicked = false;
	}
	else if (positionSt == positionState::isNotOn && buttonSt == buttonState::isPressed && lastButtonSt == buttonState::isNotPressed)
	{
		focused = false;
		blink = false;
	}
	if (clicked) {
		mark();
	}
	if (focused) {
		checkBlink();
	}
	return focused;
}
void inputText::checkState()
{
	lastButtonSt = buttonSt;
	lastPositionSt = positionSt;
	if (isOnButton())
		positionSt = positionState::isOn;
	else
		positionSt = positionState::isNotOn;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		buttonSt = buttonState::isPressed;
	else
		buttonSt = buttonState::isNotPressed;
}
bool inputText::addChar(sf::String h)//event.text.unicode)
{
	sf::String t = h[0];
	static int counter = 0;
	if (lastChar == t) {
		std::cout << "taki sam\n";
		if (counter < 2) {
			if (slowClock.getElapsedTime().asSeconds() < 0.2)
				return false;
			else {
				std::cout << "dalej\n";
				slowClock.restart();
				counter++;
			}
		}
		else {
			if (slowClock.getElapsedTime().asSeconds() < 0.02)
				return false;
			else
				slowClock.restart();
		}
	}
	else {
		std::cout << "nowy: " << t.toAnsiString() << std::endl;
		counter = 0;
		slowClock.restart();
	}
	lastChar = t;
	if (t == char(8))
	{
		if (!text.getSize())
			return false;
		coursorPosition--;
		text.erase(coursorPosition, 1);
		textOutput.setString(text);
		setCoursorPosition(coursorPosition);

		return false;
	}
	else if (t == char(10))
	{
		focused = false;
		blink = false;
		return true;
	}
	else if (t == char(127))
	{
		text.erase(coursorPosition, 1);
		textOutput.setString(text);
		return false;
	}
	if (text.getSize() < limit) {
		text.insert(coursorPosition, t);
		coursorPosition++;
		textOutput.setString(text);
		setCoursorPosition(coursorPosition);

	}
	return false;
}
void inputText::setCoursorPosition(std::size_t i) {
	coursorPosition = i;
	coursor.setPosition(textOutput.findCharacterPos(i));
}

bool inputText::isOnButton()
{
	//std::cout << sf::Mouse::getPosition(window).x << ' ' << sf::Mouse::getPosition(window).y << std::endl;
	//std::cout << button.getGlobalBounds().left << ' ' << button.getGlobalBounds().top << ' '<< button.getGlobalBounds().height << ' ' << button.getGlobalBounds().width << std::endl;
	if (sf::Mouse::getPosition(window).x > spriteBox.getGlobalBounds().left && sf::Mouse::getPosition(window).x < (spriteBox.getGlobalBounds().left + spriteBox.getGlobalBounds().width) && sf::Mouse::getPosition(window).y > spriteBox.getGlobalBounds().top && sf::Mouse::getPosition(window).y < (spriteBox.getGlobalBounds().top + spriteBox.getGlobalBounds().height))
	{
		return true;
	}
	return false;
}
void inputText::checkCoursorPosition()
{
	int mousePosition = sf::Mouse::getPosition().x;
	std::cout << mousePosition << std::endl;
	float min = abs(textOutput.findCharacterPos(0).x - mousePosition);
	setCoursorPosition(0);
	for (int i = 0; i <= text.getSize(); i++) {
		std::cout << "poz: " << i << " " << textOutput.findCharacterPos(i).x << std::endl;
		float val = abs(textOutput.findCharacterPos(i).x - mousePosition);
		if (val < min) {
			min = val;
			std::cout << "tak: " << i << std::endl;
			setCoursorPosition(i);
		}
	}
}
void inputText::setPosition(int x, int y)
{
	spriteBox.setPosition(x, y);
	setTextPosition(x + 10, y + 10);
	coursor.setPosition(x + 10, y + 10);
}
void inputText::checkBlink()
{
	if (timer.getElapsedTime().asSeconds() > 0.5) {
		timer.restart();
		blink = !blink;
	}
}
void inputText::mark()
{
	checkCoursorPosition();
	int diff = coursorPosition - coursorLastPosition;
	if (diff) {
		blueMark.setSize(sf::Vector2f(textOutput.findCharacterPos(coursorPosition).x - blueMark.getPosition().x, blueMark.getSize().y));
		coursorLastPosition = coursorPosition;
	}
}
inputText::~inputText()
{
}

