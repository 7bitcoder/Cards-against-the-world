#pragma once
#include <map>
#include "serverUtils.h"
#define LEN 8000
enum states{ waiting = 1, starting, questionInit, question, choseInit, chose, summing, kill};
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
struct slot
{
	std::u32string nick;
	bool ready;
	int id;
};
class game: public serverUtils
{
private:
	Cards black;
	whiteCards white;
	bool lock;//lock lobby for new players
	char buff[LEN];
	choserId;
	char free[9];//free table to check if slot of id is free;
	std::map<SOCKET, slot> clients;//0 listen rest players up to 8
	SOCKET leader;
	SOCKET listenSocket;
	fd_set fds;
	std::u32string lobbyId;
public:
	game(SOCKET listen, SOCKET leader, std::u32string nick, std::u32string lobbyId);
	bool broadCast(SOCKET socket, char* buff, int len, bool all = false);
	bool computeNewClientData(SOCKET socket, std::u32string& nick);
	bool acceptNewClient();
	void disconnect(SOCKET sock);
	bool waitForLeaderAccept() { char coding, id; if (receiveLen(leader, buff, coding, id, 10, 0) == -1) return false; return true; }
	getNewChoser()
	states waiting();
	~game();
};

