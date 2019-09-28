#include "List.h"

void List::pushBack(sf::RenderWindow& win, sf::Texture& pres, sf::Texture& rel, sf::Texture& off, sf::SoundBuffer& click_, sf::SoundBuffer& switch_, sf::Font& font_)
{
	list.push_back(Button(win, pres, rel, off, click_, switch_, font_));
	list.back().setColor(sf::Color::Black);
}

List::List()
{
}

List::~List()
{
}

void List::checkState()
{
	for (int i = 0; i < list.size(); i++)
	{
		list[i].checkState();
		if (list[i].settingsListFunction())
		{
			setting.posDefaultMusic = i;
			list[i].setColor(sf::Color::Red);
			chosenFile = list[i].getString();
			for (int j = 0; j < list.size(); j++)
			{
				if (j != i)
					list[j].setColor(sf::Color::Black);
			}
			break;
		}
	}
}
