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
	void PopulateBuildStack();									// BuildStacks�ɃJ�[�h��z�u
	void Deal(int num_to_deal = 1);								// pile����num_to_deal���J�[�h��talon�ɂ���

	// �J�[�h�ړ��p
	void MakeToSuitMove(int from);								// suitstack�ɓ�����
	void MakeFromSuitMove(char suit, int to);					// suitstack���瓮����
	void MakeMoveBetweenRows(int from, int to);					// bildstack�Ԉړ�
	void MakeMoveDeckToRow(int to);								// talon����buildstack��
	void MakeMoveToRow(int from, int to, char suit);			// card�ړ��A�O����͂���ŌĂ�
	bool ValidFromSuitMove(char suit, int to);				
	bool ValidBuildMove(int from, int to);						
	bool ValidToRowMove(int from, int to);
	bool ValidToSuitMove(int from);
	// �N���A�`�F�b�N�p
	bool GameCompleted();
	// �J�[�h�`�ʗp
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