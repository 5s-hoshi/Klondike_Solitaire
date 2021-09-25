#include "BuildStack.h"
#include <iostream>
BuildStack::BuildStack(int max_size)
	:_size(0)
	,_max_size(max_size)
{
	_cards.resize(max_size);
}
BuildStack::~BuildStack(void)
{
	// std::cout << "check2";
}
Card& BuildStack::operator[](int index)
{
	if (index >= 0 && index <= _size)
		return *_cards[index];
}
bool BuildStack::Push(Card* card)
{
	if (_size < _max_size)
	{
		_cards[_size] = card;										//カードのアドレスを挿入する
		_size++;
		return true;
	}
	return false;
}
/*
bool BuildStack::PushValueCopy(Card card)
{
	if (_size < _max_size);
	{
		_cards[_size] = new Card(card);
		_size++;
		return true;
	}
	return false;
}
*/
int BuildStack::Size()
{
	return _size;
}
bool BuildStack::empty()
{
	if (_size == 0)
		return true;
	return false;
}
Card& BuildStack::top(void)
{
	if (_size > 0)
		return *_cards[_size - 1];
}
bool BuildStack::RemoveAt(int index)
{
	if (index >= 0 && index < _max_size)				
	{
		_cards[index] = nullptr;
		_size--;
		return true;
	}
	return false;
}
bool BuildStack::Pop()
{
	if (_size > 0)
	{
		_cards[_size - 1] = nullptr;
		_size--;
		return true;
	}
	return false;
}
void BuildStack::Clear()
{
	for (int i = 0; i < _size; i++)
		_cards[i] = nullptr;

	_size = 0;
}
void BuildStack::MoveBetween(BuildStack& from, BuildStack& to)
{
	to.Push(&from.top());
	from.Pop();											
}

