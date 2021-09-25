#pragma once
#include "Card.h"
#include <vector>
class BuildStack
{
public:
	BuildStack(int max_size);									// コンストラクタ
	~BuildStack();												// デストラクタ
	Card& operator[](int index);								// []のオーバーロード,BuildStack S[i]がi番目のカードを返す
																// 指してるindexが範囲外の時はなにもしない

	bool Push(Card* card);										// _maxSize - 1 以下ならカードを挿入しtrueを返す、else falseを返す
	//bool PushValueCopy(Card card);
	int Size();
	bool empty();												// _sizeが0かどうか
	Card& top();												// 末端(一番上)のカードを返す

	bool RemoveAt(int index);									// index番目のカードを消す
	bool Pop();													// 一番上のカードを消す
	void Clear();												// _cardsを全部消す 
	static void MoveBetween(BuildStack& from, BuildStack& to);	// 先頭を動かす

private:
	int _max_size;												// 最大枚数
	int _size;													// 現在の枚数
	std::vector<Card*> _cards;									// 存在するカードのポインタ
};