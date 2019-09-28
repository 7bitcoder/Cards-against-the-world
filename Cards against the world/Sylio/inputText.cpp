#include "inputText.h"
#include <iostream>


inputText::inputText(sf::RenderWindow& win, sf::Texture& box_, sf::Texture& boxOff_, sf::SoundBuffer& click_) : window(win), boxOn(box_), boxOff(boxOff_), click(click_)
{
	buttonSt = buttonState::isNotPressed;
	positionSt = positionState::isNotOn;
	spriteBox.setTexture(boxOff);
	lastButtonSt = buttonSt;
	lastPositionSt = positionSt;
	clicked = false;
	activated = false;
	focuse = false;
}
bool inputText::function(bool clear)
{
	if (positionSt == positionState::isNotOn && buttonSt == buttonState::isPressed)
	{
		spriteBox.setTexture(boxOff);
		focuse = false;
	}
	else if (clicked == false && positionSt == positionState::isOn && buttonSt == buttonState::isPressed && lastButtonSt == buttonState::isNotPressed)
	{
		click.play();
		clicked = true;
		if (clear)
			this->clear();
	}
	else if (clicked == true && positionSt == positionState::isOn && buttonSt == buttonState::isNotPressed && lastButtonSt == buttonState::isPressed)
	{
		clicked = false;
		focuse = true;
		spriteBox.setTexture(boxOn);
		if (!activated)
		{
			activated = true;
			text.clear();
			textOutput.setString("");
		}
	}
	return focuse;
}
bool inputText::addChar(char t)//event.text.unicode)
{
	if (t == 8)
	{
		if (text.empty())
			return false;
		text.pop_back();
		textOutput.setString(text);
		return false;
	}
	if (t == 10)
	{
		focuse = false;
		spriteBox.setTexture(boxOff);
		return true;
	}
	if (t < 20 || t >126)
		return false;
	text += t;
	textOutput.setString(text);
	if (textOutput.getGlobalBounds().width > spriteBox.getGlobalBounds().width - 20)
	{
		text.pop_back();
		textOutput.setString(text);
		return false;
	}
	return false;
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
bool inputText::isOnButton()
{
	//std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
	//std::cout << button.getGlobalBounds().left << " " << button.getGlobalBounds().top << " "<< button.getGlobalBounds().height << " " << button.getGlobalBounds().width << std::endl;
	if (sf::Mouse::getPosition(window).x > spriteBox.getGlobalBounds().left && sf::Mouse::getPosition(window).x < (spriteBox.getGlobalBounds().left + spriteBox.getGlobalBounds().width) && sf::Mouse::getPosition(window).y > spriteBox.getGlobalBounds().top && sf::Mouse::getPosition(window).y < (spriteBox.getGlobalBounds().top + spriteBox.getGlobalBounds().height))
	{
		return true;
	}
	return false;
}
void inputText::setPosition(int x, int y)
{
	spriteBox.setPosition(x, y);
}
bool inputText::addNumber(char t)
{
	if (t == 8)
	{
		if (text.empty())
			return false;
		text.pop_back();
		textOutput.setString(text);
		return 0;
	}
	if (t < 48 || t >57 || text.size() == 2)
		return 0;
	text += t;
	textOutput.setString(text);
	return false;
}
inputText::~inputText()
{
}
