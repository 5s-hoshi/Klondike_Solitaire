#pragma once
#include "Card.h"
#include <vector>
class BuildStack
{
public:
	BuildStack(int max_size);									// �R���X�g���N�^
	~BuildStack();												// �f�X�g���N�^
	Card& operator[](int index);								// []�̃I�[�o�[���[�h,BuildStack S[i]��i�Ԗڂ̃J�[�h��Ԃ�
																// �w���Ă�index���͈͊O�̎��͂Ȃɂ����Ȃ�

	bool Push(Card* card);										// _maxSize - 1 �ȉ��Ȃ�J�[�h��}����true��Ԃ��Aelse false��Ԃ�
	//bool PushValueCopy(Card card);
	int Size();
	bool empty();												// _size��0���ǂ���
	Card& top();												// ���[(��ԏ�)�̃J�[�h��Ԃ�

	bool RemoveAt(int index);									// index�Ԗڂ̃J�[�h������
	bool Pop();													// ��ԏ�̃J�[�h������
	void Clear();												// _cards��S������ 
	static void MoveBetween(BuildStack& from, BuildStack& to);	// �擪�𓮂���

private:
	int _max_size;												// �ő喇��
	int _size;													// ���݂̖���
	std::vector<Card*> _cards;									// ���݂���J�[�h�̃|�C���^
};