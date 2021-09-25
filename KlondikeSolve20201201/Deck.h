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
	void Populate();										// カードをデッキに入れる
	void Shuffle(DRWS* prn);											// 仮置き
	void PrintDeck();										// デッキの内容を表示
	void PopulateVector(BuildStack& aDeck);					// デッキをBuildStackにコピー

private:
	Card _deck[52];
	int currentIndex;
};
