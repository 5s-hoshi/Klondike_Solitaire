#include "Card.h"
Card::Card(void)
{
}
Card::Card(char r, char s)
	:rank(r)
	, suit(s)
	, is_face_up(false)
	, child(NULL)
	, parent(NULL)
{
}
Card::~Card(void)
{
}
void Card::Flip()
{
	is_face_up = !is_face_up;
}
bool Card::GetIsFaceUp()
{
	return is_face_up;
}
char Card::GetCardSuit()
{
	return suit;
}
char Card::GetCardRank()
{
	return rank;
}
int Card::GetSolitaireValue()
{
	if (rank == 'A')
		return 1;
	else if (rank == 'T')
		return 10;
	else if (rank == 'J')
		return 11;
	else if (rank == 'Q')
		return 12;
	else if (rank == 'K')
		return 13;
	else{
		char c[] = { rank, '\0' };
		return atoi(c);
	}
}
std::ostream& operator<< (std::ostream& os, Card& c){
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	// クローバーとスペードは青で表す
	if (c.suit == 'c' || c.suit == 's') {
		SetConsoleTextAttribute(handle, BLUE);
	}
	// ダイヤとハートは赤で表す
	else {
		SetConsoleTextAttribute(handle, RED);
	}
	os << c.suit << c.rank;
	SetConsoleTextAttribute(handle, WHITE);
	return os;
}