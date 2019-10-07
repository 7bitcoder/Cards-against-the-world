#include "socketUtils.h"

socketUtils::socketUtils()
{
}


socketUtils::~socketUtils()
{
}
void socketUtils::addMessagePrefix(char* pos, uint16_t nmb, char coding, char playerId) {
	mbstate_t state{};
	pos[0] = coding;
	nmb = htons(nmb);
	memcpy(pos + 1, (char*)& nmb, 2);
	pos[3] = playerId;
}
uint16_t socketUtils::getMessagePrefix(char* pos, char& coding, char& playerId) {
	coding = pos[0];
	playerId = pos[3];
	char16_t ch; mbstate_t state{};
	uint16_t* ptr = (uint16_t*)(pos + 1);
	return ntohs(*ptr);
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
		if (!length && tillEnd)
			break;
		str.push_back(x);
	}
	return str;
}

int socketUtils::code(const std::u32string& string, char* pos) {
	mbstate_t p{};
	int size = string.size();
	if (size + 4 > LEN || !size)
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
