#pragma once
#include <iostream>
#include <vector>
namespace message {
	/*
	protokól komunikacji
	1 bajt  CODE
	0 - ZWYK£E dane czyli raw data z sizem podanym w Size
	1 - UNICODE message size w bajtach podany w Size
	2 - ERROR kod erroru podany w playerId
	3 - KOMENDA z kodem podanym  w playerId oraz ew danymi w Size
	2 bajtach SIZE
	short d³ugoœæ wiadomoœci zawsze podana w bajtach lub dodatkowe dane
	1 bajt PLAYERID
	id gracza wysy³aj¹cego jeœli server to 0 , 1 to broadcast reszta to id gracza lub dodatkowe dane
	4+ bajty wiadomoœæ
	KOMENDY :
	1 - do nowego gracza wyslanie nicków i id graczy w lobby
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
namespace codes {
	/*protokól komunikacji
	1 bajt  CODE
	0 - ZWYK£E dane czyli raw data z sizem podanym w Size
	1 - UNICODE message size w bajtach podany w Size
	2 - ERROR kod erroru podany w playerId
	KOmendy :
	z kodem podanym  w playerId oraz ew danymi w Size
	2 bajtach SIZE
	short d³ugoœæ wiadomoœci zawsze podana w bajtach lub dodatkowe dane
	1 bajt PLAYERID
	 id gracza wysy³aj¹cego jeœli server to 0 , 1 to broadcast reszta to id gracza lub dodatkowe dane
	4+ bajty wiadomoœæ
	*/
	extern const char raw;//raw  data
	extern const char unicode;//unicode string 
	extern const char error;//error
	extern const char newPlayer;// for others get id newplayer
	extern const char getId;// get new player id;
	extern const char getLobbyInfo;// get lobby nicks itp
	extern const char disconnect;// disconnect player of this id 
	extern const char Ready;// - Ready wcisniecie ready i wyslanie do servera i wszyatkich  zmiana na zielony
	extern const char notReady;// - notReady to samo pu tylko not ready zmiana na czerowny
	extern const char start;// - start leader jeœli wszyscy ready to zmiana stanu na grê LEADER
	extern const char dequeUpdate;// - update talii //aktualnie jeszcze nie LEADER
	extern const char timeUpdate;// - update czas gry LEADER

}//TODO naprawnie deadlocka broadcast disconnect