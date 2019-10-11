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
#include<mutex>
#include<map>
struct player
{
	int lobbyPort = 0;
	int chatPort = 0;
	bool lock = false;
};
extern std::mutex mut;
extern std::map<std::u32string, player> mapaLobby;
void lobbyThread(SOCKET socket, std::u32string lobbyId, std::u32string nick);

