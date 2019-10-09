#pragma once
#include <iostream>
#include <vector>
namespace message {
	typedef char code;
	extern const code unKnownError;
	extern const code lobbyIsAlredyChosen;
	extern const code noPortsAreAvailable;
	extern const code lobbyNameIsIncorrect;
	extern const code couldNotFindLobby;
	extern const code ServerIsFull;
	extern const code lobbyNameIsLocked;//server end
	extern const code unableToRechServer;
	extern const code unableToSendData;
	extern const code unableToGetData;
	extern const code connected;
	namespace {//private
		extern const std::vector<std::string> connectionError;//7
	}
	std::string getConnectError(code error);
}