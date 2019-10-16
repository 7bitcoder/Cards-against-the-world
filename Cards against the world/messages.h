#pragma once
#include <iostream>
#include <vector>
namespace message {
	/*
	protok�l komunikacji
	1 bajt  CODE
	0 - ZWYK�E dane czyli raw data z sizem podanym w Size
	1 - UNICODE message size w bajtach podany w Size
	2 - ERROR kod erroru podany w playerId
	3 - KOMENDA z kodem podanym  w playerId oraz ew danymi w Size
	2 bajtach SIZE
	short d�ugo�� wiadomo�ci zawsze podana w bajtach lub dodatkowe dane
	1 bajt PLAYERID
	id gracza wysy�aj�cego je�li server to 0 , 1 to broadcast reszta to id gracza lub dodatkowe dane
	4+ bajty wiadomo��
	KOMENDY :
	1 - do nowego gracza wyslanie nick�w i id graczy w lobby
	2 - do graczy wyslanie info o nowym graczu w lobby
	3 - nadane id gracza
	*/
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
