#include "CheckBox.h"

CheckBox::CheckBox(sf::RenderWindow& win, sf::Texture& activated_, sf::Texture& disactivated_, sf::SoundBuffer& click_) :
	click(click_),
	window(win),
	activated(activated_),
	disActivated(disactivated_)
{
	box.setTexture(disActivated);
	buttonSt = buttonState::isNotPressed;
	positionSt = positionState::isNotOn;
	activate = false;
}

int CheckBox::buttonFunction()
{
	//std::cout << state;
	if (positionSt == positionState::isNotOn && lastPositionSt == positionState::isNotOn)
		return 0;
	else if (positionSt == positionState::isOn && buttonSt == buttonState::isPressed && lastButtonSt == buttonState::isNotPressed)
	{
		click.play();

	}
	else if (positionSt == positionState::isOn && buttonSt == buttonState::isNotPressed && lastButtonSt == buttonState::isPressed)
	{

		activate = !activate;
		if (activate)
		{
			box.setTexture(activated, true);
			return 1;
		}
		else
		{
			box.setTexture(disActivated, true);
			return -1;
		}
	}
	return 0;
}

void CheckBox::checkState()
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

bool CheckBox::isOnButton()
{	//std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
	//std::cout << button.getGlobalBounds().left << " " << button.getGlobalBounds().top << " "<< button.getGlobalBounds().height << " " << button.getGlobalBounds().width << std::endl;
	if (sf::Mouse::getPosition(window).x > box.getGlobalBounds().left && sf::Mouse::getPosition(window).x < (box.getGlobalBounds().left + box.getGlobalBounds().width) && sf::Mouse::getPosition(window).y > box.getGlobalBounds().top && sf::Mouse::getPosition(window).y < (box.getGlobalBounds().top + box.getGlobalBounds().height))
	{
		return true;
	}
	return false;
}

CheckBox::~CheckBox()
{
}
