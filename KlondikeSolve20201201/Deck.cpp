#include <vector>
#include "Deck.h"
#include "class_random_sampler.h"
using namespace std;

Deck::Deck()
	:currentIndex(0)
{
	Populate();
}
Deck::~Deck(void)
{
}
void Deck::Populate()
{
	int index = 0;
	for (int i = 0; i < SUIT_SIZE; i++)
	{
		for (int j = 0; j < RANK_SIZE; j++)
		{
			_deck[index] = Card(RANKS[j], SUITS[i]);
			// cout << *_deck[index];
			index++;
		}
	}
}
void Deck::Shuffle(DRWS* prg)
{

	int max = SUIT_SIZE * RANK_SIZE;
	Card new_deck[52];
	bool is_drws = false;
	for (int i = 0; i < max - 1; i++)
	{
		int choice = prg->d_uniform(max - i);
		//‹ó‚¯‚é‚Æ‚±‚ë‚ÆÅŒã‚Ì”š‚ğ“ü‚ê‘Ö‚¦
		Card tmp = _deck[choice + i];
		_deck[choice + i] = _deck[i];
		_deck[i] = tmp;
	}
	
	/*
	for (int i = 0; i < max; i++) {
		cout << _deck[i].GetCardSuit() << _deck[i].GetCardRank() << " " << endl;
	}
	*/
}
void Deck::PrintDeck(void)
{
	int max = SUIT_SIZE * RANK_SIZE;
	for (int i = 0; i < max; i++)
	{
		if (i % 13 == 0) {
			cout << endl << endl;
		}
		cout << _deck[i].GetCardSuit() << ":" << _deck[i].GetCardRank() << "  ";
	}
	cout << endl;
}
void Deck::PopulateVector(BuildStack& aDeck)
{
	int max = SUIT_SIZE * RANK_SIZE;
	aDeck.Clear();
	for (int i = 0; i < max; i++)
		aDeck.Push(&_deck[i]);
}
