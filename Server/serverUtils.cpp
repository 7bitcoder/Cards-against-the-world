#include "serverUtils.h"



serverUtils::serverUtils()
{
}


serverUtils::~serverUtils()
{
}

bool serverUtils::sendLen(SOCKET socket, const char* data, int length)
{
	int count = 0;
	while (count < length) {
		int n = send(socket, data + count, length, 0);
		if (n == SOCKET_ERROR) {
			return false;
		}
		count += n;
		length -= n;
	}
	return true;
}
int serverUtils::receiveTime(SOCKET socket, char* data, int length, int time)
{
	TIMEVAL tv = { time, 0 };

	// Set up the file descriptor set.
	int count = 0;
	while (count < length) {
		fd_set tmp;
		FD_ZERO(&tmp);
		FD_SET(socket, &tmp);
		int iResult = select(0, &tmp, NULL, NULL, &tv);
		if (iResult == SOCKET_ERROR) {
			printf("select failed with error: %d\n", WSAGetLastError());
			closesocket(socket);
			return false;
		}
		else if (iResult == 0) {
			printf("response time for code is is up\n");
			closesocket(socket);
			return false;
		}

		int n = recv(socket, data + count, length, 0);
		if (n == SOCKET_ERROR) {
			return false;
		}
		count += n;
		length -= n;
	}
	return true;
}
int serverUtils::receiveLen(SOCKET socket, char* data, char& coding, char& playerId)
{
	if (!receiveTime(socket, data, 4, 1))
		return -1;
	int len = getMessagePrefix(data, coding, playerId);
	if (!receiveTime(socket, data + 4, len, 1))
		return -1;
	return len + 4;
}
