#pragma once
#include <filesystem>
class Deck
{
public:
	struct data {
		std::string text;
		bool doubl;
	};
private:
	enum card { none = 0, black, white };
	std::experimental::filesystem::path pathToDeck;
	int whiteCards;
	int BlackCards;
	std::vector<std::string> white_;
	std::vector<data> black_;
	card checkIfNewCard(std::string& line);
	int getNumber(std::string & len);
public:
	//TODO get files from server;
	Deck();
	bool load(std::string name);
	std::string getCard(std::size_t pos, bool black = true);
	bool getDouble(std::size_t pos);//only black cards !!!!
	~Deck();
};

extern Deck deck;
