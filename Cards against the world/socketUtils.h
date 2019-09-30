#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#else
#endif // !WIN32_LEAN_AND_MEAN

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
	void terminate(char* buff) { buff[3] = buff[2] = buff[1] = buff[0] = 0; };
	
public:
	socketUtils();
	~socketUtils();
	void addMessageLen(char* pos, uint16_t nmb) { mbstate_t state{}; c16rtomb(pos, htons(nmb), &state); }
	uint16_t getMessageLen(char* pos) { char16_t ch; mbstate_t state{}; mbrtoc16(&ch, pos, 2, &state);  return ntohs(ch); }
	bool sendU(SOCKET socket, const char* data, int length);
	std::u32string decode(char* begData, int limit = 0, int reserve = 30);//decode from rcvbuff
	int code(const std::u32string& string, char* pos); //return bytes writed //code in buff

};
