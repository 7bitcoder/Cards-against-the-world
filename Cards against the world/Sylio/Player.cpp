#include "Player.h"
#include <cmath>
#include <iostream>
#include <fstream>
//kat zmniejsza sie zgodnie z wsk zegara

/*
long long 64
koding hitbox
7 (255) - rest
8 (255) - actual velocity
9 (512) - actual angle
17 (131 000) - id globalnego
12 (4 000) - id fragment
4 (255) - radious
4 (255) - id player
1 (1) - background for boost
1 (1) - tmp range of player head
1 (1) - boost Position
*/
double Player::rScan = 40;
bool Player::update()
{
	if (!dead)
	{
		sf::Time t = time.getElapsedTime();
		time.restart();
		if (activeBoost)
		{
			boostHeadTime += t.asSeconds() * boostTimeVel;
			double p = (0.2 * sin(boostHeadTime) + 0.8);
			double q = 1 - p;
			head.setFillColor(sf::Color(blendColor.r * p + headCol.r * q, blendColor.g * p + headCol.g * q, blendColor.b * p + headCol.b * q));
		}
		if (!setting.TimeStop && !freeze)
		{
			double r = velocity * t.asSeconds();
			if (sf::Keyboard::isKeyPressed(left) && !lockLeft)
				if (sf::Keyboard::isKeyPressed(right) && !lockRight);
				else
					angle += angleVelocity * t.asSeconds();
			else if (sf::Keyboard::isKeyPressed(right) && !lockRight)
				angle -= angleVelocity * t.asSeconds();
			//std::cout << pointx << "    " << pointy << "\n";
			if (angle > 4 * NINETY_DEG)
				angle -= 4 * NINETY_DEG;
			position.x += r * sin(angle);
			position.y += r * cos(angle);
			head.setPosition(position.x * setting.xScale, position.y * setting.yScale);
			int diff = round((oldPosition.x - position.x) * (oldPosition.x - position.x) + (oldPosition.y - position.y) * (oldPosition.y - position.y));
			if (diff > 4)
			{

				if (trace.getState() && actualGap > nextGap)
				{
					actualGap = 0;
					trace.edge(false, headR, angle, position);
					fullFillResizeR(headR);
					trace.stop();
				}
				else if (!trace.getState() && actualGap > gapSize)
				{
					actualGap = 0;
					setNewGap();
					updateTrace();
					trace.edge(true, headR, angle, position);
					fullFillResizeR(headR);
					trace.start();
				}
				else if (trace.getState()) {
					updateTrace();
					roundPos();
					if (!Scan())
						return true;
					drawLineOnHitBox(round(oldPosition.x), round(oldPosition.y));
					fullFillForBoost(trace.getLastPos(), trace.getLastLastPos(), trace.getLastDPos(), trace.getLastLastDPos());
				}
				if (resize)
				{
					changeRadious(waitingR);
					resize = false;
				}
				actualGap += diff;
				oldPosition = position;
			}
		}
	}
	return false;
}

Player::Player(std::vector<double> & headVec_, std::array<std::array<long long int, 1920>, 1080> & hitbox_, sf::RenderWindow & win, sf::Color col, int& ymax_, int& ymin_, int& xmax_, int& xmin_) :
	window(win),
	xmax(xmax_),
	xmin(xmin_),
	ymax(ymax_),
	ymin(ymin_),
	hitbox(hitbox_),
	headR(0),
	angle(0),
	headVec(headVec_),
	trace(win, col, 8000000, 10000)
{
	boostTimeVel = 6;
	blendColor = sf::Color::White;
	boostHeadTime = 0;
	activeBoost = false;
	visible = true;
	freeze = false;
	dead = false;
	lockLeft = false;
	lockRight = false;
	time.restart();
	setColor(col);
	actualGap = 0;
	resize = false;
	crossBounds = false;
	immortal = false;
	nextGapOffset = 1;
	gapOffset = 1;
	hiddenGapOffset = 1;
	hiddenNextGapOffset = 1;
	points = 0;
	head.setScale(setting.xScale, setting.yScale);
}
void Player::reset()
{
	head.setFillColor(headCol);
	activeBoost = false;
	visible = true;
	freeze = false;
	dead = false;
	lockLeft = false;
	lockRight = false;
	time.restart();
	resize = false;
	crossBounds = false;
	immortal = false;
	nextGapOffset = 1;
	gapOffset = 1;
	hiddenGapOffset = 1;
	hiddenNextGapOffset = 1;
	erise();
}
void Player::changeHead(double angle_, sf::Vector2f pos)
{
	actualGap = 0;
	trace.edge(false, headR, angle, position);
	fullFillResizeR(headR);
	trace.stop();
	oldPosition = pos;
	position = oldPosition;
	angle = angle_;
	head.setPosition(position.x * setting.xScale, position.y * setting.yScale);
	setNewGap();
	updateTrace();
	trace.edge(true, headR, angle, position);
	fullFillResizeR(headR);
	trace.start();
}
bool Player::checkBounds()
{
	if (!crossBounds)
	{
		if (position.x - headR < xmin || position.x + headR > xmax || position.y + headR > ymax || position.y - headR < ymin) {
			die();
			return true;
		}
		return false;
	}
	else
	{
		if (position.x - headR < xmin)
		{
			actualGap = 0;
			trace.edge(false, headR, angle, position);
			fullFillResizeR(headR);
			trace.stop();
			oldPosition = sf::Vector2f(xmax - headR, position.y);
			position = oldPosition;
			head.setPosition(position.x * setting.xScale, position.y * setting.yScale);
			setNewGap();
			updateTrace();
			trace.edge(true, headR, angle, position);
			fullFillResizeR(headR);
			trace.start();
		}
		else if (position.x + headR > xmax)
		{
			actualGap = 0;
			trace.edge(false, headR, angle, position);
			fullFillResizeR(headR);
			trace.stop();
			oldPosition = sf::Vector2f(xmin + headR, position.y);
			position = oldPosition;
			head.setPosition(position.x * setting.xScale, position.y * setting.yScale);
			setNewGap();
			updateTrace();
			trace.edge(true, headR, angle, position);
			fullFillResizeR(headR);
			trace.start();
		}
		else if (position.y + headR > ymax)
		{
			actualGap = 0;
			trace.edge(false, headR, angle, position);
			fullFillResizeR(headR);
			trace.stop();
			oldPosition = sf::Vector2f(position.x, ymin + headR);
			position = oldPosition;
			head.setPosition(position.x * setting.xScale, position.y * setting.yScale);
			setNewGap();
			updateTrace();
			trace.edge(true, headR, angle, position);
			fullFillResizeR(headR);
			trace.start();
		}
		else if (position.y - headR < ymin)
		{
			actualGap = 0;
			trace.edge(false, headR, angle, position);
			fullFillResizeR(headR);
			trace.stop();
			oldPosition = sf::Vector2f(position.x, ymax - headR);
			position = oldPosition;
			head.setPosition(position.x * setting.xScale, position.y * setting.yScale);
			setNewGap();
			updateTrace();
			trace.edge(true, headR, angle, position);
			fullFillResizeR(headR);
			trace.start();
		}
		return false;
	}
}
Player::~Player()
{
	clearBoosts();
}

void Player::addBoost(Boost * bost_)
{
	std::vector<Boost*>::iterator it;
	for (it = boosts.begin(); it != boosts.end(); it++)
	{
		if (typeid(*bost_) == typeid(*(*it)))
		{
			break;
		}
	}
	if (it != boosts.end())
	{
		if (bost_->stack())
		{
			(*it)->addDuration(bost_->getDuration());
			delete bost_;
		}
		else
		{
			boosts.push_back(bost_);
			boosts.back()->setBoost(*this);
		}
	}
	else
	{
		boosts.push_back(bost_);
		boosts.back()->setBoost(*this);
	}
	if (boosts.size() > 0)
	{
		if (!activeBoost)
		{
			activeBoost = true;
			boostHeadTime = 0;
		}
	}
}

void Player::checkBoosts()
{
	std::vector<Boost*>::iterator it;
	bool found = false;
	for (auto itt = boosts.begin(); itt != boosts.end(); itt++)
	{
		if ((*itt)->check(*this))
		{
			found = true;
			it = itt;
			(*itt)->clearBoost(*this);
			delete (*itt);
			break;
		}
	}
	if (found)
	{
		boosts.erase(it);
		if (!boosts.size())
		{
			activeBoost = false;
			head.setFillColor(headCol);
		}
	}
}

void Player::setNewGap()
{
	gapSize = gapBounds.x*gapOffset + rand() % (int((gapBounds.y - gapBounds.x)*gapOffset));
	nextGap = (NextGapounds.x)*nextGapOffset + rand() % (int((NextGapounds.y - NextGapounds.x)*nextGapOffset));
}
void Player::drawLineOnHitBox(int x1, int y1)
{
	int x0 = roundPosition.x;
	int y0 = roundPosition.y;
	int val = playerId << 28;
	val |= int(round(headR));

	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;  /* error value e_xy */
	while (true)   /* loop */
	{
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy; /* e_xy+e_x > 0 */
			x0 += sx;
		}
		if (e2 <= dx) /* e_xy+e_y < 0 */
		{
			err += dx;
			y0 += sy;
		}
		hitbox[y0][x0] = val;
	}
}
bool Player::Scan()
{
	int leftpointx = round(headR * sin(angle + SEE_RANGE) + position.x);
	int leftpointy = round(headR * cos(angle + SEE_RANGE) + position.y);

	int rightpointx = round(headR * sin(angle - SEE_RANGE) + position.x);
	int rightpointy = round(headR * cos(angle - SEE_RANGE) + position.y);

	/*int lx = round(L.x);
	int ly = round(L.y);
	int rx = round(R.x);
	int ry = round(R.y);


	int B = lx - rx;//-dx
	 int A = ry - ly;//dy
	 int C = ly*(-B) - lx *(A);

	*/
	int BLc = leftpointx - roundPosition.x;//-dx
	int ALc = roundPosition.y - leftpointy;//dy
	int CLc = leftpointy * (-BLc) - leftpointx * (ALc);

	int BRc = roundPosition.x - rightpointx;//-dx
	int ARc = rightpointy - roundPosition.y;//dy
	int CRc = roundPosition.y * (-BRc) - roundPosition.x * (ARc);

	int r = round(rScan);

	int begx = roundPosition.x - r;
	if (begx < xmin)
		begx = xmin;
	int begy = roundPosition.y - r;
	if (begy < ymin)
		begy = ymin;
	int endx = roundPosition.x + r;
	if (endx > xmax)
		endx = xmax;
	int endy = roundPosition.y + r;
	if (endy > ymax)
		endy = ymax;

	for (int x = begx; x <= endx; x++)
	{
		for (int y = begy; y <= endy; y++)
		{
			if (ALc * x + BLc * y + CLc >= 0 || ARc * x + BRc * y + CRc >= 0)
			{
				//std::cout << 'x';
				if (!hitbox[y][x])
					continue;
				else if (hitbox[y][x] >> 61)
					continue;
				else
				{
					if (!immortal)
					{
						int hitboxVal = hitbox[y][x];
						int id = hitboxVal >> 28;
						int R = hitboxVal & 0xFFFFF;
						if (double((R + headR) * (R + headR)) > (double(roundPosition.x) - x) * (double(roundPosition.x) - x) + (double(roundPosition.y) - y) * (double(roundPosition.y) - y))
						{
							die();
							/*sf::Image im;
							im.create(1920, 1080, sf::Color::Black);
							for (int i = 1; i < 1080; i++)
							{
								for (int j = 1; j < 1920; j++)
								{
									if (!hitbox[i][j])
										im.setPixel(j, i, sf::Color::Black);
									else if (hitbox[i][j] >> 61)
										im.setPixel(j, i, sf::Color::Green);
									else
									{
										if (j == roundPosition.x && i == roundPosition.y)
											im.setPixel(j, i, sf::Color::Blue);
										else if (j == x && i == y)
											im.setPixel(j, i, sf::Color::Cyan);
										else
											im.setPixel(j, i, sf::Color::Yellow);
									}
								}
							}
							im.saveToFile("hitbox.png");*/
							return false;
						}
					}
				}
			}
			//std::cout << "_";
		}
		//std::cout << std::endl;
	}
	return true;
	//std::cout << std::endl;
}

inline void Player::updateTrace()
{
	float pointlx = headR * sin(angle + NINETY_DEG);
	float pointly = headR * cos(angle + NINETY_DEG);
	sf::Vector2f pointx(position.x + pointlx, position.y + pointly);
	sf::Vector2f pointy(position.x - pointlx, position.y - pointly);
	trace.update(pointx, pointy);
}

void Player::erise()
{
	trace.erise();
	setPosition(position);
	actualGap = 0;
	setNewGap();
}

bool Player::changeRadious(double R)
{
	if (R == headR)
		return true;
	if (trace.getState())
	{
		if (R > headR) {
			trace.edge(true, R, angle, position);
			fullFillResizeR(R);
		}
		else {
			trace.edge(false, headR, angle, position);
			fullFillResizeR(headR);
		}

	}
	headR = R;
	head.setRadius(R);
	head.setOrigin(R, R);
	boosthead.setRadius(R);
	boosthead.setOrigin(R, R);
	headVec[playerId] = R;
	for (auto& x : headVec)
	{
		if (x > rScan - 3)
			rScan = x + 3;
	}
	return true;
}
bool Player::triangleTest(sf::Vector2i s, sf::Vector2i a, sf::Vector2i b, sf::Vector2i c)
{
	int AB_A, AC_A, CB_A, AB_C, AC_C, CB_C, AB_B, AC_B, CB_B;
	AB_A = a.y - b.y;//max1000
	AB_B = b.x - a.x;//max1000
	AB_C = a.x * (b.y - a.y) - a.y * (b.x - a.x);//max1 000 000

	AC_A = a.y - c.y;
	AC_B = c.x - a.x;
	AC_C = a.x * (c.y - a.y) - a.y * (c.x - a.x);

	CB_A = c.y - b.y;
	CB_B = b.x - c.x;
	CB_C = c.x * (b.y - c.y) - c.y * (b.x - c.x);

	if ((AB_A * s.x + AB_B * s.y + AB_C) * (AB_A * c.x + AB_B * c.y + AB_C) < 0 ||
		(AC_A * s.x + AC_B * s.y + AC_C) * (AC_A * b.x + AC_B * b.y + AC_C) < 0 ||
		(CB_A * s.x + CB_B * s.y + CB_C) * (CB_A * a.x + CB_B * a.y + CB_C) < 0)
		return false;
	else if ((AB_A * s.x + AB_B * s.y + AB_C) * (AB_A * c.x + AB_B * c.y + AB_C) == 0 ||
		(AC_A * s.x + AC_B * s.y + AC_C) * (AC_A * b.x + AC_B * b.y + AC_C) == 0 ||
		(CB_A * s.x + CB_B * s.y + CB_C) * (CB_A * a.x + CB_B * a.y + CB_C) == 0)
		;
	return true;
}

void Player::fullFillForBoost(sf::Vector2f actR, sf::Vector2f actL, sf::Vector2f lasR, sf::Vector2f lasL)
{

	long long int info;
	info = long long int(1) << 61;
	info |= long long int(playerId) << 57;
	sf::Vector2i tactualRight(round(actR.x / setting.xScale), round(actR.y / setting.yScale));
	sf::Vector2i tactualLeft(round(actL.x / setting.xScale), round(actL.y / setting.yScale));
	sf::Vector2i tlastRight(round(lasR.x / setting.xScale), round(lasR.y / setting.yScale));
	sf::Vector2i tlastLeft(round(lasL.x / setting.xScale), round(lasL.y / setting.yScale));
	int minx = std::min(std::min(tactualLeft.x, tactualRight.x), std::min(tlastLeft.x, tlastRight.x));
	int maxx = std::max(std::max(tactualLeft.x, tactualRight.x), std::max(tlastLeft.x, tlastRight.x));
	int miny = std::min(std::min(tactualLeft.y, tactualRight.y), std::min(tlastLeft.y, tlastRight.y));
	int maxy = std::max(std::max(tactualLeft.y, tactualRight.y), std::max(tlastLeft.y, tlastRight.y));
	for (int x = minx; x <= maxx; x++)
	{
		for (int y = miny; y <= maxy; y++)
		{
			if (hitbox[y][x])
				continue;
			else
			{
				if (triangleTest({ x,y }, tlastLeft, tlastRight, tactualRight) || triangleTest({ x,y }, tactualLeft, tactualRight, tlastLeft)) {
					hitbox[y][x] = info;
				}
			}
		}
	}
}

void Player::fullFillResizeR(double R)
{
	int begx = round(position.x - R);
	if (begx < xmin)
		begx = xmin;
	int begy = round(position.y - R);
	if (begy < ymin)
		begy = ymin;
	int endx = round(position.x + R);
	if (endx > xmax)
		endx = xmax;
	int endy = round(position.y + R);
	if (endy > ymax)
		endy = ymax;

	long long int info;
	info = long long int(1) << 61;
	info |= long long int(playerId) << 57;

	for (int x = begx; x <= endx; x++)
	{
		for (int y = begy; y <= endy; y++)
		{
			if (hitbox[y][x])
				continue;
			else
			{
				if (((x - position.x) * (x - position.x) + (y - position.y) * (y - position.y)) < R * R)
				{
					hitbox[y][x] = info;
				}
			}
		}
	}
}
