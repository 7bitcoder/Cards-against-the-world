#pragma once
#include "lobbyThread.h"
#include "MainThreadListener.h"
std::mutex mut;

std::map<std::string, player> mapaLobby;

#define DEFAULT_BUFLEN 512
void lobbyThread(SOCKET socket_, std::string lobbyId)//, std::mutex, std::map<std::string, int>)
{

	MainThreadListener listener(socket_, lobbyId);
	//init listener and connect to leader
	if (!listener.init() || listener.validateLeader()){
		//init error
		return;
	}

}