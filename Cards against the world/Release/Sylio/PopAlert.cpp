#include "PopAlert.h"
#include "Settings.h"

extern Settings setting;
PopAlert::PopAlert(sf::RenderWindow& win_, std::string text_, sf::Texture& box_, sf::Texture& pres, sf::Texture& rel, sf::Texture& off, sf::SoundBuffer& click_, sf::SoundBuffer& switch_, sf::Font& font_) :
	window(win_),
	ok(win_, pres, rel, off, click_, switch_, font_)
{
	text.setFont(font_);
	text.setString(text_);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);
	text.setScale(setting.xScale, setting.yScale);
	drawable = false;
	box.setTexture(box_);
	box.setPosition((1980 / 2 - 200)*setting.xScale, (1080 / 2 - 70)*setting.yScale);
	box.setScale(setting.xScale, setting.yScale);
	ok.setPosition(box.getPosition().x + 110*setting.xScale, box.getPosition().y + 135*setting.yScale);
	ok.setSoundVolume(setting.SoundVolume);
	ok.setScale(setting.xScale, setting.yScale);
	text.setPosition(box.getPosition().x + 20*setting.xScale, box.getPosition().y + 20*setting.yScale);
	text.setLineSpacing(1);
	ok.setTitle("ok");
}
PopAlert::~PopAlert()
{
}
