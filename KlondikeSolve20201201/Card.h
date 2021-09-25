#pragma once
#include <iostream>
#include <windows.h>
const static int BLACK = 0;
const static int BLUE = 9;
const static int RED = 12;
const static int WHITE = 15;

class Card
{
public:
	Card();
	Card(char rank, char suit);
	~Card();
	void Flip();														// �J�[�h�̕\�����Ђ�����Ԃ�
	bool GetIsFaceUp();													// �J�[�h���\������
	int GetSolitaireValue();											// �J�[�h�̐��l���擾
	char GetCardSuit();													// �J�[�h�̎�ނ��P�����ŕ\��
	char GetCardRank();													// �J�[�h�̐������P�����ŕ\�� 'A','2'�`'9','T','J','Q','K'
	 
	friend std::ostream& operator<< (std::ostream& os, Card& c);		// << �̃I�[�o�[���[�h�Afriend�֐��Ƃ���
																		// �F�t���ŕ\����������
																		// �_�C���ƃn�[�g�͐Ԃ�,�N���[�o�[�ƃX�y�[�h�͐ŕ\��

	Card* parent;														// ��ɂȂ����Ă���J�[�h
	Card* child;														// ���ɂȂ����Ă���J�[�h

private:
	char rank;															// �������g�̐���
	char suit;															// �������g�̎��
	bool is_face_up;													// �������\������
};