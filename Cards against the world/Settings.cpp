#include "settings.h"
#include <filesystem>

Settings::Settings()
{
	try {
		std::cout << std::experimental::filesystem::current_path().string();
		if (!std::experimental::filesystem::exists("../settings.txt"))
		{
			SoundVolume = MusicVolume = 1.0f;
			std::ofstream ofile("../settings.txt", std::ios::binary);
			ofile << "Game settings:\n";
			ofile << "Sound volume: " << SoundVolume << std::endl;
			ofile << "Music volume: " << MusicVolume << std::endl;
			posDefaultMusic = 1;
			ofile << "Default music track: " << posDefaultMusic << std::endl;
			ofile.close();
		}
		else
		{
			const char* error = "could not read settings file\n";
			char data[256];
			std::string strData;
			std::ifstream infile("../settings.txt", std::ios::binary);
			infile.getline(data, 256);//Game settings
			if (!infile.good())
				throw std::exception(error);
			infile.getline(data, 256);//Sound volume
			if (!infile.good())
				throw std::exception(error);
			strData = data;
			auto it = strData.find_first_of(':');
			auto subStr = strData.substr(it + 2);
			SoundVolume = std::stod(subStr);
			if (SoundVolume < 0)
				SoundVolume = 0;
			else if (SoundVolume > 1)
				SoundVolume = 1;

			infile.getline(data, 256);//Music volume
			if (!infile.good())
				throw std::exception(error);
			strData = data;
			it = strData.find_first_of(':');
			subStr = strData.substr(it + 2);
			MusicVolume = std::stod(subStr);
			if (MusicVolume < 0)
				MusicVolume = 0;
			else if (MusicVolume > 1)
				MusicVolume = 1;

			infile.getline(data, 256);//Sound volume
			if (!infile.good())
				throw std::exception(error);
			strData = data;
			it = strData.find_first_of(':');
			subStr = strData.substr(it + 2);
			posDefaultMusic = std::stoi(subStr);
			if (posDefaultMusic < 0)
				posDefaultMusic = 0;
			else if (posDefaultMusic > 1)
				posDefaultMusic = 1;
			infile.close();
		}
	}
	catch(std::exception & ex) {
		std::cerr << ex.what();
		exit(-1);
	}
	catch (...)
	{
		std::cerr << "unknown error appers";
		exit(-1);
	}

}

void Settings::save()
{
	std::ofstream ofile("../settings.txt", std::ios::binary);
	ofile << "Game settings:\n";
	ofile << "Sound volume: " << SoundVolume << std::endl;
	ofile << "Music volume: " << MusicVolume << std::endl;
	ofile << "Default music track: " << posDefaultMusic << std::endl;
	ofile.close();
}

Settings::~Settings()
{
}
