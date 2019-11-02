#include "socketUtils.h"

socketUtils::socketUtils()
{
}


socketUtils::~socketUtils()
{
}
void socketUtils::addMessagePrefix(char* pos, uint16_t nmb, char coding, char playerId) {
	pos[0] = coding;
	nmb = htons(nmb);
	memcpy(pos + 1, (char*)& nmb, 2);
	pos[3] = playerId;
}
uint16_t socketUtils::getMessagePrefix(char* pos, char& coding, char& playerId) {
	coding = pos[0];
	playerId = pos[3];
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

int socketUtils::code(const std::u32string & string, char* pos) {
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

void socketUtils::codeCard(char* pos, uint16_t card)
{
	card = htons(card);
	memcpy(pos, (char*)& card, 2);
}

void socketUtils::codeCards(char* pos, std::vector<uint16_t> & cards)
{
	for (int i = 0; i < cards.size(); i++)
		codeCard(pos + i * 2, cards.at(i));
}

uint16_t socketUtils::decodeCard(char* pos)
{
	uint16_t* ptr = (uint16_t*)(pos);
	return ntohs(*ptr);
}

std::vector<int> socketUtils::decodeCards(char* pos, int len)
{
	std::vector<int> cards;
	for (int i = 0; i < len; i++) {
		cards.push_back(decodeCard(pos + i * 2));
	}
	return cards;
}

void socketUtils::codeAllCards(char * pos, std::vector<std::array<int, 2>>& cardPlayer)
{
	for (int i = 0; i < cardPlayer.size(); i++) {
		codeCard(pos + i * 3, cardPlayer.at(i).at(0));
		*(pos + (i * 3) + 2) = cardPlayer.at(i).at(1);
	}
}
