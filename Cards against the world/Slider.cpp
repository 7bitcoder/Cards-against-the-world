
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
	double x, pos;
	int dif;
	switch (state)
	{
	case pointnerState::isClickedOn:
		click.play();
		pressBegin = sf::Mouse::getPosition(window).y;
		break;
	case pointnerState::isPressed:
		pos = sf::Mouse::getPosition(window).y;
		x = pos - pressBegin;
		if (x > Step && delta < maxDelta) {//go down;
			dif = x / Step;
			if (delta + dif > maxDelta)
				dif = maxDelta - delta;
			delta += dif;
			setPos();
			pressBegin += dif * Step;
			return -dif;
		}
		else if (x < -Step && delta > 0) {//go up
			dif = -x / Step;
			if (delta - dif < 0)
				dif = delta;
			delta -= dif;
			setPos();
			pressBegin -= dif * Step;
			return dif;
		}
		break;
	default:
		break;
	}
	return 0;
}

Slider::Slider(sf::RenderWindow & win, sf::SoundBuffer & click_) : window(win), click(click_)
{
	state = pointnerState::isNotOn;
	isAlredyOnButton = false;
	mouseAlredyPressed = false;
	isAlredyOut = false;
	delta = 0;
};

Slider::~Slider()
{
}
