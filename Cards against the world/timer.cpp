#include "timer.h"

timer::~timer()
{
}

timer::timer(sf::Font& font_) : font(font_)
{
	title.setFont(font);
	tim.setFont(font);
	title.setFillColor(sf::Color::White);
	tim.setFillColor(sf::Color::White);
	deadline_ = 10;
}

void timer::setPosition(int x, int y)
{
	back.setPosition(x, y);
	title.setPosition(x, y);
	tim.setPosition( x + 90, y);
}

void timer::setTitle(std::string str)
{
	title.setString(str);
}

void timer::stop()
{
	runing = false;
}

void timer::start()
{
	runing = true;
	clock.restart();
}

bool timer::run()
{
	if (runing) {
		auto time = clock.getElapsedTime().asMilliseconds() / 1000;
		if (time != lastRead) {
			lastRead = time;
			int left = allSecounds - time;
			time_ = { left / 60, left % 60 };
			if (left <= 0) {
				time_ = { 0, 0 };
				update();
				stop();
				return true;
			}
			if (!deadlineBlock && left <= deadline_) {
				deadlineBlock = true;
				tim.setFillColor(sf::Color::Red);
			}
			update();
		}
	}
	return false;
}

bool timer::setTimer(int min_, int sec_)
{
	if (min_ < 0 || min_ > 60 || sec_ < 0 || sec_ > 60)
		return false;
	deadlineBlock = false;
	time_ = { min_, sec_ };
	allSecounds = min_ * 60 + sec_;
	update();
	clock.restart();
}

void timer::update()
{
	std::string str = time_.x / 10 ? std::to_string(time_.x) : "0" + std::to_string(time_.x);
	str += ":";
	str += time_.y / 10 ? std::to_string(time_.y) : "0" + std::to_string(time_.y);
	tim.setString(str);
}
