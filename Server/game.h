#pragma once
#include <map>
#include "serverUtils.h"
#define LEN 8000
enum states{ waiting = 1, starting, questionInit, question, choseInit, chose, summing};
struct slot
{
	std::u32string nick;
	int id;
};
class game: public serverUtils
{
private:
	bool lock;//lock lobby for new players
	char buff[LEN];
	char free[9];//free table to check if slot of id is free;
	std::map<SOCKET, slot> clients;//0 listen rest players up to 8
	SOCKET leader;
	SOCKET listenSocket;
	fd_set fds;
	std::u32string lobbyId;
public:
	game(SOCKET listen, SOCKET leader, std::u32string nick, std::u32string lobbyId);
	bool broadCast(SOCKET socket, char* buff, int len);
	bool computeNewClientData(SOCKET socket, std::u32string& nick);
	bool acceptNewClient();
	void disconnect(SOCKET sock);
	bool waitForLeaderAccept() { char coding, id; if (receiveLen(leader, buff, coding, id, 10, 0) == -1) return false; return true; }
	states waiting();
	~game();
};

