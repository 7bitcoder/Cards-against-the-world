#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<math.h>
#include<iostream>
class Background
{
private:
	sf::Clock time;
	sf::Texture back;
	sf::Sprite base;
	sf::Sprite Background1;
	sf::Sprite Background2;
	sf::RenderWindow& window;
	double radious;
	double velocity;
	double angle;
	sf::Vector2i zeroPiont;
	sf::Vector2i prevPos;
	sf::Shader* shader;
public:
	void draw();
	Background(sf::RenderWindow& window_) : window(window_) { radious = 0; velocity = 0; angle = 0; }
	void setShader(sf::Shader& sh) { shader = &sh; }
	void set(sf::Texture back_, sf::Texture& base_, double r, double v);
	~Background();
};

