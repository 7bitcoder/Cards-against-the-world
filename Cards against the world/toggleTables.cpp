#include "toggleTables.h"

bool toggleTables::function()
{
	auto pos = sf::Mouse::getPosition();
	if (!block_ && back.getGlobalBounds().contains(pos.x, pos.y) && !inProgress) {
		toggle();
		click.play();
		return true;
	}
	return false;
}

void toggleTables::forcewNormalTable()
{
	forceNormal = true;
}

void toggleTables::forceChosingTable()
{
	forceChsosing = true;
}

void toggleTables::setPosition(sf::Vector2f pos)
{
	back.setOrigin(18, 18);
	circle.setOrigin(18, 18);
	back.setPosition(pos);
	circle.setPosition(pos);
	title.setOrigin(title.getGlobalBounds().width / 2, 0);
	title.setPosition(pos.x, pos.y - 60);
}

void toggleTables::update()
{
	if (forceChsosing) {
		if (!inProgress) {
			showNormal = false;
			set();
			forceChsosing = false;
		}
	}
	else if (forceNormal) {
		if (!inProgress) {
			showNormal = true;
			set();
			forceNormal = false;
		}
	}
	if (inProgress) {
		chosing->update();
		normal->update();
		inProgress = !chosing->end() || !normal->end();
	}

}

void toggleTables::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(title);
	if (!showNormal) {
		normal->draw();
		chosing->draw();
		target.draw(back);
	}
	else {
		chosing->draw();
		normal->draw();
		target.draw(circle);
	}
}

toggleTables::~toggleTables()
{
}
