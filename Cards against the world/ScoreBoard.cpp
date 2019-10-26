#include "ScoreBoard.h"
#include <algorithm>
#include "Settings.h"
extern Settings setting;

void ScoreBoard::update()
{
	if (updating)
	{
		bool flag = true;
		for (int i = 0; i < rank.size(); i++)
		{
			if (states.front()[i]->activte)
			{
				move(i);
				flag = false;
			}
		}
		if (flag)
		{
			updating = false;
			main.pop();
		}
	}
	else if (!main.empty())
	{
		updating = true;
		states.pop();
		for (int i = 0; i < dates.size(); i++)
		{
			if (states.front()[i]->text.getPosition().y != positions[i])
			{
				states.front()[i]->activte = true;
				states.front()[i]->timer.restart();
			}
			else
				states.front()[i]->activte = false;

		}
	}

}

void ScoreBoard::updateScore(int id)
{
	dates[id].score++;
	dates[id].text.setString(dates[id].nickname + std::to_string(dates[id].score));
	states.push(states.back());
	std::sort(states.back().begin(), states.back().end(), [](std::vector<data>::iterator i, std::vector<data>::iterator j) { return (i->score > j->score); });
	main.push(id);
}
void ScoreBoard::draw()
{
	for (int i = 0; i < dates.size(); i++)
	{
		if (main.empty() || i != main.front())//if updating??
			window.draw(dates[i].text);
	}
	if (!main.empty())
		window.draw(dates[main.front()].text);
}

void ScoreBoard::move(int ra)
{
	float ymove = states.front()[ra]->timer.getElapsedTime().asSeconds() * vel;
	states.front()[ra]->timer.restart();
	//if (!setting.TimeStop)
	if (states.front()[ra]->text.getPosition().y > positions[ra])
	{
		states.front()[ra]->text.move(0, -ymove);
		if (states.front()[ra]->text.getPosition().y <= positions[ra])
		{
			states.front()[ra]->activte = false;
			states.front()[ra]->text.setPosition(posx, positions[ra]);
		}
	}
	else if (states.front()[ra]->text.getPosition().y < positions[ra])
	{
		states.front()[ra]->text.move(0, ymove);
		if (states.front()[ra]->text.getPosition().y >= positions[ra])
		{
			states.front()[ra]->activte = false;
			states.front()[ra]->text.setPosition(posx, positions[ra]);
		}
	}
}


void ScoreBoard::setPosition(int x, int y, std::map<int, sf::String> & players, sf::Font & font)
{
	updating = false;
	posx = x;
	posy = y;
	dates.clear();
	positions.clear();
	rank.clear();
	while (!states.empty())
		states.pop();
	while (!main.empty())
		main.pop();
	dates.resize(players.size());
	for (auto& player : players)
	{
		dates[player.first].activte = false;
		dates[player.first].nickname = player.second + " ";
		dates[player.first].score = 0;
		dates[player.first].text.setFillColor(base);
		dates[player.first].text.setCharacterSize(charSize);
		dates[player.first].text.setString(dates[player.first].nickname + "0");
		dates[player.first].text.setFont(font);
		dates[player.first].text.setScale(setting.xScale, setting.yScale);
	}
	for (int i = 0; i < dates.size(); i++)
	{
		positions.push_back(y + int(i * (charSize) * 1.5));
		dates[i].text.setPosition(x, positions.back());
		rank.push_back(dates.begin() + i);
	}
	diff = (positions[1] - positions[0] - charSize) / 2;
	states.push(std::move(std::vector<std::vector<data>::iterator>(rank.begin(), rank.end())));
}

ScoreBoard::ScoreBoard(sf::RenderWindow& win): window(win) {}

void ScoreBoard::init(int size, float vel_, int numberOfPleyers) {
	for (int i = 0; i < numberOfPleyers; i++) {
		dates.emplace_back( 0, "", false );
	}
	vel = vel_;
	updating = false;
	charSize = size * setting.yScale;
}

ScoreBoard::~ScoreBoard()
{
}
