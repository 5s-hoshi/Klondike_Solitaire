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
	void Flip();														// カードの表裏をひっくり返す
	bool GetIsFaceUp();													// カードが表か裏か
	int GetSolitaireValue();											// カードの数値を取得
	char GetCardSuit();													// カードの種類を１文字で表す
	char GetCardRank();													// カードの数字を１文字で表す 'A','2'～'9','T','J','Q','K'
	 
	friend std::ostream& operator<< (std::ostream& os, Card& c);		// << のオーバーロード、friend関数として
																		// 色付きで表したいため
																		// ダイヤとハートは赤で,クローバーとスペードは青で表す

	Card* parent;														// 上につながっているカード
	Card* child;														// 下につながっているカード

private:
	char rank;															// 自分自身の数字
	char suit;															// 自分自身の種類
	bool is_face_up;													// 自分が表か裏か
};