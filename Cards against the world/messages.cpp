#include "messages.h"
namespace message {
	const code unKnownError = 0;
	const code lobbyIsAlredyChosen = 1;
	const code noPortsAreAvailable = 2;
	const code lobbyNameIsIncorrect = 3;
	const code couldNotFindLobby = 4;
	const code ServerIsFull = 5;
	const code lobbyNameIsLocked = 6;//server end
	const code unableToRechServer = 7;
	const code unableToSendData = 8;
	const code unableToGetData = 9;
	const code connected = 10;
	namespace {//private
		const std::vector<std::string> connectionError{
			"Unknown error",
			"Lobby is already chosen" ,
			"No ports are available on server",
			"Lobby name is incorrect",
			"Cund not find lobby",
			"Server is full",
			"Lobby name is locked",
			"Unable to reach server",
			"Unable to send data",
			"Unable to get Data" };//7
	}
	std::string getConnectError(code error) {
		if (error < 1 || error >= connectionError.size())
			return connectionError[0];
		return connectionError[error];
	}
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
	const char raw = 0;//raw  data
	const char unicode = 1;//unicode string 
	const char error = 2;//error
	const char newPlayer = 3;// for others get id newplayer
	const char getId = 4;// get new player id;
	const char getLobbyInfo = 5;// get lobby nicks itp
	const char disconnect = 6;// disconnect player of this id 
	const char Ready = 7;// - Ready wcisniecie ready i wyslanie do servera i wszyatkich  zmiana na zielony
	const char notReady = 8;// - notReady to samo pu tylko not ready zmiana na czerowny
	const char start = 9;// - start leader jeœli wszyscy ready to zmiana stanu na grê LEADER
	const char dequeUpdate = 10;// - update talii //aktualnie jeszcze nie LEADER
	const char timeUpdate = 11;// - update czas gry LEADER

}//TODO naprawnie deadlocka broadcast disconnect