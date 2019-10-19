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
	pos -= back.getPosition();
	auto bound = back.getGlobalBounds();
	if (pos.x > 0 && pos.y > 0 && pos.x < bound.width && pos.y < bound.height)
	{
		return true;
	}
	return false;
}

void card::setPosition(int x, int y, int offset_)
{
	offset = offset_;
	back.setPosition(x, y);
	text.setPosition(x + offset, y + offset);
	//219 302
	number.setPosition(x + 174, y + 230);
	chose.setPosition(x, y);
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
}

void card::resetChosen()
{
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
	number.setCharacterSize(18);
	chose.setFont(font);
	chose.setFillColor(kind_ == kind::black ? sf::Color::White : sf::Color::Black);
	chose.setCharacterSize(15);
	chosen = false;
}
card::~card()
{}
