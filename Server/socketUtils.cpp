#include "socketUtils.h"



socketUtils::socketUtils()
{
}


socketUtils::~socketUtils()
{
}
bool socketUtils::sendU(SOCKET socket, const char* data, int length)
{
	int count = 0;
	while (count < length) {
		int n = send(socket, data + count, length, 0);
		if (n == -1) {
			return false;
		}
		count += n;
		length -= n;
	}
	return true;
}
std::u32string socketUtils::decode(char* pos, int limit, int reserve) {
	std::u32string str;
	str.reserve(reserve);
	mbstate_t p{};
	bool tillEnd = !limit;
	char32_t x;
	int length;
	for (int i = 0; i < limit || tillEnd; i++) {
		length = mbrtoc32(&x, pos + i * 4, 4, &p);
		if (!length)
			break;
		str.push_back(x);
	}
	return str;
}

int socketUtils::code(const std::u32string& string, char* pos) {
	mbstate_t p{};
	int size = string.size();
	if (size + 2 > LEN || !size)
		return 0;//error
	size_t length;
	int i;
	for (i = 0; i < size; i++) {
		clear(pos + i * 4);
		length = c32rtomb(pos + i * 4, string[i], &p);
	}
	clear(pos + i * 4);
	return (++i) * 4;
}




