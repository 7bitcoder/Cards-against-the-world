#pragma once
#include <filesystem>
class Deck
{
private:
	enum card { none = 0, black, white };
	std::experimental::filesystem::path pathToDeck;
	int whiteCards;
	int BlackCards;
	std::vector<std::string> white_;
	std::vector<std::string> black_;
	card checkIfNewCard(std::string& line);
	int getNumber(std::string & len);
public:
	//TODO get files from server;
	Deck();
	std::string getCard(std::size_t pos, bool black = true);
	bool load(std::string name);
	~Deck();
};

extern Deck deck;
