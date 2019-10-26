#pragma once
#include <map>
#include "serverUtils.h"
#include <algorithm>
#include <vector>
#include <deque>
#include <set>

#define LEN 8000
class Cards{//front is top back is bottom of cards deque
	private:
	std::deque<int> cards;
	public:
		Cards() { ; }
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
	int id = cards.front(); 
	cards.pop_front();
	return id;
	}
	int size() { return cards.size(); }
	bool empty() { return cards.empty();}// do sprawdzenia czy talia jest pusta czarna przy rozpoczecie rundy
	void putCardBack(int car){
		cards.push_back(car);
	}
};
class whiteCards:public Cards{
	private:
	std::set<int> cardsTaken;//check if putCards back are OK
	public:
	int getCard(){
	int  id = Cards::getCard();
	cardsTaken.insert(id);
	return id;
	}
	bool putCardBack(int car){
		auto it = cardsTaken.find(car);
		if(it == cardsTaken.end())
		   return false;
		cardsTaken.erase(it);
	Cards::putCardBack(car);
	return true;		
	}
};
struct slot
{
	std::u32string nick;
	bool ready;
	int id;
};
enum states { waiting = 1, starting, questionInit, question, questionOvertime, choseInit, chose, choseOvertime, summing, kill };
class game: public serverUtils
{
private:
	Cards black;
	whiteCards white;
	bool lock;//lock lobby for new players
	char buff[LEN];
	std::map<SOCKET, slot>::iterator choser;
	char free[9];//free table to check if slot of id is free;
	std::map<SOCKET, slot> clients;//0 listen rest players up to 8
	std::vector<int> playersSended;//players that sended cards
	SOCKET leader;
	SOCKET listenSocket;
	fd_set fds;
	std::u32string lobbyId;
public:
	game(SOCKET listen, SOCKET leader, std::u32string nick, std::u32string lobbyId);
	bool broadCast(SOCKET socket, char* buff, int len, bool all = false);
	bool computeNewClientData(SOCKET socket, std::u32string& nick);
	bool rejectNewClient();
	bool acceptNewClient();
	void disconnect(SOCKET sock);
	bool waitForLeaderAccept() { char coding, id; if (receiveLen(leader, buff, coding, id, 10, 0) == -1) return false; return true; }
	states waitingF();
	states startingF();
	states questionInitF();
	states questionF();
	states questionOvertimeF();
	states choseinitF();
	states choseF();
	states choseOvertimeF();
	states summingF();
	~game();
};

