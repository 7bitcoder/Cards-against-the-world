#include "settings.h"


Settings::Settings()
{
	if (!std::filesystem::exists("../settings.bin"))
	{
		SoundVolume = MusicVolume = 1.0f;
		std::ofstream ofile("../settings.bin", std::ios::binary);
		ofile.seekp(0);
		ofile.write((char*)& SoundVolume, sizeof(double));
		ofile.write((char*)& MusicVolume, sizeof(double));
		posDefaultMusic = 1;
		ofile.write((char*)& posDefaultMusic, sizeof(int));
		ofile.close();
	}
	else
	{
		std::ifstream infile("../settings.bin", std::ios::binary);
		infile.seekg(0);
		infile.read((char*)& SoundVolume, sizeof(double));
		infile.read((char*)& MusicVolume, sizeof(double));
		infile.read((char*)& posDefaultMusic, sizeof(int));
		infile.close();
	}

}

void Settings::save()
{
	std::ofstream ofile("../settings.bin", std::ios::binary);
	ofile.clear();
	ofile.seekp(0);
	ofile.write((char*)& SoundVolume, sizeof(double));
	ofile.write((char*)& MusicVolume, sizeof(double));
	ofile.write((char*)& posDefaultMusic, sizeof(int));
	ofile.close();
}

Settings::~Settings()
{
}
