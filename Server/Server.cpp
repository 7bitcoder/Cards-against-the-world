// Server.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#define TIME_WAIT 60
#include "tcpListener.h"
#include "lobbyThread.h"
#include <thread>

extern std::mutex mut;
extern std::map<std::string, player> mapaLobby;

namespace error {
	const char lobbyIsAlredyChosen[] = "a";
	const char noPortsAreAvailable[] = "b";
	const char lobbyNameIsIncorrect[] = "c";
	const char couldNotFindLobby[] = "d";
	const char ServerIsFull[] = "e";
}

int main(void) {
	const int maxLobby = 30;
	int numberOfLobbys = 0;
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}

	tcpListener mainListener(3000);
	if (!mainListener.init())
		return 0;
	while (true) {
		if (!mainListener.run())
			continue;
		if (!mainListener.checkData()) //get data and check serure code
			continue;
		if (mainListener.isNewLobby()) {
			std::string lobbyStr = mainListener.getLobby();
			mut.lock();
			if (mapaLobby.find(lobbyStr) != mapaLobby.end() && mapaLobby[lobbyStr].chatPort && mapaLobby[lobbyStr].lobbyPort) {
				mut.unlock();
				printf("lobby is alredy chosen\n");
				mainListener.sendAll(error::lobbyIsAlredyChosen, strlen(error::lobbyIsAlredyChosen));
				Sleep(1);
				mainListener.closeConnection();
				continue;
			}
			mut.unlock();
			if (lobbyStr.empty()) {
				printf("lobby name is incorrect\n");
				mainListener.sendAll(error::lobbyNameIsIncorrect, strlen(error::lobbyNameIsIncorrect));
				Sleep(1);
				mainListener.closeConnection();
				continue;
			}
			mut.lock();
			if (mapaLobby.size() < maxLobby) {
				mut.unlock();
				std::thread lobby(lobbyThread, mainListener.getNewClientSocket(), lobbyStr);
				lobby.detach();
				continue;
			}
			mut.unlock();
			printf("server is full\n");
			mainListener.sendAll(error::ServerIsFull, strlen(error::ServerIsFull));
			Sleep(1);
			mainListener.closeConnection();

		}
		else {
			std::string lobby = mainListener.getLobby();
			if (lobby.empty()) {
				printf("lobby name is incorrect\n");
				mainListener.sendAll(error::lobbyNameIsIncorrect, strlen(error::lobbyNameIsIncorrect));
				Sleep(1);
				mainListener.closeConnection();
				continue;
			}
			mut.lock();
			if (mapaLobby.find(lobby) == mapaLobby.end() || !mapaLobby[lobby].chatPort || mapaLobby[lobby].lobbyPort) {
				mut.unlock();
				printf("could not find lobby\n");
				mainListener.sendAll(error::couldNotFindLobby, strlen(error::couldNotFindLobby));
				Sleep(1);
				mainListener.closeConnection();
				continue;
			}
			int newPort = mapaLobby[mainListener.getLobby()].lobbyPort;
			int chatPort = mapaLobby[mainListener.getLobby()].chatPort;
			mut.unlock();
			std::string portMsg = std::to_string(newPort);
			mainListener.sendAll(portMsg.c_str(), portMsg.size());
			Sleep(1);
			std::string portMsg = std::to_string(chatPort);
			mainListener.sendAll(portMsg.c_str(), portMsg.size());
			Sleep(1);
			mainListener.closeConnection();
		}
	}
	WSACleanup();

	return 0;
	/*
	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);


	fd_set master;
	FD_ZERO(&master);
	FD_SET(ListenSocket, &master);

	TIMEVAL time;
	time.tv_sec = 1;
	time.tv_usec = 0;
	printf("setup in while loop ->\n");
	while (true)
	{
		fd_set copy = master;
		printf("socket wait\n");
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		printf("after select /interval/interrupted\n");
		for (int i = 0; i < socketCount; i++)
		{
			printf("in for %d\n", i);
			SOCKET sock = copy.fd_array[i];
			if (sock == ListenSocket)
			{
				//accept new connection
				SOCKET client = accept(ListenSocket, nullptr, nullptr);
				FD_SET(client, &master);

				std::string welcome = "welcome to chat server xD!";
				printf("new user\n");
				send(client, welcome.c_str(), welcome.size() + 1, 0);

			}
			else //get message
			{
				ZeroMemory(recvbuf, recvbuflen);

				int bytesIn = recv(sock, recvbuf, recvbuflen, 0);
				printf("Message: %s\n", recvbuf);
				if (bytesIn <= 0)//message is wrong
				{
					printf("message error close socket\n");
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else//if message is correct
				{
					for (int i = 0; i < master.fd_count; i++)
					{
						SOCKET outSock = master.fd_array[i];
						if (outSock != ListenSocket && outSock != sock)
						{
							std::string ss = "Socket :";
							ss += " :";
							ss += sock;
							ss += " :";
							ss += recvbuf;
							ss += "\n";
							send(outSock, ss.c_str(), ss.size() + 1, 0);
						}
					}
				}
			}
		}
	}



	*/
}
// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
