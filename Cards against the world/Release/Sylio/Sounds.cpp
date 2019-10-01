#include "Sounds.h"

Sounds::Sounds(std::string click_, std::string switch_)
{
	if (!switchBuffer.loadFromFile("../Sounds/" + switch_))
		throw std::exception("sound file missing sound.h");
	switchSound.setBuffer(switchBuffer);
	switchSound.setVolume(30 * setting.SoundVolume);

	if (!clickBuffer.loadFromFile("../Sounds/" + click_))
		throw std::exception("sound file missing sound.h");
	clickSound.setBuffer(clickBuffer);
	clickSound.setVolume(100 * setting.SoundVolume);
}

void Sounds::setVolume(float vol)
{
	switchSound.setVolume(30 * vol);
	clickSound.setVolume(100 * vol);
}

Sounds::~Sounds()
{

}
