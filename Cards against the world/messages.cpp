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