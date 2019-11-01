#include "card.h"


sf::Texture card::whiteTempl;
sf::Texture card::blackTempl;
sf::Font card::font;
void card::compute()
{
	sf::String tmp = string;
	test.setString(tmp);
	float max = back.getGlobalBounds().width - 2 * offset;
	int i, beg, end, offset_, length = tmp.getSize() - 1;
	i = beg = end = offset_ = 0;
	while (end <= length) {//cos tam 
		float len = 0;
		float start = test.findCharacterPos(beg).x;
		for (i = beg; len < max; i++) {
			len = test.findCharacterPos(i).x - start;
			if (tmp[i] == ' ')
				end = i;
			if (i == length) {
				end = i;
				break;
			}
		}
		if (end == beg)
			end = i;
		end++;
		string.insert(end + offset_++, "\n");
		beg = end;
	}
}

void card::setId(int id_)
{
	id = id_;
	std::string x = std::to_string(id);
	number.setString(x);
	number.setOrigin(number.getGlobalBounds().width / 2, 0);
}

bool card::isOn(sf::Vector2f pos)
{
	return back.getGlobalBounds().contains(pos);
}

void card::setPosition(float x, float y)
{
	back.setPosition(x, y);
	text.setPosition(x + offset, y + offset);
	//219 302
	number.setPosition(x + indexOffest.x, y + indexOffest.y);
	chose.setPosition(x + offset / 3, y + offset / 3);
}

void card::setIndexOffest(sf::Vector2f offest, int charSize)
{
	indexOffest = offest;
	number.setCharacterSize(charSize);
}

void card::setBacgrounded(uint8_t val)
{
	back.setColor(sf::Color(val, val, val, 255));
}

void card::setTextUtf8(std::string str)
{
	string.clear();
	auto it = str.begin();
	while (it != str.end()) {
		sf::Uint32 character;
		it = sf::Utf8::decode(it, str.end(), character);
		if (character)
			string += character;
	}
	compute();
	text.setString(string);
}

void card::setChosen(bool secound)
{
	chosen = true;
	secound ? chose.setString("2") : chose.setString("1");
	setBacgrounded(200);
}

void card::resetChosen()
{
	setBacgrounded(255);
	chosen = false;
	chose.setString("");
}

card::card(card::kind kind__)
{
	kind_ = kind__;
	back.setTexture(kind_ == kind::black ? blackTempl : whiteTempl);
	test.setPosition(0, 0);
	test.setFont(font);
	test.setFillColor(sf::Color::White);
	text.setFont(font);
	text.setFillColor(kind_ == kind::black ? sf::Color::White : sf::Color::Black);
	number.setFont(font);
	number.setFillColor(kind_ == kind::black ? sf::Color::White : sf::Color::Black);
	chose.setFont(font);
	chose.setFillColor(kind_ == kind::black ? sf::Color::White : sf::Color::Black);
	chose.setCharacterSize(18);
	chosen = false;
}
card::~card()
{}
