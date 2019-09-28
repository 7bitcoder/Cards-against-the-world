#include "Trace.h"
#include<iostream>
#include "Settings.h"

extern Settings setting;

Trace::Trace(sf::RenderWindow& win, sf::Color col, int aloc, int gpuT)
	:window(win)
{
	gpuTreshold = gpuT;
	cpuMem.reserve(aloc);
	begin = 0;
	color = col;
	drawing = false;
}

Trace::~Trace()
{
}

void Trace::update(sf::Vector2f& left, sf::Vector2f& right)
{
	cpuMem.push_back(sf::Vertex(sf::Vector2f(left.x*setting.xScale, left.y * setting.yScale), color));
	cpuMem.push_back(sf::Vertex(sf::Vector2f(right.x * setting.xScale, right.y * setting.yScale), color));
	//std::cout << cpuMem.size() << std::endl;
}

void Trace::stop()
{
	gpuMem.push_back(sf::VertexBuffer(sf::TrianglesStrip, sf::VertexBuffer::Usage::Static));
	int diff = cpuMem.size() - begin;//0123456789
	if (!gpuMem.back().create(diff))
	{
		throw std::exception("gpu memory alocation fail");
	}
	if (!gpuMem.back().update(&cpuMem[begin], diff, 0))
	{
		throw std::exception("gpu vertex memory alocation fail");
	}
	begin = cpuMem.size();
	drawing = false;
}

void Trace::start()
{
	drawing = true;
}
void Trace::edge(bool beg, double& R, double& angle, sf::Vector2f& position)
{
	double ang = NINETY_DEG / 5;
	if (beg)
	{
		for (int i = 0; i < 6; i++)
		{
			//std::cout << position.x << " :pos: " << position.y << std::endl;
			sf::Vector2f point(setting.xScale*(R * sin(angle + 2.0 * NINETY_DEG - i * ang) + position.x),setting.yScale*( R * cos(angle + 2 * NINETY_DEG - i * ang) + position.y));
			cpuMem.push_back(sf::Vertex(point, color));
			//std::cout << point.x << " :: " << point.y << std::endl;
			point.x = (R * sin(angle + 2 * NINETY_DEG + i * ang) + position.x)*setting.xScale;
			point.y = (R * cos(angle + 2 * NINETY_DEG + i * ang) + position.y)*setting.yScale;
			//std::cout << point.x << " :: " << point.y << std::endl;
			cpuMem.push_back(sf::Vertex(point, color));
		}
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			sf::Vector2f point(setting.xScale*( R * sin(angle + NINETY_DEG - i * ang) + position.x), setting.yScale*(R * cos(angle + NINETY_DEG - i * ang) + position.y));
			cpuMem.push_back(sf::Vertex(point, color));
			point.x = (R * sin(angle - NINETY_DEG + i * ang) + position.x)*setting.xScale;
			point.y = (R * cos(angle - NINETY_DEG + i * ang) + position.y)*setting.yScale;
			cpuMem.push_back(sf::Vertex(point, color));
		}
	}
}

