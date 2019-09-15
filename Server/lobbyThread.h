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

extern std::mutex mut;
extern std::map<std::string, int> mapaLobby;
void lobbyThread(SOCKET socket, std::string lobbyId);

