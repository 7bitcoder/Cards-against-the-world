#include "Rounds.h"



Rounds::Rounds(sf::RenderWindow& win, sf::Texture& box_, sf::Texture& pointner_, sf::Texture& line_, sf::SoundBuffer& click_):
	window(win),
	SliderW(win, pointner_, line_, click_)
{
	back.setTexture(box_);
	min = 10;
	max = 60;
	number = 10;
	nmb.setCharacterSize(50);
	nmb.setString("20");
	this->setSlider(double(20) / (max - min));
}

int Rounds::getNumber()
{
	return number;
}

bool Rounds::function()
{

	if (sliderFunction())
	{
		number = min + getValue() * (max - min);
		nmb.setString(std::to_string(number));
	}
	return false;
}


Rounds::~Rounds()
{
}
