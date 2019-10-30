#include "Music.h"



void Music::load()
{
	if (!menuMusic.openFromFile("Music/lofi.wav"))
		throw std::exception("music file missing");
	menuMusic.setLoop(true);
	menuMusic.setVolume(30 * setting.MusicVolume);
	//menuMusic.play();
	if (!gameMusic.openFromFile("Music/lofi.wav"))
		throw std::exception("music file missing");
	gameMusic.setLoop(true);
	gameMusic.setVolume(30 * setting.MusicVolume);
}
bool Music::setGameMusic(std::string dir)
{
	return gameMusic.openFromFile("../Music/" + dir);
}

Music::~Music()
{
}
