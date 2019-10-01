#include "Background.h"
#include "Settings.h"
#define PI 3.14159265
extern Settings setting;

void Background::draw()
{
	auto t = time.getElapsedTime();
	time.restart();
	//angle += t.asMilliseconds() * velocity;
	Background1.rotate(t.asMilliseconds() * velocity);
	//std::cout << Background1.getRotation() << std::endl;
/*	double y = cos(PI* Background1.getRotation() /180)*radious;
	double x = sin(PI* Background1.getRotation() /180)*radious;
	Background1.setPosition(rint(x) + zeroPiont.x ,zeroPiont.y - rint(y));*/
	window.draw(Background1);// , shader);
	window.draw(base);
}

void Background::set(sf::Texture back_, sf::Texture& base_, double r, double v)
{
	Background1.rotate(40);
	prevPos = { 0,0 };
	base.setTexture(base_);
	base.setScale(setting.xScale, setting.yScale);
	back = back_;
	Background1.setTexture(back);
	Background2.setTexture(back);
	radious = r;
	velocity = v;
	Background1.setOrigin(back.getSize().x / 2, back.getSize().y / 2 + r);
	angle = 0;
	Background1.setScale(setting.xScale, setting.yScale);
	Background1.setPosition(window.getSize().x / 2, window.getSize().y + r);
	zeroPiont.x = Background1.getPosition().x;
	zeroPiont.y = Background1.getPosition().y + r;
	time.restart();
}


Background::~Background()
{
}
