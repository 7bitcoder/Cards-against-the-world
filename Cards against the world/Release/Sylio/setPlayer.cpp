#include "setPlayer.h"
#include <iostream>
#include "Settings.h"


extern Settings setting;

bool setPlayer::focused = false;
std::vector<sf::Keyboard::Key> setPlayer::forbidden = {};
setPlayer::setPlayer(sf::RenderWindow& win, sf::Texture& box_, sf::Texture& boxOff, sf::Texture& controlsOn, sf::Texture& controlsOff, sf::Texture& checkOn, sf::Texture& checkOff, sf::SoundBuffer& click, sf::Font& font, std::string string)
	:window(win), left(win, controlsOn, controlsOff, click),
	right(win, controlsOn, controlsOff, click),
	nickname(win, box_, boxOff, click),
	checkBox(win, checkOn, checkOff, click)
{
	checkBox.setAlpha(100);
	checkBox.setScale(setting.xScale, setting.yScale);

	leftKey = sf::Keyboard::Unknown;
	rightKey = sf::Keyboard::Unknown;
	focused = false;
	activated = 1;
	nickname.setSize(40);
	nickname.setColor(sf::Color::Black);
	nickname.setFont(font);
	nickname.setString(string);
	nickname.setScale(1.8*setting.xScale, 1.4*setting.yScale);
	nickname.setAlpha(100);

	right.setSize(45);
	right.setColor(sf::Color::Black);
	right.setFont(font);
	right.setString("R");
	right.setScale(1.4*setting.xScale, 1.4*setting.yScale);
	right.setAlpha(100);

	left.setSize(45);
	left.setColor(sf::Color::Black);
	left.setFont(font);
	left.setString("L");
	left.setScale(1.4*setting.xScale, 1.4*setting.yScale);
	left.setAlpha(100);
}

bool setPlayer::function()
{
	focused = true;
	if (nickname.function())
	{
		nickname.setAlpha(255);
		left.setAlpha(255);
		right.setAlpha(255);
		checkBox.setAlpha(255);
		checkBox.setActivate();
		return true;
	}
	else if (left.function(true))
	{
		checkBox.setActivate();
		left.setAlpha(255);
		right.setAlpha(255);
		nickname.setAlpha(255);
		checkBox.setAlpha(255);
		return true;
	}
	else if (right.function(true))
	{
		checkBox.setActivate();
		right.setAlpha(255);
		left.setAlpha(255);
		nickname.setAlpha(255);
		checkBox.setAlpha(255);
		return true;
	}
	else
	{
		int t = checkBox.buttonFunction();
		if (t == 1)
		{
			nickname.setAlpha(255);
			left.setAlpha(255);
			right.setAlpha(255);
			checkBox.setAlpha(255);
			return true;

		}
		else if (t == -1)
		{
			nickname.setAlpha(100);
			left.setAlpha(100);
			right.setAlpha(100);
			checkBox.setAlpha(100);
			return true;
		}
		focused = false;
		return false;
	}	//activated = 0;
		//focused = false;

}

bool setPlayer::setChar(sf::Keyboard::Key key)
{
	std::string str = translate(key);
	if (nickname.getFocuse() && str.size() == 1)
	{
		if (nickname.empty())
			nickname.clear();
		if (nickname.addChar(str[0]) && left.empty())
		{
			nickname.disActivate();
			left.activate();
			left.clear();
		}
	}
	else if (left.getFocuse() && str != "" && str != "\n")
	{

		if (!forbidden.empty())
		{
			auto ptr = std::find(forbidden.begin(), forbidden.end(), key);
			if (ptr != forbidden.end() && leftKey != key)
			{
				return false;
			}
			if (leftKey != key)
			{
				if (leftKey != sf::Keyboard::Unknown)
				{
					auto ptr2 = std::find(forbidden.begin(), forbidden.end(), leftKey);
					forbidden.erase(ptr2);
				}
				forbidden.push_back(key);
			}
		}
		else
			forbidden.push_back(key);
		leftKey = key;
		left.clear();
		left.setString(str);
		left.disActivate();
		if (right.empty())
		{
			right.activate();
			right.clear();
		}
	}
	else if (right.getFocuse() && str != "" && str != "\n")
	{

		if (!forbidden.empty())
		{
			auto ptr = std::find(forbidden.begin(), forbidden.end(), key);
			if (ptr != forbidden.end() && rightKey != key)
			{
				return false;
			}
			if (rightKey != key)
			{
				if (rightKey != sf::Keyboard::Unknown)
				{
					auto ptr2 = std::find(forbidden.begin(), forbidden.end(), rightKey);
					forbidden.erase(ptr2);
				}
				forbidden.push_back(key);
			}
		}
		else
			forbidden.push_back(key);
		rightKey = key;
		right.clear();
		right.setString(str);
		right.disActivate();
		if (left.empty())
		{
			left.activate();
			left.clear();
		}
	}
	return false;
}

void setPlayer::checkState()
{
	nickname.checkState();
	left.checkState();
	right.checkState();
	checkBox.checkState();
}

void setPlayer::setPosition(int x, int y)
{
	nickname.setPosition(x*setting.xScale, y*setting.yScale);
	nickname.setTextPosition((x + 15)*setting.xScale, (y + 10)*setting.yScale);

	left.setPosition((x + 190 * 1.8 + 40)*setting.xScale, y*setting.yScale);
	left.setTextPosition((x + 190 * 1.8 + 40 + 20)*setting.xScale, (y + 5)*setting.yScale);

	right.setPosition((x + 49 * 1.5 + 80 + 190 * 1.8)*setting.xScale, y*setting.yScale);
	right.setTextPosition((x + 49 * 1.5 + 80 + 190 * 1.8 + 20)*setting.xScale, (y + 5)*setting.yScale);

	checkBox.setPosition((x - 50)*setting.xScale, (y + 20)*setting.yScale);
}
std::string setPlayer::getNickname() {
	return nickname.getText();
}

sf::Keyboard::Key setPlayer::getLeftKey()
{
	auto str = left.getText();
	if (str == "")
		return sf::Keyboard::Unknown;
	return leftKey;
}

sf::Keyboard::Key setPlayer::getRightKey()
{
	auto str = right.getText();
	if (str == "")
		return sf::Keyboard::Unknown;
	return rightKey;
}

setPlayer::~setPlayer()
{
}
std::string setPlayer::translate(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Key::A:
		return "a";
	case sf::Keyboard::Key::B:
		return "b";
	case sf::Keyboard::Key::C:
		return "c";
	case sf::Keyboard::Key::D:
		return "d";
	case sf::Keyboard::Key::Q:
		return "q";
	case sf::Keyboard::Key::E:
		return "e";
	case sf::Keyboard::Key::F:
		return "f";
	case sf::Keyboard::Key::G:
		return "g";
	case sf::Keyboard::Key::H:
		return "h";
	case sf::Keyboard::Key::I:
		return "i";
	case sf::Keyboard::Key::J:
		return "j";
	case sf::Keyboard::Key::K:
		return "k";
	case sf::Keyboard::Key::L:
		return "l";
	case sf::Keyboard::Key::M:
		return "m";
	case sf::Keyboard::Key::N:
		return "n";
	case sf::Keyboard::Key::O:
		return "o";
	case sf::Keyboard::Key::P:
		return "p";
	case sf::Keyboard::Key::R:
		return "r";
	case sf::Keyboard::Key::S:
		return "s";
	case sf::Keyboard::Key::T:
		return "t";
	case sf::Keyboard::Key::Y:
		return "y";
	case sf::Keyboard::Key::W:
		return "w";
	case sf::Keyboard::Key::U:
		return "u";
	case sf::Keyboard::Key::X:
		return "x";
	case sf::Keyboard::Key::Z:
		return "z";
	case sf::Keyboard::Key::V:
		return "v";
	case sf::Keyboard::Key::Add:
		return "+";
	case sf::Keyboard::Key::Space:
		return " ";
	case sf::Keyboard::Key::BackSpace:
		return "\b";
	case sf::Keyboard::Key::BackSlash:
		return "\\";
	case sf::Keyboard::Key::Dash:
		return "-";
	case sf::Keyboard::Key::Comma:
		return ",";
	case sf::Keyboard::Key::Divide:
		return "/";
	case sf::Keyboard::Key::Down:
		return "v";
	case sf::Keyboard::Key::Equal:
		return "=";
	case sf::Keyboard::Key::Left:
		return "<";
	case sf::Keyboard::Key::Multiply:
		return "*";
	case sf::Keyboard::Key::Num0:
		return "0";
	case sf::Keyboard::Key::Num1:
		return "1";
	case sf::Keyboard::Key::Num2:
		return "2";
	case sf::Keyboard::Key::Num3:
		return "3";
	case sf::Keyboard::Key::Num4:
		return "4";
	case sf::Keyboard::Key::Num5:
		return "5";
	case sf::Keyboard::Key::Num6:
		return "6";
	case sf::Keyboard::Key::Num7:
		return "7";
	case sf::Keyboard::Key::Num8:
		return "8";
	case sf::Keyboard::Key::Num9:
		return "9";
	case sf::Keyboard::Key::Numpad0:
		return "0";
	case sf::Keyboard::Key::Numpad1:
		return "1";
	case sf::Keyboard::Key::Numpad2:
		return "2";
	case sf::Keyboard::Key::Numpad3:
		return "3";
	case sf::Keyboard::Key::Numpad4:
		return "4";
	case sf::Keyboard::Key::Numpad5:
		return "5";
	case sf::Keyboard::Key::Numpad6:
		return "6";
	case sf::Keyboard::Key::Numpad7:
		return "7";
	case sf::Keyboard::Key::Numpad8:
		return "8";
	case sf::Keyboard::Key::Numpad9:
		return "9";
	case sf::Keyboard::Key::Right:
		return ">";
	case sf::Keyboard::Key::Slash:
		return "/";
	case sf::Keyboard::Key::Up:
		return "^";
	case sf::Keyboard::Key::Tilde:
		return "~";
	case sf::Keyboard::Key::Quote:
		return "'";
	case sf::Keyboard::Key::SemiColon:
		return ";";
	case sf::Keyboard::Key::Subtract:
		return "-";
	case sf::Keyboard::Key::RBracket:
		return "]";
	case sf::Keyboard::Key::LBracket:
		return "[";
	case sf::Keyboard::Key::Period:
		return ".";
	case sf::Keyboard::Key::Enter:
		return "\n";
	default:
		return "";
	}
	return 0;
}