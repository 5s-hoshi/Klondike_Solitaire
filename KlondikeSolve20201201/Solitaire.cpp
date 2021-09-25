#include "Solitaire.h"
using namespace std;

//////////////////////////////////////////////////
/*					����						*/
/*		�f�b�L������ăV���b�t���A_pile��		*/
/*					 ��							*/
/*		�ő�20����BuildStack���w����		*/
/*					 ��							*/	
/*		�ő�13����suitstack��4���			*/
/*					 ��							*/	
/*			BuildStack�ɃJ�[�h��z��			*/
//////////////////////////////////////////////////

Solitaire::Solitaire(DRWS* prg)
	:_pile(52)
	, _discarded_cards(52)
{
	_deck = Deck();
	_deck.Shuffle(prg);
	_deck.PopulateVector(_pile);
	/*
	for (int i = 0; i < 52; i++) {
		cout << _pile[i];
	}
	cin.get();
	//*/
	for (int i = 0; i < BUILD_STACK_SIZE; i++)	{
		BuildStack buildstack = BuildStack(20);
		_buildstacks.push_back(buildstack);
	}

	for (int i = 0; i < SUIT_STACK_SIZE; i++)	{
		BuildStack suitstack = BuildStack(13);
		_suitstacks.push_back(suitstack);
	}
	PopulateBuildStack();
}
Solitaire::~Solitaire()
{
	// cout << "check1";
}
void Solitaire::PopulateBuildStack()
{
	for (int i = 0; i < BUILD_STACK_SIZE; i++)	
	{
		for (int j = 0; j < BUILD_STACK_SIZE - i; j++)	
		{
			//�@�ebuildstack�̃g�b�v�ɂ���card��\�ɂ�����ԂŃZ�b�g
			if (j == (BUILD_STACK_SIZE - i) - 1) 
			{
				_pile.top().Flip();
			}
			_buildstacks[j].Push(&_pile.top());
			_pile.Pop();
		}
	}
	/*
	for (int i = 0; i < BUILD_STACK_SIZE; i++) {
		cout << i << "th bs : ";
		for (int j = 0; j < BUILD_STACK_SIZE - i; j++) {
			cout << j << " : " << _buildstacks[i][j];
			cin.get();
		}
		cout << endl;
	}
	cin.get();
	//*/
}
void Solitaire::Deal(int num_to_deal)
{
	int size = _pile.Size();
	for (int i = 0; i < size; i++)	
	{
		// �J�[�h�̐擪���\�Ɍ����Ă���Η��Ԃ��A�̂Ă�
		if (_pile.top().GetIsFaceUp())	
		{
			_pile.top().Flip();
			_discarded_cards.Push(&_pile.top());
			_pile.Pop();
		}
	}
	// _pile���Ȃ��Ȃ������납��_pile�ɖ߂��Ă���
	if (_pile.empty())
	{
		for (int i = _discarded_cards.Size() - 1; i >= 0; i--)
		{
			_pile.Push(&_discarded_cards[i]);
		}

		_discarded_cards.Clear();
	}
	int index = 0;
	for (int i = 0; i < num_to_deal; i++)	
	{
		index = _pile.Size() - i - 1;
		if (index >= 0)	
		{
			_pile[index].Flip();
		}
		else {
			break;
		}
	}
}
void Solitaire::PrintSuitDetails()
{
	cout << "=============================================\n";
	for (int j = 0; j < _suitstacks.size(); j++)
	{
		switch (j)	
		{
		case 0:
			cout << "h :";
			break;
		case 1:
			cout << "d :";
			break;
		case 2:
			cout << "s :";
			break;
		case 3:
			cout << "c :";
			break;
		}

		for (int i = 0; i < _suitstacks[j].Size(); i++)	
		{
			if (&_suitstacks[j][i] != NULL)
				cout << _suitstacks[j][i] << " ";
			else
				break;
		}
		cout << endl;
	}
	cout << "=============================================\n";
	cout << endl;
}
void Solitaire::PrintTalonDetails()	
{
	cout << "--------------------------------------------\n";
	cout << "|TALON: ";
	for (unsigned int i = 0; i < _pile.Size(); i++)	
	{
		if (_pile[i].GetIsFaceUp())
			cout << _pile[i] << " ";

	}
	cout << "\n--------------------------------------------\n";
}
void Solitaire::PrintAllDetails(bool show_all)
{
	PrintTalonDetails();
	PrintSuitDetails();
	cout << "| 6 | \t | 5 | \t | 4 | \t | 3 | \t | 2 | \t | 1 | \t | 0 |\n";
	cout << "----- \t ----- \t ----- \t ----- \t ----- \t ----- \t -----\n";

	int col_max = _buildstacks.size();
	int row_max = 0;
	// BuildStack������

	for (int i = 0; i < col_max; i++)
	{
		if (_buildstacks[i].Size() > row_max)
		{
			row_max = _buildstacks[i].Size();
		}
	}
	for (int i = 0; i < row_max; i++)
	{
		for (int j = col_max - 1; j >= 0; j--)
		{
			if (i >= _buildstacks[j].Size())
			{
				cout << " \t ";
				continue;
			}

			if (_buildstacks[j][i].GetIsFaceUp())
			{
				cout << "  " << _buildstacks[j][i] << " \t ";
			}
			else if (show_all)
			{
				cout << "  " << _buildstacks[j][i] << " \t ";
			}
			else
			{
				cout << "  " << (char)254 << " \t ";
			}
		}
		cout << endl;
	}
	cout << endl << endl;

	// �f�b�L������
	if (show_all)
	{
		cout << "\nCards in the pile: \n\n";
		for (int i = 0; i < _pile.Size(); i++)
			cout << _pile[i] << " ";
		cout << endl << endl;
		// �̂ĎD������
		cout << "\nCards in the used pile: \n\n";
		for (unsigned int i = 0; i < _discarded_cards.Size(); i++)
			cout << _discarded_cards[i] << " ";
		cout << endl << endl;

		// �e�q������
		cout << "BuildStack's adoption:\n";

		for (int i = 0; i < _buildstacks.size(); i++) 
		{
			cout << i << ": \n";
			for (int j = 0; j < _buildstacks[i].Size(); j++)
			{
				if (_buildstacks[i][j].parent != nullptr)
				{
					cout << _buildstacks[i][j] << "'s parent is " << *_buildstacks[i][j].parent << endl;
				}
				if (_buildstacks[i][j].child != nullptr)
				{
					cout << _buildstacks[i][j] << "'s child is " << *_buildstacks[i][j].child << endl;
				}
			}
		}
		cout << "\nSuitStack's adoption:\n";

		for (int i = 0; i < _suitstacks.size(); i++)
		{
			switch (i)
			{
			case 0:
				cout << "h :\n";
				break;
			case 1:
				cout << "d :\n";
				break;
			case 2:
				cout << "s :\n";
				break;
			case 3:
				cout << "c :\n";
				break;
			}
			for (int j = 0; j < _suitstacks[i].Size(); j++)
			{
				if (_suitstacks[i][j].parent != nullptr)
				{
					cout << _suitstacks[i][j] << "'s parent is " << *_suitstacks[i][j].parent << endl;
				}
				if (_suitstacks[i][j].child != nullptr)
				{
					cout << _suitstacks[i][j] << "'s child is " << *_suitstacks[i][j].child << endl;
				}
			}
		}
	}
}
bool Solitaire::ValidBuildMove(int from, int to)
{
	//�����Ƃ���ֈړ��͂ł��Ȃ�
	if (from == to && from != 7)
		return false;
	Card* to_card = nullptr;
	Card* from_card = nullptr;
	// talon����
	if (from == 7)	
	{
		if (!_pile.empty())	
		{
			// talon�ɉ����Ȃ��Ƃ�
			if (!_pile.top().GetIsFaceUp())	
			{
				return false;
			}
			else 
			{
				from_card = &_pile.top();
			}
		}
		else 
		{
			return false;
		}
	}
	else
	{
		if (!_buildstacks[from].empty())
		{
			from_card = &_buildstacks[from].top();
		}
		// �ړ���buildstack����
		else 
		{
			return false;
		}
	}
	if (!_buildstacks[to].empty()) 
	{
		to_card = &_buildstacks[to].top();
	}
	// �ړ���buildstack����
	if (_buildstacks[to].empty())
	{
		// King�݈̂ړ���
		if (from_card->GetCardRank() == 'K') 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	// �����̏���
	else if (from_card->GetSolitaireValue() == to_card->GetSolitaireValue() - 1)	
	{
		//�@�F�̏���
		int to_color = 0, from_color = 0;
		//�@��
		if (to_card->GetCardSuit() == 'h' || to_card->GetCardSuit() == 'd') 
		{
			to_color = 0;
		}
		// ��
		else
		{
			to_color = 1;
		}
		// ��
		if (from_card->GetCardSuit() == 'h' || from_card->GetCardSuit() == 'd') 
		{
			from_color = 0;
		}
		// ��
		else 
		{
			from_color = 1;
		}
		if (from_color == to_color) 
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}
bool Solitaire::ValidToSuitMove(int from)
{
	Card* from_card = nullptr;
	// talon����
	if (from == 7)
	{
		if (!_pile.empty())	
		{
			if (!_pile.top().GetIsFaceUp())	
			{
				return false;
			}
			else
			{
				from_card = &_pile.top();
			}
		}
		else
		{
			return false;
		}
	}
	// buildstack����
	else
	{
		if (!_buildstacks[from].empty())
			from_card = &_buildstacks[from].top();
		else
			return false;
	}
	char suit = from_card->GetCardSuit();
	int move_index = 0;
	switch (suit)
	{
	case 'h':
		move_index = 0;
		break;
	case 'd':
		move_index = 1;
		break;
	case 's':
		move_index = 2;
		break;
	case 'c':
		move_index = 3;
		break;
	}
	if (from_card->GetSolitaireValue() == _suitstacks[move_index].Size() + 1)
		return true;


	return false;
}
void Solitaire::MakeToSuitMove(int from)
{
	if (ValidToSuitMove(from)) {
		Card* from_card = nullptr;
		// talon����
		if (from == 7)
			from_card = &_pile.top();
		// buildstack����
		else
			from_card = &_buildstacks[from].top();
		char suit = from_card->GetCardSuit();
		int suit_index = 0;
		switch (suit)
		{
		case 'h':
			suit_index = 0;
			break;
		case 'd':
			suit_index = 1;
			break;
		case 's':
			suit_index = 2;
			break;
		case 'c':
			suit_index = 3;
			break;
		}
		if (from_card->GetSolitaireValue() == _suitstacks[suit_index].Size() + 1)
		{
			// �ړ���buildstack�̐e�̐≏
			if (from_card->parent != nullptr)
			{
				from_card->parent->child = nullptr;
			}
			// ���ł�suitstack��card������ꍇ�͐e�w��
			if (_suitstacks[suit_index].Size() != 0)
			{
				// �q�̉��g
				from_card->parent = &_suitstacks[suit_index].top();
				from_card->child = nullptr;
				// suitstack�̐e�̐≏
				_suitstacks[suit_index].top().child = from_card;
			}
			// suitstack��card���Ȃ��ꍇ
			else
			{
				from_card->parent = nullptr;
				from_card->child = nullptr;
			}
			// buildstack �� suitstack
			if (from != 7)
			{
				BuildStack::MoveBetween(_buildstacks[from], _suitstacks[suit_index]);
				//�ړ��������オ��łȂ��A���Ȃ�\������
				if (!_buildstacks[from].empty())
				{
					if (!_buildstacks[from].top().GetIsFaceUp())
						_buildstacks[from].top().Flip();
				}
			}
			// talon �� suitstack
			else
			{
				_suitstacks[suit_index].Push(&_pile.top());
				_pile.Pop();
			}
		}
	}
}
void Solitaire::MakeMoveBetweenRows(int from, int to) 
{
	if (ValidBuildMove(from, to)) 
	{	
		// �e���̉��g
		if (!_buildstacks[to].empty()) 
		{
			_buildstacks[to].top().child = &_buildstacks[from].top();
		}
		// �q���̉��g
		if (_buildstacks[from].top().GetCardRank() != 'K') 
		{
			_buildstacks[from].top().parent = &_buildstacks[to].top();
		}
		BuildStack::MoveBetween(_buildstacks[from], _buildstacks[to]);
		
		// �ړ���ɕ\������
		if (!_buildstacks[from].empty()) 
		{
			if (!_buildstacks[from].top().GetIsFaceUp())
			{
				_buildstacks[from].top().Flip();
			}
		}
	}
}
void Solitaire::MakeMoveDeckToRow(int to)
{	
	if (ValidBuildMove(7, to))
	{
		// �e���̉��g
		if (!_buildstacks[to].empty()) {
			_buildstacks[to].top().child = &_pile.top();
		}
		// �q���̉��g
		if (_pile.top().GetCardRank() != 'K') {
			_pile.top().parent = &_buildstacks[to].top();
		}
		_buildstacks[to].Push(&_pile.top());
		_pile.Pop();
	}
}
bool Solitaire::ValidFromSuitMove(char suit, int to)
{

	Card* from_card = nullptr;
	Card* to_card = nullptr;
	int from_index = 0;
	switch (suit)
	{
	case 'h':
		from_index = 0;
		break;
	case 'd':
		from_index = 1;
		break;
	case 's':
		from_index = 2;
		break;
	case 'c':
		from_index = 3;
		break;
	}
	if (_suitstacks[from_index].empty())
		return false;

	from_card = &_suitstacks[from_index].top();
	
	if (!_buildstacks[to].empty())
	{
		to_card = &_buildstacks[to].top();
	}
	// �ړ���buildstack����
	if (_buildstacks[to].empty())
	{
		// King�݈̂ړ���
		if (from_card->GetCardRank() == 'K')
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// �����̏���
	else if (from_card->GetSolitaireValue() == to_card->GetSolitaireValue() - 1)
	{
	//�@�F�̏���
	int to_color = 0, from_color = 0;
	//�@��
	if (to_card->GetCardSuit() == 'h' || to_card->GetCardSuit() == 'd')
	{
		to_color = 0;
	}
	// ��
	else
	{
		to_color = 1;
	}
	// ��
	if (from_card->GetCardSuit() == 'h' || from_card->GetCardSuit() == 'd')
	{
		from_color = 0;
	}
	// ��
	else
	{
		from_color = 1;
	}
	if (from_color == to_color)
	{
		return false;
	}
	else
	{
		return true;
	}
	}
	return false;
}
void Solitaire::MakeFromSuitMove(char suit, int to)
{
	if (ValidFromSuitMove(suit, to))
	{	
		int from_index = 0;
		switch (suit)
		{
		case 'h':
			from_index = 0;
			break;
		case 'd':
			from_index = 1;
			break;
		case 's':
			from_index = 2;
			break;
		case 'c':
			from_index = 3;
			break;
		}
		// �ړ���e���̉��g
		if ((!_suitstacks[from_index].empty()) && _suitstacks[from_index].top().GetCardRank() != 'K')
		{
			_buildstacks[to].top().child = &_suitstacks[from_index].top();
		}
		// �ړ����e���̉��g
		/*
		cout << "check 1\n";
		cout << _suitstacks[from_index].Size();
		cin.get();
		//*/
		if (_suitstacks[from_index].Size() > 1) {
			/*
			cout << "check point\n";
			cin.get();
			//*/
			Card* from_card = nullptr;
			from_card = &_suitstacks[from_index].top();
			from_card->parent->child = nullptr;
		}
		// �q���̉��g
		if (_suitstacks[from_index].top().GetCardRank() != 'K')
			_suitstacks[from_index].top().parent = &_buildstacks[to].top();
		else
			_suitstacks[from_index].top().parent = nullptr;
		BuildStack::MoveBetween(_suitstacks[from_index], _buildstacks[to]);	
	}
}
bool Solitaire::ValidToRowMove(int from, int to)
{

	if (_buildstacks[from].empty())
		return false;
	else
	{
		Card* from_card = nullptr;
		from_card = &_buildstacks[from].top();
		int num = 1;
		if (from_card == nullptr)
		{
			cout << "ptr is nullptr!\n";
			cin.get();
		}
		while (from_card->parent != nullptr)
		{
			from_card = from_card->parent;
			num++;
		}
		// �s���悪��Ȃ�king�̂݋��e�Ak�܂݂̃J�[�h�Q�̔������Ƃт͋��e���Ȃ�
		if (_buildstacks[to].empty()) 
		{
			if (from_card->GetCardRank() == 'K' && _buildstacks[from].Size() > num) 
				return true;
			else
				return false;
		}
		else
		{
			Card* to_card = nullptr;
			to_card = &_buildstacks[to].top();
			if (from_card->GetSolitaireValue() == to_card->GetSolitaireValue() - 1) {
				int to_color = 0, from_color = 0;
				if (to_card->GetCardSuit() == 'h' || to_card->GetCardSuit() == 'd') {
					to_color = 0;
				}
				else {
					to_color = 1;
				}
				if (from_card->GetCardSuit() == 'h' || from_card->GetCardSuit() == 'd') {
					from_color = 0;
				}
				else {
					from_color = 1;
				}
				if (from_color == to_color) {
					return false;
				}
				else {
					return true;
				}
			}
			return false;
		}
	}
}
void Solitaire::MakeMoveToRow(int from, int to, char suit)
{	
	// Deck����
	if (from == 7)
	{
		MakeMoveDeckToRow(to);
		//talon���g���؂�����̂ĎD��top����g���Ă�����
		if (!_pile.empty())
		{
			if (!_pile.top().GetIsFaceUp() && !_discarded_cards.empty())
			{
				_discarded_cards.top().Flip();
				_pile.Push(&_discarded_cards.top());
				_discarded_cards.Pop();
			}
		}
		else if (!_discarded_cards.empty()) {
			_discarded_cards.top().Flip();
			_pile.Push(&_discarded_cards.top());
			_discarded_cards.Pop();
		}
		return;
	}
	// Suit����
	else if (from == 8) {
		MakeFromSuitMove(suit, to);
		return;
	}

	// buildstack��
	else if (_buildstacks[from].Size() == 0) {
		return;
	}
	Card* from_card = nullptr;
	from_card = &_buildstacks[from].top();
	// �e�����Ȃ��A�܂�1���݈̂ړ�
	if (from_card->parent == nullptr) {
		MakeMoveBetweenRows(from, to);
	}
	// �������ړ��̉\��
	else{
		int pos = _buildstacks[from].Size() - 1;
		// �e���ő�܂ł��ǂ�
		if (ValidToRowMove(from, to)) {
			while (from_card->parent != nullptr) {
				from_card = from_card->parent;
			}
			bool has_children = true;
			/*
			// �ړ������ԏ�̃J�[�h�ɑ΂���e�̐≏�A���݂͂��̏����͍s���Ȃ�
			if (from_card->parent != nullptr) {
				from_card->parent->child = nullptr;
			}
			//*/
			/*
			for (int _ = 0; _ < _buildstacks[from].Size(); _++)
			{
				cout << "add:" << &_buildstacks[from][_]  <<" buildstack[" << from << "][" << _ << "] :" << _buildstacks[from][_] << endl;
				cout << "child's add:" << _buildstacks[from][_].child << endl;
			}
			cout << endl;
			//*/
			while (has_children)
			{
				/*
				cout << "add:" << &_buildstacks[from][pos] << " buildstack[" << from << "][" << pos << "] :" << _buildstacks[from][pos];
				//*/
				// �ړ�����J�[�h�̉��g�ƍs����̃J�[�h�̉��g
				if (!_buildstacks[to].empty()) 
				{
					from_card->parent = &_buildstacks[to].top();
					_buildstacks[to].top().child = from_card;
				}
				/*
				cout << endl;
				cout << "pos=" << pos << "  \t ";
				cout << "remove:" << _buildstacks[from][pos] << endl;
				//*/
				_buildstacks[to].Push(from_card);
				// BuildStack operator[]�̊֌W�Ō�납������Ă䂭
				_buildstacks[from].RemoveAt(pos);
				from_card = from_card->child;

				/*
				cout << "now from card add:" << from_card << endl;
				//*/
				
				pos--;
				if (from_card == nullptr) {
					has_children = false;
				}
			}
		}
		// �ړ����J�[�h��\��
		if (!_buildstacks[from].empty()) {
			if (!_buildstacks[from].top().GetIsFaceUp()) {
				_buildstacks[from].top().Flip();
			}
		}
	}
}
bool Solitaire::GameCompleted(){
	for (int i = 0; i < _suitstacks.size(); i++) {
		if (_suitstacks[i].Size() < 13) {
			return false;
		}
	}
	return true;
}