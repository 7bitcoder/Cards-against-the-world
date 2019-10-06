#include "inputText.h"
#include <iostream>


inputText::inputText(sf::RenderWindow& win, sf::SoundBuffer& click_, int charLimit) : window(win), click(click_),
mark(textOutput, win)
{
	limit = charLimit;
	focused = false;
	coursorPosition = 0;
	blink = false;
	clicked = false;
	activated = false;
}
bool inputText::function()
{
	checkState();
	if (clicked == false && positionSt == positionState::isOn && buttonSt == buttonState::isPressed && lastButtonSt == buttonState::isNotPressed)
	{
		click.play();
		if (!activated) {
			activated = true;
			this->clear();
		}
		focused = true;
		checkCoursorPosition();
		timer.restart();
		mark.clear();
		mark.hook(coursorPosition);
		coursorLastPosition = coursorPosition;
		blink = true;
		clicked = true;
	}
	else if (buttonSt == buttonState::isNotPressed && lastButtonSt == buttonState::isPressed) {
		clicked = false;
	}
	else if (positionSt == positionState::isNotOn && buttonSt == buttonState::isPressed && lastButtonSt == buttonState::isNotPressed)
	{
		focused = false;
		blink = false;
		mark.clear();
	}
	if (clicked) {
		findMark();
	}
	if (focused) {
		checkBlink();
	}
	return focused;
}
void inputText::checkState()
{
	lastButtonSt = buttonSt;
	lastPositionSt = positionSt;
	if (isOnButton())
		positionSt = positionState::isOn;
	else
		positionSt = positionState::isNotOn;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		buttonSt = buttonState::isPressed;
	else
		buttonSt = buttonState::isNotPressed;
}
bool inputText::addChar(sf::Event::KeyEvent h) {
	wchar_t t = translate(h);
	int state = checkSpecialCharacters(t);
	if (!state)
		return false;
	else if (state == 1) {
		return true;
	}
	if (text.getSize() < limit) {
		text.insert(coursorPosition, t);
		coursorPosition++;
		textOutput.setString(text);
		setCoursorPosition(coursorPosition);
	}
	return false;
}
void inputText::checkBounds()
{
	/*if (coursor.getPosition().x > bounds.x)
		shiftLeft();
	else if(cour)*/
}

int inputText::checkSpecialCharacters(wchar_t t) {

	if (!t) // zero
		return false;
	if (t == wchar_t(1)) // ctr + v
	{
		if (mark.activated) {
			auto beg = mark.getBeg();
			auto end = mark.getEnd();
			if (beg > end)
				std::swap(beg, end);
			text.erase(beg, end - beg);
			setCoursorPosition(beg);
			mark.clear();
		}
		sf::String clip = sf::Clipboard::getString();
		text.insert(coursorPosition, clip);
		auto size = clip.getSize();
		if (text.getSize() > limit) {
			text.erase(limit, text.getSize() - limit);
		}
		coursorPosition += clip.getSize();
		if (coursorPosition > limit) {
			coursorPosition = limit;
		}
		text.replace(wchar_t(10), sf::String());//remove \n
		textOutput.setString(text);
		setCoursorPosition(coursorPosition);
		return 0; //break
	}
	else if (t == wchar_t(3)) // right arrow
	{
		mark.clear();
		if (!coursorPosition)
			return false;
		coursorPosition--;
		timer.restart();//to make it visible
		blink = true;
		setCoursorPosition(coursorPosition);
		return 0;//go next
	}
	else if (t == wchar_t(2)) // left arrow
	{
		mark.clear();
		if (coursorPosition == text.getSize())
			return false;
		coursorPosition++;
		timer.restart();
		blink = true;
		setCoursorPosition(coursorPosition);
		return 0;
	}
	else if (t == wchar_t(4)) // ctrl + c
	{
		auto beg = mark.getBeg();
		auto end = mark.getEnd();
		if (beg > end)
			std::swap(beg, end);
		sf::String cpl = text.substring(beg, end - beg);
		sf::Clipboard::setString(cpl);
		return 0;
	}
	else if (t == wchar_t(5)) // ctrl + x
	{
		if (mark.activated) {
			auto beg = mark.getBeg();
			auto end = mark.getEnd();
			if (beg > end)
				std::swap(beg, end);
			sf::String cpl = text.substring(beg, end - beg);
			text.erase(beg, end - beg);
			setCoursorPosition(beg);
			sf::Clipboard::setString(cpl);
			textOutput.setString(text);
			mark.clear();
		}
		return 0;
	}
	else if (t == wchar_t(8)) // backspace
	{
		if (mark.activated) {
			auto beg = mark.getBeg();
			auto end = mark.getEnd();
			if (beg > end)
				std::swap(beg, end);
			text.erase(beg, end - beg);
			setCoursorPosition(beg);
			textOutput.setString(text);
			mark.clear();
			return 0;
		}
		if (!coursorPosition)
			return 0;
		coursorPosition--;
		text.erase(coursorPosition, 1);
		textOutput.setString(text);
		setCoursorPosition(coursorPosition);
		return 0;
	}
	else if (t == wchar_t(10))//enter
	{
		focused = false;
		blink = false;
		clicked = false;
		if (mark.activated) {
			auto beg = mark.getBeg();
			auto end = mark.getEnd();
			if (beg > end)
				std::swap(beg, end);
			text.erase(beg, end - beg);
			setCoursorPosition(beg);
			textOutput.setString(text);
		}
		mark.clear();
		return 1;
	}
	else if (t == wchar_t(127)) // delete todo mabey
	{
		text.erase(coursorPosition, 1);
		textOutput.setString(text);
		return 0;
	}
	else if (mark.activated) {
		auto beg = mark.getBeg();
		auto end = mark.getEnd();
		if (beg > end)
			std::swap(beg, end);
		text.erase(beg, end - beg);
		setCoursorPosition(beg);
		textOutput.setString(text);
		mark.activated = false;
		mark.clear();
		return -1;
	}
}
void inputText::setCoursorPosition(std::size_t i) {
	coursorPosition = i;
	coursor.setPosition(textOutput.findCharacterPos(i));
}

bool inputText::isOnButton()
{
	if (sf::Mouse::getPosition(window).x > pos.x && sf::Mouse::getPosition(window).x < pos.x + size.x && sf::Mouse::getPosition(window).y > pos.y && sf::Mouse::getPosition(window).y < pos.y + size.y)
		return true;
	return false;
}
void inputText::checkCoursorPosition()
{
	int mousePosition = sf::Mouse::getPosition().x;
	float min = abs(textOutput.findCharacterPos(0).x - mousePosition);
	int minI = 0;
	for (int i = 0; i <= text.getSize(); i++) {
		float val = abs(textOutput.findCharacterPos(i).x - mousePosition);
		if (val < min) {
			min = val;
			minI = i;
		}
	}
	setCoursorPosition(minI);
}
void inputText::setPosition(int x, int y)
{
	setTextPosition(x, y);
	coursor.setPosition(x, y + 3);
}
void inputText::checkBlink()
{
	if (timer.getElapsedTime().asSeconds() > 0.5) {
		timer.restart();
		blink = !blink;
	}
}
void inputText::findMark()
{
	checkCoursorPosition();
	mark.update(coursorPosition);
}
inputText::~inputText()
{
}


wchar_t inputText::translate(sf::Event::KeyEvent key)
{
	switch (key.code)
	{
	case sf::Keyboard::Key::A:
		return key.shift ? (key.alt ? L'•' : L'A') : (key.alt ? L'π' : L'a');
	case sf::Keyboard::Key::B:
		return key.shift ? L'B' : L'b';
	case sf::Keyboard::Key::C:
		return key.shift ? (key.alt ? L'∆' : L'C') : (key.alt ? L'Ê' : key.control ? char32_t(4) : L'c');
	case sf::Keyboard::Key::D:
		return key.shift ? L'D' : L'd';
	case sf::Keyboard::Key::Q:
		return key.shift ? L'Q' : L'q';
	case sf::Keyboard::Key::E:
		return key.shift ? (key.alt ? L' ' : L'E') : (key.alt ? L'Í' : L'e');
	case sf::Keyboard::Key::F:
		return key.shift ? L'F' : L'f';
	case sf::Keyboard::Key::G:
		return key.shift ? L'G' : L'g';
	case sf::Keyboard::Key::H:
		return key.shift ? L'H' : L'h';
	case sf::Keyboard::Key::I:
		return key.shift ? L'I' : L'i';
	case sf::Keyboard::Key::J:
		return key.shift ? L'J' : L'j';
	case sf::Keyboard::Key::K:
		return key.shift ? L'K' : L'k';
	case sf::Keyboard::Key::L:
		return key.shift ? (key.alt ? L'£' : L'L') : (key.alt ? L'≥' : L'l');
	case sf::Keyboard::Key::M:
		return key.shift ? L'M' : L'm';
	case sf::Keyboard::Key::N:
		return key.shift ? (key.alt ? L'—' : L'N') : (key.alt ? L'Ò' : L'n');
	case sf::Keyboard::Key::O:
		return key.shift ? (key.alt ? L'”' : L'O') : (key.alt ? L'Û' : L'o');
	case sf::Keyboard::Key::P:
		return key.shift ? L'P' : L'p';
	case sf::Keyboard::Key::R:
		return key.shift ? L'R' : L'r';
	case sf::Keyboard::Key::S:
		return key.shift ? (key.alt ? L'å' : L'S') : (key.alt ? L'ú' : L's');
	case sf::Keyboard::Key::T:
		return key.shift ? L'T' : L't';
	case sf::Keyboard::Key::Y:
		return key.shift ? L'Y' : L'y';
	case sf::Keyboard::Key::W:
		return key.shift ? L'W' : L'w';
	case sf::Keyboard::Key::U:
		return key.shift ? L'U' : L'u';
	case sf::Keyboard::Key::X:
		return key.shift ? (key.alt ? L'è' : L'X') : (key.alt ? L'ü' : key.control ? wchar_t(5) : L'x');
	case sf::Keyboard::Key::Z:
		return key.shift ? (key.alt ? L'Ø' : L'Z') : (key.alt ? L'ø' : L'z');
	case sf::Keyboard::Key::V:
		return key.shift ? L'V' : (key.control ? wchar_t(1) : L'v');
	case sf::Keyboard::Key::Add:
		return '+';
	case sf::Keyboard::Key::Space:
		return ' ';
	case sf::Keyboard::Key::BackSpace:
		return '\b';
	case sf::Keyboard::Key::BackSlash:
		return '\\';
	case sf::Keyboard::Key::Dash:
		return '-';
	case sf::Keyboard::Key::Comma:
		return ',';
	case sf::Keyboard::Key::Divide:
		return '/';
	case sf::Keyboard::Key::Equal:
		return '=';
	case sf::Keyboard::Key::Left:
		return wchar_t(3);
	case sf::Keyboard::Key::Multiply:
		return '*';
	case sf::Keyboard::Key::Num0:
		return '0';
	case sf::Keyboard::Key::Num1:
		return key.shift ? L'!' : L'1';
	case sf::Keyboard::Key::Num2:
		return key.shift ? L'@' : L'2';
	case sf::Keyboard::Key::Num3:
		return key.shift ? L'#' : L'3';
	case sf::Keyboard::Key::Num4:
		return key.shift ? L'$' : L'4';
	case sf::Keyboard::Key::Num5:
		return key.shift ? L'%' : L'5';
	case sf::Keyboard::Key::Num6:
		return key.shift ? L'^' : L'6';
	case sf::Keyboard::Key::Num7:
		return key.shift ? L'&' : L'7';
	case sf::Keyboard::Key::Num8:
		return key.shift ? L'*' : L'8';
	case sf::Keyboard::Key::Num9:
		return key.shift ? L'(' : L'9';
	case sf::Keyboard::Key::Numpad0:
		return key.shift ? L')' : L'0';
	case sf::Keyboard::Key::Numpad1:
		return '1';
	case sf::Keyboard::Key::Numpad2:
		return '2';
	case sf::Keyboard::Key::Numpad3:
		return '3';
	case sf::Keyboard::Key::Numpad4:
		return '4';
	case sf::Keyboard::Key::Numpad5:
		return '5';
	case sf::Keyboard::Key::Numpad6:
		return '6';
	case sf::Keyboard::Key::Numpad7:
		return '7';
	case sf::Keyboard::Key::Numpad8:
		return '8';
	case sf::Keyboard::Key::Numpad9:
		return '9';
	case sf::Keyboard::Key::Right:
		return wchar_t(2);
	case sf::Keyboard::Key::Slash:
		return '/';
	case sf::Keyboard::Key::Tilde:
		return '~';
	case sf::Keyboard::Key::Quote:
		return '\'';
	case sf::Keyboard::Key::SemiColon:
		return ';';
	case sf::Keyboard::Key::Subtract:
		return '-';
	case sf::Keyboard::Key::RBracket:
		return ']';
	case sf::Keyboard::Key::LBracket:
		return '[';
	case sf::Keyboard::Key::Period:
		return '.';
	case sf::Keyboard::Key::Enter:
		return '\n';
	default:
		return 0;
	}
	return 0;
}