#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include "class_random_sampler.h"
#include "Deck.h"
#include "Card.h"
#include "BuildStack.h"
const int BUILD_STACK_SIZE = 7;
const int SUIT_STACK_SIZE = 4;


class Solitaire
{
public:
	Solitaire(DRWS* prg);
	~Solitaire();
	void PopulateBuildStack();									// BuildStacksにカードを配置
	void Deal(int num_to_deal = 1);								// pileからnum_to_deal枚カードをtalonにする

	// カード移動用
	void MakeToSuitMove(int from);								// suitstackに動かす
	void MakeFromSuitMove(char suit, int to);					// suitstackから動かす
	void MakeMoveBetweenRows(int from, int to);					// bildstack間移動
	void MakeMoveDeckToRow(int to);								// talonからbuildstackに
	void MakeMoveToRow(int from, int to, char suit);			// card移動、外からはこれで呼ぶ
	bool ValidFromSuitMove(char suit, int to);				
	bool ValidBuildMove(int from, int to);						
	bool ValidToRowMove(int from, int to);
	bool ValidToSuitMove(int from);
	// クリアチェック用
	bool GameCompleted();
	// カード描写用
	void PrintAllDetails(bool show_all = false);
	void PrintSuitDetails();
	void PrintTalonDetails();
private:
	Deck _deck;
	BuildStack _pile;
	BuildStack _discarded_cards;
	std::vector<BuildStack> _buildstacks;
	std::vector<BuildStack> _suitstacks;
};