#pragma once
#include"socketUtils.h"
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

class serverUtils : public socketUtils {
private:
public:
	serverUtils();
	~serverUtils();
	bool sendLen(SOCKET socket, const char* data, int length);//len must be sended;
	int receiveTime(SOCKET socket, char* data, int lenght, int time);//len must be received;
	int receiveLen(SOCKET socket, char* data, char& coding, char& playerId);//receive + decode len + reciveRaw
	//up functions are mainly for chat
	//TODO send command and receive command
};
