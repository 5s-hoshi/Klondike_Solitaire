#pragma once
#include "Card.h"
#include <ctime>
#include <iostream>
#include "BuildStack.h"
#include "class_random_sampler.h"
const static int RANK_SIZE = 13;
const static int SUIT_SIZE = 4;
const static char RANKS[] = { 'A','2','3','4','5','6','7','8','9','T','J','Q','K' };
const static char SUITS[] = { 'h','d','s','c' };
class Deck
{
public:
	Deck();
	~Deck();
	void Populate();										// �J�[�h���f�b�L�ɓ����
	void Shuffle(DRWS* prn);											// ���u��
	void PrintDeck();										// �f�b�L�̓��e��\��
	void PopulateVector(BuildStack& aDeck);					// �f�b�L��BuildStack�ɃR�s�[

private:
	Card _deck[52];
	int currentIndex;
};
