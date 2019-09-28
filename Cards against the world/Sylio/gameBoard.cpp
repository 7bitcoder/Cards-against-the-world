#include "gameBoard.h"



std::array<std::array<long long int, 1920>, 1080> gameBoard::hitbox = { 0 };
gameBoard::gameBoard(sf::RenderWindow& win, Background& back) :
	window(win),
	background(back),
	Players(),
	scoreBoard(window, 30, 500)
{
	this->pause = false;
	defxMin = 300;
	defyMax = 1075;
	defxMax = 1915;
	defyMin = 5;
	setBounds(defyMax, defyMin, defxMax, defxMin, defyMin);
	srand(std::time(0));
	boostR = 40;
	dontSetBoostR = 50;
	minBoostTime = 5;
	maxBoostTime = 10;
	boundRounds = 0;
	boundRange = 100;
	boundVel = 0.5;
	timeFlag = false;
	duration = 0;
	boostTime = getTimeBoost();
	if (!blind.loadFromFile("../boost_icons/blind.png"))
		throw std::exception("boost icon missing");
	if (!colapseBounds.loadFromFile("../boost_icons/bounds.png"))
		throw std::exception("boost icon missing");
	if (!brokenWalls.loadFromFile("../boost_icons/broken_walls.png"))
		throw std::exception("boost icon missing");
	if (!brokenWallsGreen.loadFromFile("../boost_icons/broken_walls_green.png"))
		throw std::exception("boost icon missing");
	if (!clearAll.loadFromFile("../boost_icons/clean_board.png"))
		throw std::exception("boost icon missing");
	if (!freeze.loadFromFile("../boost_icons/freeze.png"))
		throw std::exception("boost icon missing");
	if (!freezeRed.loadFromFile("../boost_icons/freeze_red.png"))
		throw std::exception("boost icon missing");
	if (!growUp.loadFromFile("../boost_icons/grow_up.png"))
		throw std::exception("boost icon missing");
	if (!growUpRed.loadFromFile("../boost_icons/grow_up_red.png"))
		throw std::exception("boost icon missing");
	if (!immortalGreen.loadFromFile("../boost_icons/immortal_green.png"))
		throw std::exception("boost icon missing");
	if (!longerGaps.loadFromFile("../boost_icons/longer_gaps.png"))
		throw std::exception("boost icon missing");
	if (!longerGapsGreen.loadFromFile("../boost_icons/longer_gaps_green.png"))
		throw std::exception("boost icon missing");
	if (!moreOftenHoles.loadFromFile("../boost_icons/more_often_holes.png"))
		throw std::exception("boost icon missing");
	if (!moreOftenHolesGreen.loadFromFile("../boost_icons/more_often_holes_green.png"))
		throw std::exception("boost icon missing");
	if (!lockLeftRed.loadFromFile("../boost_icons/only_left_red.png"))
		throw std::exception("boost icon missing");
	if (!lockRightRed.loadFromFile("../boost_icons/only_right_red.png"))
		throw std::exception("boost icon missing");
	if (!shrink.loadFromFile("../boost_icons/shrink.png"))
		throw std::exception("boost icon missing");
	if (!shrinkGreen.loadFromFile("../boost_icons/shrink_green.png"))
		throw std::exception("boost icon missing");
	if (!slowDown.loadFromFile("../boost_icons/slow_down.png"))
		throw std::exception("boost icon missing");
	if (!slowDownRed.loadFromFile("../boost_icons/slow_down_red.png"))
		throw std::exception("boost icon missing");
	if (!slowDownGreen.loadFromFile("../boost_icons/slow_down_green.png"))
		throw std::exception("boost icon missing");
	if (!speedUp.loadFromFile("../boost_icons/speed_up.png"))
		throw std::exception("boost icon missing");
	if (!speedUpRed.loadFromFile("../boost_icons/speed_up_red.png"))
		throw std::exception("boost icon missing");
	if (!speedUpGreen.loadFromFile("../boost_icons/speed_up_green.png"))
		throw std::exception("boost icon missing");
	if (!switchControlsRed.loadFromFile("../boost_icons/switch_controls_red.png"))
		throw std::exception("boost icon missing");
	if (!switchHeads.loadFromFile("../boost_icons/switch_head.png"))
		throw std::exception("boost icon missing");
	if (!font.loadFromFile("../Font/kenvector_future_thin.ttf"))
		throw std::exception("font file missing");

	startUpText.setFont(font);
	startUpText.setCharacterSize(80);
	startUpText.setFillColor(sf::Color::White);
	startUpText.setPosition((xmin + (xmax - xmin) / 2) * setting.xScale, (ymin + (ymax - ymin) / 2 - 100) * setting.yScale);
	startUpText.setScale(setting.xScale, setting.yScale);

	Winner.setFont(font);
	Winner.setCharacterSize(80);
	Winner.setFillColor(sf::Color::White);
	Winner.setPosition((xmin + (xmax - xmin) / 2) * setting.xScale, (ymin + (ymax - ymin) / 2 - 100) * setting.yScale);
	Winner.setScale(setting.xScale, setting.yScale);

	roundsText.setFont(font);
	roundsText.setCharacterSize(40);
	roundsText.setFillColor(sf::Color::White);
	roundsText.setPosition(20 * setting.xScale, 200 * setting.yScale);
	roundsText.setScale(setting.xScale, setting.yScale);
}

st gameBoard::update()
{

	sf::Text fps;
	fps.setFont(font);
	fps.setCharacterSize(35);
	fps.setPosition(20 * setting.xScale, 20 * setting.yScale);
	fps.setScale(setting.xScale, setting.yScale);
	fps.setFillColor(sf::Color::White);

	sf::Clock timer;
	sf::Clock boostTImer;

	if (!setting.pause)
	{
		clearHitbox();
		clearBoosts();
		createPlayers();
		scoreBoard.setPosition(25 * setting.xScale, 400 * setting.yScale, Players, font);
		AllRounds = setting.rounds;
		rounds = AllRounds;
	}
	int cnt = 0;
	sf::Event event;

	while (rounds)
	{
		timer.restart();
		boostTImer.restart();
		if (!setting.pause)
		{
			end = false;
			start = true;
			poolPoints = 1;
			setting.TimeStop = true;
			spacePressed = false;
			sec = 3;
			timeFlag = false;
			duration = 0;
			boundRounds = 0;
			setBounds(defyMax, defyMin, defxMax, defxMin, 5);
			roundsText.setString("Round " + std::to_string(AllRounds - rounds + 1) + " / " + std::to_string(AllRounds));
			startUpText.setString("press space to start");
			startUpText.setOrigin(startUpText.getGlobalBounds().width / 2, startUpText.getGlobalBounds().height / 2);
		}
		while (window.isOpen())
		{
			if (start)
			{
				if (!spacePressed)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
					{
						spacePressed = true;
						startUpText.setString("round " + std::to_string(AllRounds - rounds + 1));
						startUpText.setScale(setting.xScale, setting.yScale);
						startUpText.setOrigin(startUpText.getGlobalBounds().width / 2, startUpText.getGlobalBounds().height / 2);
						boostTImer.restart();
					}
				}
				else if (boostTImer.getElapsedTime().asSeconds() > 1)
				{
					startUpText.setString(std::to_string(sec));
					startUpText.setScale(setting.xScale, setting.yScale);
					startUpText.setOrigin(startUpText.getGlobalBounds().width / 2, startUpText.getGlobalBounds().height / 2);
					boostTImer.restart();
					if (sec == 0)
					{
						start = false;
						setting.TimeStop = false;
						continue;
					}
					sec--;
				}

			}
			else if (end)
			{
				if (boostTImer.getElapsedTime().asSeconds() > 3)
				{
					rounds--;
					restart();
					break;
				}
			}
			if (cnt == 200)
			{
				fps.setString(std::to_string(int(cnt / timer.getElapsedTime().asSeconds())));
				timer.restart();
				cnt = 0;
			}
			if (poolPoints == Players.size())
			{
				poolPoints++;
				for (auto& winner : Players)
				{
					if (!winner.getState())
					{
						end = true;
						winner.die();
						winner.addPoints(poolPoints);
						scoreBoard.updateScore(winner.getId(), winner);
						Winner.setString("Round winner: " + winner.getNickname());
						Winner.setScale(setting.xScale, setting.yScale);
						Winner.setOrigin(Winner.getGlobalBounds().width / 2, Winner.getGlobalBounds().height / 2);
						boostTImer.restart();
					}
				}

			}
			updatePlayers();
			scoreBoard.update();
			checkBoostsColission();
			if (boundRounds)
				boundFunction();
			cnt++;
			if (!start && !end && boostTImer.getElapsedTime().asSeconds() > boostTime)
			{
				boostTImer.restart();
				if (!setting.TimeStop)
				{
					boostTime = getTimeBoost();
					if (boostsOnBoard.size() < 21) {
						drawOnHitTmp();
						if (getBoostPosition())
						{
							markBoostPosOnHit(boostPosition);
							getRandomBoost();
							//dodaj boost;
						}
						clearOnHitTmp();
					}
				}
				//else nie ma miejsca
			}
			window.clear(sf::Color::Black);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				setting.pause = true;
				setting.TimeStop = true;
				updatePlayers();//for  proper time handle when paused
				scoreBoard.update();
				checkBoostsColission();//|^
				if (boundRounds)
					boundFunction();
				return st::pause;
			}
			else if (isF6Pressed())
			{
				EriseAll();
			}
			else if (isF5Pressed())
				setting.TimeStop = !setting.TimeStop;
			else if (isF2Pressed())
			{
				changeHeads();
			}
			else if (isF3Pressed())
			{
				Boost* tmp = new LongerGaps;
				Players.back().addBoost(tmp);
			}
			//background.draw();
			drawBounds();
			drawPlayers();
			window.draw(fps);
			window.draw(roundsText);
			for (auto& aa : boostsOnBoard)
				window.draw(aa);
			if (start)
				window.draw(startUpText);
			else if (end)
				window.draw(Winner);
			scoreBoard.draw();
			window.display();
			if (setting.pause)
			{
				setting.pause = false;
				if (!start)
					setting.TimeStop = false;
			}
		}
	}
	//podsumowanie ranking itp
	return st::mainMenu;
}


gameBoard::~gameBoard()
{
}
void gameBoard::setBounds(int ymax_, int ymin_, int xmax_, int xmin_, int thicc_)
{
	xmax = xmax_;
	xmin = xmin_;
	ymin = ymin_;
	ymax = ymax_;
	thicc = thicc_;
	bounds[0][0] = sf::Vertex(sf::Vector2f((xmax + thicc) * setting.xScale, (ymin - thicc) * setting.yScale), sf::Color::White);
	bounds[0][1] = sf::Vertex(sf::Vector2f(xmax * setting.xScale, ymin * setting.yScale), sf::Color::White);
	bounds[0][2] = sf::Vertex(sf::Vector2f(xmax * setting.xScale, ymax * setting.yScale), sf::Color::White);
	bounds[0][3] = sf::Vertex(sf::Vector2f((xmax + thicc) * setting.xScale, (ymax + thicc) * setting.yScale), sf::Color::White);

	bounds[1][0] = sf::Vertex(sf::Vector2f((xmax + thicc) * setting.xScale, (ymax + thicc) * setting.yScale), sf::Color::White);
	bounds[1][1] = sf::Vertex(sf::Vector2f(xmax * setting.xScale, ymax * setting.yScale), sf::Color::White);
	bounds[1][2] = sf::Vertex(sf::Vector2f(xmin * setting.xScale, ymax * setting.yScale), sf::Color::White);
	bounds[1][3] = sf::Vertex(sf::Vector2f((xmin - thicc) * setting.xScale, (ymax + thicc) * setting.yScale), sf::Color::White);

	bounds[2][0] = sf::Vertex(sf::Vector2f((xmin - thicc) * setting.xScale, (ymax + thicc) * setting.yScale), sf::Color::White);
	bounds[2][1] = sf::Vertex(sf::Vector2f(xmin * setting.xScale, ymax * setting.yScale), sf::Color::White);
	bounds[2][2] = sf::Vertex(sf::Vector2f(xmin * setting.xScale, ymin * setting.yScale), sf::Color::White);
	bounds[2][3] = sf::Vertex(sf::Vector2f((xmin - thicc) * setting.xScale, (ymin - thicc) * setting.yScale), sf::Color::White);

	bounds[3][0] = sf::Vertex(sf::Vector2f((xmin - thicc) * setting.xScale, (ymin - thicc) * setting.yScale), sf::Color::White);
	bounds[3][1] = sf::Vertex(sf::Vector2f(xmin * setting.xScale, ymin * setting.yScale), sf::Color::White);
	bounds[3][2] = sf::Vertex(sf::Vector2f(xmax * setting.xScale, ymin * setting.yScale), sf::Color::White);
	bounds[3][3] = sf::Vertex(sf::Vector2f((xmax + thicc) * setting.xScale, (ymin - thicc) * setting.yScale), sf::Color::White);

}

void gameBoard::clearHitbox()
{
	for (auto& x : hitbox)
		for (auto& y : x)
			y = 0;
}

void gameBoard::createPlayers()
{
	Players.clear();
	thicc = 5;
	int i = 0;
	generateMap();
	for (auto& x : setting.playersSettings)
	{
		Players.push_back(std::move(Player(allHeadRadious, hitbox, window, x.color, ymax, ymin, xmax, xmin)));
		Players.back().setId(i);
		allHeadRadious.push_back(5);
		Players.back().setParams(generteAngle(), 6, 2.5, 100);
		Players.back().setPosition(generatePositions());
		Players.back().setControls(x.left, x.right);
		Players.back().setNick(x.nickname);
		i++;
		Players.back().setGapBounds(40, 300, 500, 1000);

	}
	/*
		Players.push_back(std::move(Player(allHeadRadious, hitbox, window, sf::Color::Red, ymax, ymin, xmax, xmin)));
		Players.back().setId(i);
		allHeadRadious.push_back(5);
		Players.back().setParams(generteAngle(), 6, 2.5, 100);
		Players.back().setPosition(generatePositions());
		Players.back().setControls(sf::Keyboard::Key::S, sf::Keyboard::Key::D);
		Players.back().setNick("sylwow");
		i++;
		Players.back().setGapBounds(40, 300, 500, 1000);
		*/
}

bool gameBoard::isF2Pressed()
{
	static bool isPressed = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
	{
		if (!isPressed)
		{
			isPressed = true;
			return true;
		}
		return false;
	}
	else
	{
		isPressed = false;
		return false;
	}
}
bool gameBoard::isF3Pressed()
{
	static bool isPressed = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
	{
		if (!isPressed)
		{
			isPressed = true;
			return true;
		}
		return false;
	}
	else
	{
		isPressed = false;
		return false;
	}
}
bool gameBoard::isF5Pressed()
{
	static bool isPressed = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
	{
		if (!isPressed)
		{
			isPressed = true;
			return true;
		}
		return false;
	}
	else
	{
		isPressed = false;
		return false;
	}
}
bool gameBoard::isF6Pressed()
{
	static bool isPressed = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F6))
	{
		if (!isPressed)
		{
			isPressed = true;
			return true;
		}
		return false;
	}
	else
	{
		isPressed = false;
		return false;
	}
}
void gameBoard::generateMap()
{
	map.clear();
	int divx = 10;
	int divy = 10;
	int widex = xmax - xmin;
	widex /= divx;
	int widey = ymax - ymin;
	widey /= divy;
	int offx = rand() % widex;
	int offy = rand() % widey;
	divx--;
	divy--;
	for (int i = 1; i < divx; i++)
	{
		for (int j = 1; j < divy; j++)
		{
			map.push_back({ offx + xmin + i * widex, offy + ymin + j * widey });
		}
	}
}
sf::Vector2f gameBoard::generatePositions()
{
	int i = rand() % map.size();
	auto it = map.begin() + i;
	sf::Vector2f tmp;
	tmp.x = it->x;
	tmp.y = it->y;
	map.erase(it);
	return tmp;
}
double gameBoard::generteAngle()
{
	return double(rand() % 360)* 0.0174532925;
}

void gameBoard::EriseId(int id)
{
	Players[id].erise();
	for (int i = 0; i < 1920; i++)
	{
		for (int j = 0; j < 1080; j++)
		{
			if (!hitbox[j][i])
				continue;
			else
			{
				int id_ = hitbox[j][i] >> 28;
				if (id_ == id)
					hitbox[j][i] = 0;
			}
		}
	}
}

void gameBoard::EriseAll()
{
	for (auto& x : Players)
		x.erise();
	for (int i = 0; i < 1920; i++)
	{
		for (int j = 0; j < 1080; j++)
		{
			hitbox[j][i] = 0;
		}
	}
}

bool gameBoard::getBoostPosition()
{
	int X = xmin + int(boostR) + rand() % (xmax - xmin - int(boostR));
	int Y = ymin + int(boostR) + rand() % (ymax - ymin - int(boostR));
	int lowestRint = 2500;
	int lowestRintPow = 2500 * 2500;;
	int boundRX = 1920 + X;
	int boundRY = 1920 + Y;
	sf::Vector2i point;
	int R = boostR;
	int Ymax = ymax - R;
	int Xmax = xmax - R;
	int Ymin = ymin + R;
	int Xmin = xmin + R;
	double actualR;
	for (int x = X; x < Xmax && x < boundRX; x++)
	{
		for (int y = Y; y < Ymax && y < boundRY; y++)
		{
			if (!hitbox[y][x])//scan(x, y, R))
			{
				int r_ = round((X - x) * (X - x) + (Y - y) * (Y - y));
				if (r_ < lowestRintPow)
				{
					lowestRintPow = r_;
					lowestRint = round(sqrt(r_));
					boundRX = lowestRint + X;
					boundRY = lowestRint + Y;
					boostPosition = { x,y };
				}
			}
			else
				continue;
		}
	}
	boundRX = lowestRint + X;
	boundRY = Y - lowestRint;
	for (int x = X; x < Xmax && x < boundRX; x++)
	{
		for (int y = Y; y > Ymin && y > boundRY; y--)
		{
			if (!hitbox[y][x])//scan(x, y, R))
			{
				int r_ = round((X - x) * (X - x) + (Y - y) * (Y - y));
				if (r_ < lowestRintPow)
				{
					lowestRintPow = r_;
					lowestRint = round(sqrt(r_));
					boundRX = lowestRint + X;
					boundRY = Y - lowestRint;
					boostPosition = { x,y };
				}
			}
			else
				continue;
		}
	}
	boundRX = X - lowestRint;
	boundRY = lowestRint + Y;
	for (int x = X; x > Xmin && x > boundRX; x--)
	{
		for (int y = Y; y < Ymax && y < boundRY; y++)
		{
			if (!hitbox[y][x])//scan(x, y, R))
			{
				int r_ = round((X - x) * (X - x) + (Y - y) * (Y - y));
				if (r_ < lowestRintPow)
				{
					lowestRintPow = r_;
					lowestRint = round(sqrt(r_));
					boundRX = X - lowestRint;
					boundRY = lowestRint + Y;
					boostPosition = { x,y };
				}
			}
			else
				continue;
		}
	}
	boundRX = X - lowestRint;
	boundRY = Y - lowestRint;
	for (int x = X; x > Xmin && x > boundRX; x--)
	{
		for (int y = Y; y > Ymin && y > boundRY; y--)
		{
			if (!hitbox[y][x])//scan(x, y, R))
			{
				int r_ = round((X - x) * (X - x) + (Y - y) * (Y - y));
				if (r_ < lowestRintPow)
				{
					lowestRintPow = r_;
					lowestRint = round(sqrt(r_));
					boundRX = X - lowestRint;
					boundRY = Y - lowestRint;
					boostPosition = { x,y };
				}
			}
			else
				continue;
		}
	}
	if (lowestRint == 2500)
		return false;
	else
		return true;
}

void gameBoard::drawOnHitTmp()
{
	for (auto& x : Players) {
		sf::Vector2f position = x.getPos();

		int begx = round(position.x - dontSetBoostR);
		if (begx < xmin)
			begx = xmin;
		int begy = round(position.y - dontSetBoostR);
		if (begy < ymin)
			begy = ymin;
		int endx = round(position.x + dontSetBoostR);
		if (endx > xmax)
			endx = xmax;
		int endy = round(position.y + dontSetBoostR);
		if (endy > ymax)
			endy = ymax;

		long long int info;
		info = long long int(1) << 63;

		for (int x = begx; x <= endx; x++)
		{
			for (int y = begy; y <= endy; y++)
			{
				if (hitbox[y][x])
					continue;
				else
				{
					if (((x - position.x) * (x - position.x) + (y - position.y) * (y - position.y)) < dontSetBoostR * dontSetBoostR)
					{
						hitbox[y][x] = info;
					}
				}
			}
		}
	}
}

void gameBoard::clearOnHitTmp()
{
	for (int x = 0; x < 1920; x++)
	{
		for (int y = 0; y < 1080; y++)
		{
			if (hitbox[y][x] >> 63)
				hitbox[y][x] = 0;
		}
	}
}

void gameBoard::getRandomBoost()
{
	int N = 26;
	int choice = rand() % N;

	boostsOnBoard.push_back(BoostOnBoard());
	switch (choice)
	{
	case 0:
		boostsOnBoard.back().setTexture(blind);
		boostsOnBoard.back().setBoost(choice, boostMode::all);
		break;
	case 1:
		boostsOnBoard.back().setTexture(colapseBounds);
		boostsOnBoard.back().setBoost(choice, boostMode::global);
		break;
	case 2:
		boostsOnBoard.back().setTexture(brokenWalls);
		boostsOnBoard.back().setBoost(choice, boostMode::all);
		break;
	case 3:
		boostsOnBoard.back().setTexture(brokenWallsGreen);
		boostsOnBoard.back().setBoost(choice, boostMode::one);
		break;
	case 4:
		boostsOnBoard.back().setTexture(clearAll);
		boostsOnBoard.back().setBoost(choice, boostMode::global);
		break;
	case 5:
		boostsOnBoard.back().setTexture(freeze);
		boostsOnBoard.back().setBoost(choice, boostMode::all);
		break;
	case 6:
		boostsOnBoard.back().setTexture(freezeRed);
		boostsOnBoard.back().setBoost(choice, boostMode::rest);
		break;
	case 7:
		boostsOnBoard.back().setTexture(growUp);
		boostsOnBoard.back().setBoost(choice, boostMode::all);
		break;
	case 8:
		boostsOnBoard.back().setTexture(growUpRed);
		boostsOnBoard.back().setBoost(choice, boostMode::rest);
		break;
	case 9:
		boostsOnBoard.back().setTexture(immortalGreen);
		boostsOnBoard.back().setBoost(choice, boostMode::one);
		break;
	case 10:
		boostsOnBoard.back().setTexture(longerGaps);
		boostsOnBoard.back().setBoost(choice, boostMode::all);
		break;
	case 11:
		boostsOnBoard.back().setTexture(longerGapsGreen);
		boostsOnBoard.back().setBoost(choice, boostMode::one);
		break;
	case 12:
		boostsOnBoard.back().setTexture(moreOftenHoles);
		boostsOnBoard.back().setBoost(choice, boostMode::all);
		break;
	case 13:
		boostsOnBoard.back().setTexture(moreOftenHolesGreen);
		boostsOnBoard.back().setBoost(choice, boostMode::one);
		break;
	case 14:
		boostsOnBoard.back().setTexture(lockLeftRed);
		boostsOnBoard.back().setBoost(choice, boostMode::rest);
		break;
	case 15:
		boostsOnBoard.back().setTexture(lockRightRed);
		boostsOnBoard.back().setBoost(choice, boostMode::rest);
		break;
	case 16:
		boostsOnBoard.back().setTexture(shrink);
		boostsOnBoard.back().setBoost(choice, boostMode::all);
		break;
	case 17:
		boostsOnBoard.back().setTexture(shrinkGreen);
		boostsOnBoard.back().setBoost(choice, boostMode::one);
		break;
	case 18:
		boostsOnBoard.back().setTexture(slowDown);
		boostsOnBoard.back().setBoost(choice, boostMode::all);
		break;
	case 19:
		boostsOnBoard.back().setTexture(slowDownGreen);
		boostsOnBoard.back().setBoost(choice, boostMode::one);
		break;
	case 20:
		boostsOnBoard.back().setTexture(slowDownRed);
		boostsOnBoard.back().setBoost(choice, boostMode::rest);
		break;
	case 21:
		boostsOnBoard.back().setTexture(speedUp);
		boostsOnBoard.back().setBoost(choice, boostMode::all);
		break;
	case 22:
		boostsOnBoard.back().setTexture(speedUpGreen);
		boostsOnBoard.back().setBoost(choice, boostMode::one);
		break;
	case 23:
		boostsOnBoard.back().setTexture(speedUpRed);
		boostsOnBoard.back().setBoost(choice, boostMode::rest);
		break;
	case 24:
		boostsOnBoard.back().setTexture(switchControlsRed);
		boostsOnBoard.back().setBoost(choice, boostMode::rest);
		break;
	case 25:
	default:
		boostsOnBoard.back().setTexture(switchHeads);
		boostsOnBoard.back().setBoost(choice, boostMode::global);
		break;
	}
	boostsOnBoard.back().setPosition(boostPosition.x * setting.xScale, boostPosition.y * setting.yScale);
	boostsOnBoard.back().setScale(setting.xScale, setting.yScale);
	boostsOnBoard.back().setOrigin(40 * setting.xScale, 40 * setting.yScale);
}
Boost* gameBoard::decode(int s)
{
	Boost* wsk = nullptr;
	switch (s)
	{
	case 0:
		wsk = new Blind;
		break;
	case 1:
		if (!boundRounds)
			boundTimer.restart();
		boundRounds++;
		break;
	case 2:
	case 3:
		wsk = new CrossBounds;
		break;
	case 4:
		EriseAll();
		break;
	case 5:
	case 6:
		wsk = new Freeze;
		break;
	case 7:
	case 8:
		wsk = new GrowUp;
		break;
	case 9:
		wsk = new Immortal;
		break;
	case 10:
	case 11:
		wsk = new LongerGaps;
		break;
	case 12:
	case 13:
		wsk = new MoreOftenHoles;
		break;
	case 14:
		wsk = new LockLeft;
		break;
	case 15:
		wsk = new LockRight;
		break;
	case 16:
	case 17:
		wsk = new Shrink;
		break;
	case 18:
	case 19:
	case 20:
		wsk = new SlowDown;
		break;
	case 21:
	case 22:
	case 23:
		wsk = new SpeedUp;
		break;
	case 24:
		wsk = new SwitchControls;
		break;
	case 25:
	default:
		changeHeads();
		break;
	}
	return wsk;
}
void gameBoard::markBoostPosOnHit(sf::Vector2i boostPosition)
{
	int begx = round(boostPosition.x - boostR);
	if (begx < xmin)
		begx = xmin;
	int begy = round(boostPosition.y - boostR);
	if (begy < ymin)
		begy = ymin;
	int endx = round(boostPosition.x + boostR);
	if (endx > xmax)
		endx = xmax;
	int endy = round(boostPosition.y + boostR);
	if (endy > ymax)
		endy = ymax;

	long long int info;
	info = long long int(1) << 62;

	for (int x = begx; x <= endx; x++)
	{
		for (int y = begy; y <= endy; y++)
		{
			if (hitbox[y][x])
				continue;
			else
			{
				if (((x - boostPosition.x) * (x - boostPosition.x) + (y - boostPosition.y) * (y - boostPosition.y)) < boostR * boostR)
				{
					hitbox[y][x] = info;
				}
			}
		}
	}
}
void gameBoard::clearBoostPosOnHit(sf::Vector2f boostPosition)
{
	int begx = round(boostPosition.x - boostR);
	if (begx < xmin)
		begx = xmin;
	int begy = round(boostPosition.y - boostR);
	if (begy < ymin)
		begy = ymin;
	int endx = round(boostPosition.x + boostR);
	if (endx > xmax)
		endx = xmax;
	int endy = round(boostPosition.y + boostR);
	if (endy > ymax)
		endy = ymax;

	long long int info;
	info = long long int(1) << 62;

	for (int x = begx; x <= endx; x++)
	{
		for (int y = begy; y <= endy; y++)
		{
			if ((hitbox[y][x] >> 62) & 1)
				hitbox[y][x] = 0;
		}
	}
}

void gameBoard::checkBoostsColission()
{
	std::vector<BoostOnBoard>::iterator it;
	bool found;
	for (auto player = Players.begin(); player != Players.end(); player++)
	{
		found = false;
		sf::Vector2f playertPos = player->getPos();
		for (it = boostsOnBoard.begin(); it != boostsOnBoard.end(); it++)
		{
			sf::Vector2f boostPos = it->getPosition();
			if (((double(playertPos.x) - boostPos.x) * (double(playertPos.x) - boostPos.x) + (double(playertPos.y) - boostPos.y) * (double(playertPos.y) - boostPos.y)) < (player->getRadious() + boostR) * (player->getRadious() + boostR))
			{
				found = true;
				boostMode mode = it->getMode();
				int BoostId = it->getBoost();
				int ran;
				switch (mode)
				{
				case boostMode::one:
					player->addBoost(decode(BoostId));
					break;
				case boostMode::all:
					for (auto pl = Players.begin(); pl != Players.end(); pl++)
						pl->addBoost(decode(BoostId));
					break;
				case boostMode::rest:
					for (auto pl = Players.begin(); pl != Players.end(); pl++)
						if (pl != player)
							pl->addBoost(decode(BoostId));
					break;
				case boostMode::random:
					ran = rand() & Players.size();
					(Players.begin() + ran)->addBoost(decode(BoostId));
					break;
				case boostMode::global:
					decode(BoostId);
					break;
				default:
					break;
				}
				clearBoostPosOnHit(it->getPosition());
				break;
			}
		}
		if (found)
			boostsOnBoard.erase(it);
	}
}

void gameBoard::clearBoosts()
{
	boostsOnBoard.clear();
}

void gameBoard::restart()
{
	generateMap();
	boostTime = getTimeBoost();
	clearHitbox();
	clearBoosts();
	for (auto& player : Players)
	{
		player.setParams(generteAngle(), 6, 2.5, 100);
		player.setPosition(generatePositions());
		player.reset();
	}
	/*Players.back().setParams(generteAngle(), 6, 2.5, 100);
	Players.back().setPosition(generatePositions());
	Players.back().reset();
	*/
}

void gameBoard::boundFunction()
{
	if (setting.TimeStop)
	{
		if (!timeFlag)
		{
			timeFlag = true;
			duration += boundTimer.getElapsedTime().asSeconds();
		}
		boundTimer.restart();
	}
	else
	{
		timeFlag = false;
		float off = boundRange * sin((boundTimer.getElapsedTime().asSeconds() + duration) * boundVel);
		if (off < 0)
		{
			boundRounds--;
			boundTimer.restart();
			duration = 0;
			setBounds(defyMax, defyMin, defxMax, defxMin, 5);
		}
		else
		{
			setBounds(defyMax - off, defyMin + off, defxMax - off, defxMin + off, 5);
		}
	}
}

void gameBoard::changeHeads()
{
	int i = 0;
	double angl = Players[0].getAngle();
	sf::Vector2f posTmp = Players[0].getPos();
	for (i = 0; i < Players.size() - 1; i++)
	{
		Players[i].changeHead(Players[i + 1].getAngle(), Players[i + 1].getPos());
	}
	Players[i].changeHead(angl, posTmp);
}


