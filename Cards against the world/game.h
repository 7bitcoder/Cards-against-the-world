#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include<algorithms>
#include "socketUtils.h"
#include"messages.h"
#include "chat.h"
#define LEN 4000
struct data {
int id
}
class Cards{//front is top back is bottom of cards deque
	private:
	std::deque<data> cards;
	public:
	deques();{}
	void init(int wh )
	{
	cards.clear();
	for(int i = 0; i<wh; i++)
		cards.push_back(i);
	}
	void shuffle() {
	std::random_shuffle(cards.begin(), cards.end());
	}
	int getCard() { 
	int id = cards.front().id; 
	cards.pop_front();
	return id;
	}
	bool empty() { return cards.empty();}// do sprawdzenia czy talia jest pusta czarna przy rozpoczecie rundy
	void putCardsBack(std::vector<int> & car){
	for(int x : car)
		cards.push_back(x);
	}
};
class whiteCards:public Cards{
	private:
	std::set<int> cardsTaken;//check if putCards back are OK
	public:
	int getCard(){
	int  id = Cads::getCard();
	cardsTaken.insert(id);
	return id;
	}
	bool putCardsBack(std::vector<int> & car){
	for(int x : car){
		auto it = cardsTaken.find(x);
		if(it == cardsTaken.end()
		   return false;
		cardsTaken.erase(it);
	}
	Cards::putCardsBack(car);
	return true;		
	}
};
class game :public socketUtils
{
private:
	Cards black;
	whiteCards white;
	enum state{lobby, GameBegin, exit};
	state state_;
	chat Chat;
	std::map<int, sf::String > players;//0 listen rest players up to 8
	unsigned short portToConnect = 3000;
	sf::IpAddress address = "3.229.14.134";
	std::u32string code = U"a7dzRwQjnw5kW6uEnhx7";
	sf::RenderWindow& window;
	sf::String lobbyId;
	sf::String nickname;
	sf::TcpSocket entranceSocket;
	sf::TcpSocket lobbySocket;
	sf::TcpSocket chatSocket;
	char buff[LEN];
	bool newLobby;
	char playerId;
	bool ready_ = false;
	bool lockLobby  = false;
	//
	sf::Texture backgroundTexture;
	sf::Sprite background;
	sf::Font font;
	sf::SoundBuffer clickBuff;
	sf::SoundBuffer switchBuff;
	//sf::Sprite background;
	sf::Texture block;
	sf::Texture blockPressed;
	sf::Texture offButton;
	sf::Texture checkOff;

	//
	bool Send(std::u32string s, sf::TcpSocket& socket);
	int getCommand(sf::TcpSocket& socket, char& coding, char& playerId);
	std::u32string getString(sf::TcpSocket& socket, int length);
	bool receive(sf::TcpSocket& socket, std::u32string& data, char& coding, char& playerId);
	game::state joinWait();
	game::state LeaderWait();
	void checkCommands();
public:
	game(sf::RenderWindow& win, sf::SoundBuffer& sndbuff, sf::Font font, sf::String lobbyId, sf::String nick, bool newlobby);
	message::code connect();
	void run();
	~game();
};

