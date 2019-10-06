#include "chat.h"
#include <algorithm>
void chat::scrolled(int delta)
{
	if (scrollable && box.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
		while (delta) {
			if (delta > 0) {
				if (moveDown())
					scroll.move(true);
				delta--;
			}
			else if (delta < 0) {
				if (moveUp())
					scroll.move(false);//up
				delta++;
			}
		}
	}
}

bool chat::moveUp()
{
	if (visible.back() == container.size() - 1)
		return false;
	int i;
	for (i = 0; i < visible.size() - 1; i++) {
		visible[i] = visible[i + 1];
	}
	visible[i]++;
	setContainerPositions();
	return true;
}

bool chat::moveDown()
{
	if (visible.front() == 0)
		return false;
	int i;
	for (i = visible.size() - 1; i > 0; i--) {
		visible[i] = visible[i - 1];
	}
	visible[i]--;
	setContainerPositions();
	return true;
}

void chat::reset()
{
	int i = container.size() - 1;
	for (auto it = visible.rbegin(); it != visible.rend(); it++) {
		if (i < 0) {
			*it = -1;
		}
		else {
			*it = i;
			i--;
		}
	}
}

void chat::setContainerPositions() {
	for (int i = 0; i < visible.size(); i++) {
		if (visible[i] != -1)
			container[visible[i]].setPosition(containerPositions[i]);
	}
}
void chat::checkState()
{
}

bool chat::addChar(sf::Event::KeyEvent h)
{
	if (textBar.addChar(h)) {
		this->operator<<(textBar.getText());
		textBar.reset();
		return true;
	}
	return false;
}


void chat::computeText(sf::String & string)
{
	if (container.size() > 200)
		container.erase(container.begin(), container.begin() + 50);
	addTime(string);
	test.setString(string);
	float max = rightSide.getPosition().x - box.getPosition().x - 2 * charSize_;
	int i, beg, end, length = string.getSize() - 1;
	i = beg = end = 0;
	while (end <= length) {//cos tam 
		float len = 0;
		float start = test.findCharacterPos(beg).x;
		for (i = beg; len < max; i++) {
			len = test.findCharacterPos(i).x - start;
			if (string[i] == U' ')
				end = i;
			if (i == length) {
				end = i;
				break;
			}
		}
		if (end == beg)
			end = i;
		end++;
		container.push_back(sf::Text(string.substring(beg, end - beg), font, charSize_));
		container.back().setFillColor(sf::Color::Black);
		beg = end;
	}
	reset();
	setContainerPositions();
	if (container.size() > visible.size()) {
		scrollable = true;
		computeScrollBarSize();
	}
}

void chat::addTime(sf::String & string)
{
	tm timeVal;
	time_t t = time(NULL);
	if (localtime_s(&timeVal, &t)) {
		timeVal.tm_hour = 0;
		timeVal.tm_min = 0;
		timeVal.tm_sec = 0;
	}
	std::string tmp;
	sf::String time = "[";
	tmp = std::to_string(timeVal.tm_hour);
	time += tmp.size() == 1 ? tmp.insert(0, "0") : tmp;
	time += ':';
	tmp = std::to_string(timeVal.tm_min);
	time += tmp.size() == 1 ? tmp.insert(0, "0") : tmp;
	time += ':';
	tmp = std::to_string(timeVal.tm_sec);
	time += tmp.size() == 1 ? tmp.insert(0, "0") : tmp;
	time += "]";
	string.insert(0, time);
}

void chat::checkSideBarState()
{
	if (scrollable) {
		scroll.checkState();
		int diff = scroll.sliderFunction();
		if (!diff)
			return;
		else {
			move(diff);
		}
	}
}
void chat::move(int delta)
{
	while (delta) {
		if (delta > 0) {
			moveDown();
			delta--;
		}
		else if (delta < 0) {
			moveUp();
			delta++;
		}
	}
}
void chat::computeScrollBarSize()
{
	double height = box.getSize().y;
	double diff = (container.size()) * (charSize_ + 5);
	diff = height / diff * height;
	if (diff > 30)
		scroll.setSize(sf::Vector2f(charSize_, diff));
	double diff2 = height - scroll.getSize().y;
	scroll.setStep(diff2 / (container.size() - visible.size()));
	double downBound = rightSide.getPosition().y + diff2;
	scroll.setMaxDelta(container.size() - visible.size());
	scroll.reset();
}

chat::chat(sf::RenderWindow & win, sf::SoundBuffer & click_, int charLimit, int chatlines, sf::Font & font_)
	: window(win), textBar(win, click_, charLimit), font(font_), scroll(window, click_)
{
	for (int i = 0; i < chatlines; i++) {
		visible.push_back(-1);
	}
	scrollable = false;
	chatLines = chatlines;
	test.setPosition(0, 0);
	test.setFont(font_);
	test.setFillColor(sf::Color::Black);
}

void chat::setValues(sf::Vector2f position, int charSize, int widen)
{
	charSize_ = charSize;
	int outline = 10;
	test.setCharacterSize(charSize);

	box.setPosition(position);
	box.setSize(sf::Vector2f(widen, chatLines * (charSize + 5)));
	box.setFillColor(sf::Color(240, 240, 240, 128));
	box.setOutlineColor(sf::Color(195, 195, 195, 255));
	box.setOutlineThickness(charSize);
	sf::Vector2f topp(position.x - outline, position.y - charSize);

	inputBox.setPosition(sf::Vector2f(position.x, position.y + outline + box.getSize().y));
	inputBox.setSize(sf::Vector2f(widen, charSize + 10));
	inputBox.setFillColor(sf::Color(240, 240, 240, 255));
	inputBox.setOutlineColor(sf::Color(195, 195, 195, 255));
	inputBox.setOutlineThickness(outline);

	inputLeftTerm.setSize(sf::Vector2f(charSize, 2 * outline + charSize + 10));
	inputLeftTerm.setPosition(sf::Vector2f(position.x - charSize, position.y + box.getSize().y));
	inputLeftTerm.setFillColor(sf::Color(195, 195, 195, 255));

	inputRightTerm.setSize(sf::Vector2f(charSize, 2 * outline + charSize + 10));
	inputRightTerm.setPosition(sf::Vector2f(position.x + inputBox.getSize().x, position.y + box.getSize().y));
	inputRightTerm.setFillColor(sf::Color(195, 195, 195, 255));

	textBar.setPosition(position.x, position.y + outline + box.getSize().y);
	sf::Vector2f pos(position.x, position.y + outline + box.getSize().y);
	sf::Vector2f sizeP(inputBox.getSize().x - charSize, inputBox.getSize().y);
	textBar.setBounds(pos, sizeP);
	textBar.setString("");
	textBar.setColor();
	textBar.setFont(font);
	textBar.setSize(charSize);
	textBar.setBounds(inputLeftTerm.getPosition().x, inputRightTerm.getPosition().x);

	rightSide.setSize(sf::Vector2f(charSize, box.getSize().y)); // init scroll
	rightSide.setPosition(sf::Vector2f(position.x + box.getSize().x - charSize, position.y));
	rightSide.setFillColor(sf::Color(205, 205, 205, 255));

	scroll.setSize(sf::Vector2f(charSize, box.getSize().y)); // init scroll
	scroll.setPosition(sf::Vector2f(position.x + box.getSize().x - charSize, position.y));
	scroll.setFillColor(sf::Color(127, 127, 127, 255));

	scroll.setXcoord(rightSide.getPosition().x);
	scroll.setOrigin(rightSide.getPosition().y);
	for (int i = 0; i < visible.size(); i++) {
		containerPositions.push_back(sf::Vector2f(3 + position.x, position.y + i * (charSize + 5)));
	}
}
void chat::draw()
{
	window.draw(box);
	for (auto x : visible) {
		if (x != -1)
			window.draw(container[x]);
	}
	window.draw(inputBox);
	textBar.draw();
	window.draw(inputLeftTerm);
	window.draw(inputRightTerm);
	window.draw(rightSide);
	window.draw(scroll);

}


chat::~chat()
{

}
