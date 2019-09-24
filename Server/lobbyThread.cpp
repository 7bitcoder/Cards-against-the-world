#pragma once
#include "lobbyThread.h"
#include "MainThreadListener.h"
#include "chat.h"
std::mutex mut;

std::map<std::string, player> mapaLobby;

#define DEFAULT_BUFLEN 512
void ChatThread(SOCKET socket_, std::string lobbyId_)
{
	//main listener tylko init
	MainThreadListener listener(socket_, lobbyId_);
	int port = listener.createListenSocket();
	if (!port) { ; }//todo
	std::string portStr = std::to_string(port);
	mut.lock();
	mapaLobby[lobbyId_].chatPort = port;
	mut.unlock();
	chat Chat(listener.ListenSocket, lobbyId_, portStr);
	Chat.run();
} 

void lobbyThread(SOCKET socket_, std::string lobbyId)//, std::mutex, std::map<std::string, int>)
{

	MainThreadListener listener(socket_, lobbyId);
	//init listener and connect to leader
	int port = listener.createListenSocket();
	if (!port) { ; }//TODO
	mut.lock();
	mapaLobby[lobbyId].lobbyPort = port;
	mut.unlock();
	if (!listener.acceptLeaderConnection()) { ; }//TODO
	std::thread chat(ChatThread, listener.ClientSocket, lobbyId);//TODO implement games states and lock lobby

	while (true) { ; }
	return;
}


