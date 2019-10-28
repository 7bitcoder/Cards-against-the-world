#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#else
#endif // !WIN32_LEAN_AND_MEAN
#include <vector>
#include <vector>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <uchar.h>

#define LEN 4000
#pragma comment (lib, "Ws2_32.lib")
class socketUtils {
protected:
	std::u32string passCode = U"a7dzRwQjnw5kW6uEnhx7";
	void clear(char* buff) { buff[3] = buff[2] = buff[1] = buff[0] = 0; };

public:
	socketUtils();
	~socketUtils();
	void addMessagePrefix(char* pos, uint16_t nmb, char coding, char playerId);// coding 0 - normal, 1 unidoce, rest custom, playerId 0 server 1 braodcast rest player id
	uint16_t getMessagePrefix(char* pos, char& coding, char& playerId);
	std::u32string decode(char* begData, int limit = 0, int reserve = 30);//decode from rcvbuff
	int code(const std::u32string& string, char* pos); //return bytes writed //code in buff
	void codeCard(char* pos, uint16_t card);
	void codeCards(char* pos, std::vector<uint16_t> & cards);
	uint16_t decodeCard(char* pos);
	std::vector<int> decodeCards(char* pos, int len);
};

