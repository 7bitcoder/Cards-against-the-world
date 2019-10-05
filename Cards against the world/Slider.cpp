
#include "Slider.h"
#include<iostream>
#include "Settings.h"

void Slider::checkState()
{
	if (!this->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {

		if (state == pointnerState::isPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (!mouseAlredyPressed)
			{
				mouseAlredyPressed = true;
			}
			// left mouse button is pressed: shoot
			state = pointnerState::isPressed;
		}
		else {
			if (mouseAlredyPressed)
			{
				mouseAlredyPressed = false;
				state = pointnerState::isClickedOut;
			}
			else
				state = pointnerState::isNotPressed;
		}
	}
	else
	{

		if (!isAlredyOnButton) {
			isAlredyOut = false;
			isAlredyOnButton = true;
			state = pointnerState::apperedOn;
		}
		else {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!mouseAlredyPressed)
				{
					mouseAlredyPressed = true;
					state = pointnerState::isClickedOn;
				}
				// left mouse button is pressed: shoot
				else
					state = pointnerState::isPressed;
			}
			else {
				if (mouseAlredyPressed)
				{
					mouseAlredyPressed = false;
					state = pointnerState::isClickedOut;
				}
				else
					state = pointnerState::isNotPressed;
			}
		}
	}
}
int Slider::sliderFunction()
{
	int x;
	double diff, newPos;
	switch (state)
	{
	case pointnerState::isClickedOn:
		click.play();
		pressBegin = sf::Mouse::getPosition().y;
		pressScrollPos = this->getPosition().y;
		break;
	case pointnerState::isPressed:
		x = sf::Mouse::getPosition().y - pressBegin;
		newPos = pressScrollPos + x;
		if (newPos < bounds.x)
			newPos = bounds.x;
		if (newPos > bounds.y)
			newPos = bounds.y;
		diff = bounds.y - newPos;
		actualval = round(diff / Step);
		if (actualval != oldVal)
		{
			oldVal = actualval;
			return actualval - oldVal;
		}
		break;
	default:
		break;
	}
	return 0;
}

Slider::Slider(sf::RenderWindow& win, sf::SoundBuffer& click_) : window(win), click(click_)
{
	state = pointnerState::isNotOn;
	isAlredyOnButton = false;
	mouseAlredyPressed = false;
	isAlredyOut = false;
	actualval = oldVal = 0;

};

Slider::~Slider()
{
}
