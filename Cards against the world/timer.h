#pragma once
#include "SFML/Graphics.hpp"
class timer : public sf::Drawable
{
private:
	sf::Sprite back;
	sf::Text title;
	sf::Text tim;
	sf::Clock clock;
	sf::Font font;
	sf::Vector2i time_;//x min y sec
	sf::Int32 allSecounds;
	sf::Int32 deadline_;//x min y sec
	bool deadlineBlock;
	sf::Int32 lastRead = 0;
	bool runing;
	void update();
public:
	timer( sf::Font& font_);
	void setTexture(sf::Texture& back_) { back.setTexture(back_); }
	void setPosition(int x, int y);
	void setTitle(std::string str);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(back); target.draw(title); target.draw(tim); }
	void stop();
	void start();
	void setSize(int size) { tim.setCharacterSize(size); }
	bool run();//true if time is up
	void setDeadline(int min, int sec) { deadline_ = min * 60 + sec; }//bez zabezpieczen uwaga !!!
	bool setTimer(int min, int sec);//setTimer
	~timer();
};

