#include "strategy.h"
using namespace std;


string strategy_6(Solitaire* game, DRWS* prg, unsigned int weight, unsigned int resistance, unsigned int deal_resistance, unsigned int *d_cnter, bool print_all)
{
	const int from_num = BUILD_STACK_SIZE + SUIT_STACK_SIZE + 1; // 12

	// 可能な行動
	int options_to_suitstack[BUILD_STACK_SIZE + 1];
	int options_to_buildstack[from_num * BUILD_STACK_SIZE];
	int options_from_talon[2];
	vector<tuple<int, char, int>> valid_options;
	vector<tuple<int, char, int>> valid_options_talon;
	int option_from_s = 0, option_to_s = 0, option_others = 0, option_from_t = 0;


	for (int i = 0; i < from_num * BUILD_STACK_SIZE; i++) {
		int from = i / BUILD_STACK_SIZE, to = i % BUILD_STACK_SIZE;
		// 同じ所への移動は許さない
		if (from == to)
			continue;
		// suitstackから
		if (from > 7)
		{	
			char from_suit;
			switch (from)
			{
			case 8:
				from_suit = 'h';
				break;
			case 9:
				from_suit = 'd';
				break;
			case 10:
				from_suit = 's';
				break;
			case 11:
				from_suit = 'c';
				break;
			}
			//cout << "from " << from_suit << " to " << to;
			if (game->ValidFromSuitMove(from_suit, to))
			{
				//cout << " is valid!\n";
				options_to_buildstack[i] = 1;
				tuple<int, char, int> t(8, from_suit, to);
				valid_options.push_back(t);
				option_from_s++;
			}
			
			//cout << "is invalid...\n";
		}
		// Deckから
		else if (from == 7)
		{
			if (game->ValidBuildMove(7, to))
			{
				options_from_talon[0] = 1;
				tuple<int, char, int> t(7, '_', to);
				valid_options_talon.push_back(t);
				option_from_t++;
			}
		}
		// BuildStack間
		else
		{
			if (game->ValidToRowMove(from, to))
			{
				options_to_buildstack[i] = 1;
				tuple<int, char, int> t(from, '_', to);
				valid_options.push_back(t);
				option_others++;
			}
		}
	}
	for (int i = 0; i < BUILD_STACK_SIZE; i++)
	{
		if (game->ValidToSuitMove(i))
		{
			options_to_suitstack[i] = 1;
			tuple<int, char, int> t(i, '_', 8);
			valid_options.push_back(t);
			option_to_s++;
		}
	}
	if (game->ValidToSuitMove(7))
	{
		options_from_talon[1] = 1;
		tuple<int, char, int> t(7, '_', 8);
		valid_options_talon.push_back(t);
		option_from_t++;
	}


	// valid_option には、(B->B), (T->B), (S->B), (B->S), (T->S)に入っている.

	// 重みづけ
	// (・->S) * W * D_R * R, (S->B) * D_R, (B->B)and(T->B) * D_R * R, (Deal) * R 
	int to_s_w = weight * deal_resistance * resistance;
	int from_s_w = deal_resistance;
	int others_w = resistance * deal_resistance;
	int deal_w = resistance;
	int options_size = option_to_s * to_s_w + option_from_s * from_s_w + option_others * others_w;

	if (print_all)
	{
		cout << "options are:\n";
		cout << "----------------------------------\n";
		for (int i = 0; i < option_from_t; i++) 
		{
			tuple<int, char, int> op = valid_options_talon[i];
			int from = get<0>(op), to = get<2>(op);
			char s = get<1>(op), f, t;
			if (from < 7)
				f = 'B';
			else if (from == 7)
				f = 'T';
			else
				f = 'S';
			if (to < 7)
				t = 'B';
			else
				t = 'S';
			cout << from << " " << s << " " << to << " \t (" << f << " -> " << t << ")\n";
		}


		for (int i = 0; i < option_from_s + option_to_s + option_others; i++)
		{
			tuple<int, char, int> op = valid_options[i];
			int from = get<0>(op), to = get<2>(op);
			char s = get<1>(op), f , t;
			if (from < 7)
				f = 'B';
			else if (from == 7)
				f = 'T';
			else
				f = 'S';
			if (to < 7)
				t = 'B';
			else
				t = 'S';
			cout << from << " " << s << " " << to <<  " \t (" << f << " -> " << t << ")\n";
		}
		cout << "deal\n";
		cout << "----------------------------------\n";
	}

	if (option_from_t > 0)
	{
		tuple<int, char, int> option;
		int to = 0;
		if (option_from_t == 1)
			option = valid_options_talon[0];
		else
		{
			int prn = prg->d_uniform(to_s_w + others_w);
			if (prn < to_s_w)
				option = valid_options_talon[1];
			else
				option = valid_options_talon[0];
		}
		to = get<2>(option);


		if (to == 8)
		{
			string ans = "2" + to_string(7);
			if (print_all)
			{
				cout << "command is " << ans << endl;
				cin.get();
			}
			return ans;
		}
		// buildstackに		
		else
		{
			string ans = to_string(17) + to_string(to);
			if (print_all)
			{
				cout << "----------------------------------\n";
				cout << "command is " << ans << endl;
				cin.get();
			}
			return ans;
		}

	}



	if (options_size != 0)
	{
		*d_cnter = 0;
		int prn = prg->d_uniform(options_size + deal_w);
		
		if (print_all)
		{
			cout << "deal counter = " << *d_cnter << endl << endl;
			cout << "uniform distribution on " << options_size + deal_w << "\n";
			cout << "w=" << weight << ", r=" << resistance << ", d_r=" << deal_resistance << endl;
			cout << "others_w=" << others_w << ", from_s_w=" << from_s_w << ", to_s_w=" << to_s_w << ", deal_w=" << deal_w << endl;
			cout << "options_size = " << options_size << endl << endl;
			cout << "prn = " << prn;
		}
		if (prn >= options_size)
		{
			if (print_all) {
				cout << ", chosen option = deal\n";
				cin.get();
			}
			return "3";
		}
		int option_num = -1;
		// (B->B), (T->B)
		for (int i = 0; i < option_others; i++)
		{
			option_num++;
			prn -= others_w;
			if (prn < 0)
				break;
		}
		// (S->B)
		if (prn >= 0)
		{
			for (int i = 0; i < option_from_s; i++) {
				option_num++;
				prn -= from_s_w;
				if (prn < 0)
					break;
			}
		}
		// (B->S), (T->S)
		if (prn >= 0)
			option_num += (prn / to_s_w) + 1;
	
		if (print_all) 
			cout <<", option_num is " << option_num << endl;
	

		tuple<int, char, int> option = valid_options[option_num];

		int from = get<0>(option), to = get<2>(option);
		char s = get<1>(option);
		if (print_all)
		{
			cout << "chosen option = ( " << from << ", " << s << ", " << to << ")\n";
		}
		//suitstackに
		if (to == 8)
		{
			string ans = "2" + to_string(from);
			if (print_all)
			{
				cout << "command is " << ans << endl;
				cin.get();
			}
			return ans;
		}
		// buildstackに		
		else if (s != '_')
		{
			string ans = to_string(18) + s + to_string(to);
			if (print_all)
			{
				cout << "----------------------------------\n";
				cout << "command is " << ans << endl;
				cin.get();
			}
			return ans;
		}
		else {
			string ans = "1" + to_string(from) + to_string(to);
			if (print_all)
			{
				cout << "----------------------------------\n";
				cout << "command is " << ans << endl;
				cin.get();
			}
			return ans;
		}
	}
	else {
		(*d_cnter)++;
		if (print_all) {
			cout << "chosen option = deal\n";
			cout << "deal counter = " << *d_cnter <<endl;
			cin.get();
		}
		return "3";
	}
}

string strategy_5(Solitaire* game, DRWS* prg, unsigned int weight, unsigned int resistance, unsigned int deal_resistance, unsigned int* d_cnter, bool print_all) 
{

	const int from_num = BUILD_STACK_SIZE + SUIT_STACK_SIZE + 1; // 12

	// 可能な行動
	int options_to_suitstack[BUILD_STACK_SIZE + 1];
	int options_to_buildstack[from_num * BUILD_STACK_SIZE];
	vector<tuple<int, char, int>> valid_options;
	int option_from_s = 0, option_to_s = 0, option_others = 0;


	for (int i = 0; i < from_num * BUILD_STACK_SIZE; i++) {
		int from = i / BUILD_STACK_SIZE, to = i % BUILD_STACK_SIZE;
		// 同じ所への移動は許さない
		if (from == to)
			continue;
		// suitstackから
		if (from > 7)
		{
			char from_suit;
			switch (from)
			{
			case 8:
				from_suit = 'h';
				break;
			case 9:
				from_suit = 'd';
				break;
			case 10:
				from_suit = 's';
				break;
			case 11:
				from_suit = 'c';
				break;
			}
			//cout << "from " << from_suit << " to " << to;
			if (game->ValidFromSuitMove(from_suit, to))
			{
				//cout << " is valid!\n";
				options_to_buildstack[i] = 1;
				tuple<int, char, int> t(8, from_suit, to);
				valid_options.push_back(t);
				option_from_s++;
			}

			//cout << "is invalid...\n";
		}
		// Deckから
		else if (from == 7)
		{
			if (game->ValidBuildMove(7, to))
			{
				options_to_buildstack[i] = 1;
				tuple<int, char, int> t(7, '_', to);
				valid_options.push_back(t);
				option_others++;
			}
		}
		// BuildStack間
		else
		{
			if (game->ValidToRowMove(from, to))
			{
				options_to_buildstack[i] = 1;
				tuple<int, char, int> t(from, '_', to);
				valid_options.push_back(t);
				option_others++;
			}
		}
	}
	for (int i = 0; i < BUILD_STACK_SIZE + 1; i++)
	{
		if (game->ValidToSuitMove(i))
		{
			options_to_suitstack[i] = 1;
			tuple<int, char, int> t(i, '_', 8);
			valid_options.push_back(t);
			option_to_s++;
		}
	}


	// valid_option には、(B->B), (T->B), (S->B), (B->S), (T->S)に入っている.

	// 重みづけ
	// (・->S) * W * D_R * R, (S->B) * D_R, (B->B)and(T->B) * D_R * R, (Deal) * R 
	int to_s_w = weight * deal_resistance * resistance;
	int from_s_w = deal_resistance;
	int others_w = resistance * deal_resistance;
	int deal_w = resistance;
	int options_size = option_to_s * to_s_w + option_from_s * from_s_w + option_others * others_w;

	if (print_all)
	{
		cout << "options are:\n";
		cout << "----------------------------------\n";
		for (int i = 0; i < option_from_s + option_to_s + option_others; i++)
		{
			tuple<int, char, int> op = valid_options[i];
			int from = get<0>(op), to = get<2>(op);
			char s = get<1>(op), f, t;
			if (from < 7)
				f = 'B';
			else if (from == 7)
				f = 'T';
			else
				f = 'S';
			if (to < 7)
				t = 'B';
			else
				t = 'S';
			cout << from << " " << s << " " << to << " \t (" << f << " -> " << t << ")\n";
		}
		cout << "deal\n";
		cout << "----------------------------------\n";
	}


	if (options_size != 0)
	{
		*d_cnter = 0;
		int prn = prg->d_uniform(options_size + deal_w);

		if (print_all)
		{
			cout << "deal counter = " << *d_cnter << endl << endl;
			cout << "uniform distribution on " << options_size + deal_w << "\n";
			cout << "w=" << weight << ", r=" << resistance << ", d_r=" << deal_resistance << endl;
			cout << "others_w=" << others_w << ", from_s_w=" << from_s_w << ", to_s_w=" << to_s_w << ", deal_w=" << deal_w << endl;
			cout << "options_size = " << options_size << endl << endl;
			cout << "prn = " << prn;
		}
		if (prn >= options_size)
		{
			if (print_all) {
				cout << ", chosen option = deal\n";
				cin.get();
			}
			return "3";
		}
		int option_num = -1;
		// (B->B), (T->B)
		for (int i = 0; i < option_others; i++)
		{
			option_num++;
			prn -= others_w;
			if (prn < 0)
				break;
		}
		// (S->B)
		if (prn >= 0)
		{
			for (int i = 0; i < option_from_s; i++) {
				option_num++;
				prn -= from_s_w;
				if (prn < 0)
					break;
			}
		}
		// (B->S), (T->S)
		if (prn >= 0)
			option_num += (prn / to_s_w) + 1;

		if (print_all)
			cout << ", option_num is " << option_num << endl;


		tuple<int, char, int> option = valid_options[option_num];

		int from = get<0>(option), to = get<2>(option);
		char s = get<1>(option);
		if (print_all)
		{
			cout << "chosen option = ( " << from << ", " << s << ", " << to << ")\n";
		}
		//suitstackに
		if (to == 8)
		{
			string ans = "2" + to_string(from);
			if (print_all)
			{
				cout << "command is " << ans << endl;
				cin.get();
			}
			return ans;
		}
		// buildstackに		
		else if (s != '_')
		{
			string ans = to_string(18) + s + to_string(to);
			if (print_all)
			{
				cout << "----------------------------------\n";
				cout << "command is " << ans << endl;
				cin.get();
			}
			return ans;
		}
		else {
			string ans = "1" + to_string(from) + to_string(to);
			if (print_all)
			{
				cout << "----------------------------------\n";
				cout << "command is " << ans << endl;
				cin.get();
			}
			return ans;
		}
	}
	else {
		(*d_cnter)++;
		if (print_all) {
			cout << "chosen option = deal\n";
			cout << "deal counter = " << *d_cnter << endl;
			cin.get();
		}
		return "3";
	}
}



string strategy_7(Solitaire* game, DRWS* prg, unsigned int weight, unsigned int resistance, unsigned int deal_resistance, unsigned int* d_cnter, bool print_all)
{

	const int from_num = BUILD_STACK_SIZE + SUIT_STACK_SIZE + 1; // 12

	// 可能な行動
	int options_to_suitstack[BUILD_STACK_SIZE + 1];
	int options_to_buildstack[from_num * BUILD_STACK_SIZE];
	vector<tuple<int, char, int>> valid_options;
	int option_from_s = 0, option_to_s = 0, option_others = 0;


	for (int i = 0; i < from_num * BUILD_STACK_SIZE; i++) {
		int from = i / BUILD_STACK_SIZE, to = i % BUILD_STACK_SIZE;
		// 同じ所への移動は許さない
		if (from == to)
			continue;
		// suitstackから
		if (from > 7)
		{
			char from_suit;
			switch (from)
			{
			case 8:
				from_suit = 'h';
				break;
			case 9:
				from_suit = 'd';
				break;
			case 10:
				from_suit = 's';
				break;
			case 11:
				from_suit = 'c';
				break;
			}
			//cout << "from " << from_suit << " to " << to;
			if (game->ValidFromSuitMove(from_suit, to))
			{
				//cout << " is valid!\n";
				options_to_buildstack[i] = 1;
				tuple<int, char, int> t(8, from_suit, to);
				valid_options.push_back(t);
				option_from_s++;
			}

			//cout << "is invalid...\n";
		}
		// Deckから
		else if (from == 7)
		{
			if (game->ValidBuildMove(7, to))
			{
				options_to_buildstack[i] = 1;
				tuple<int, char, int> t(7, '_', to);
				valid_options.push_back(t);
				option_others++;
			}
		}
		// BuildStack間
		else
		{
			if (game->ValidToRowMove(from, to))
			{
				options_to_buildstack[i] = 1;
				tuple<int, char, int> t(from, '_', to);
				valid_options.push_back(t);
				option_others++;
			}
		}
	}
	for (int i = 0; i < BUILD_STACK_SIZE + 1; i++)
	{
		if (game->ValidToSuitMove(i))
		{
			options_to_suitstack[i] = 1;
			tuple<int, char, int> t(i, '_', 8);
			valid_options.push_back(t);
			option_to_s++;
		}
	}


	// valid_option には、(B->B), (T->B), (S->B), (B->S), (T->S)に入っている.

	// 重みづけ
	// (・->S) * W * D_R * R, (S->B) * D_R, (B->B)and(T->B) * D_R * R, (Deal) * R 
	int to_s_w = weight * resistance;
	int from_s_w = 1;
	int others_w = resistance;
	int options_size = option_to_s * to_s_w + option_from_s * from_s_w + option_others * others_w;

	if (print_all)
	{
		cout << "options are:\n";
		cout << "----------------------------------\n";
		for (int i = 0; i < option_from_s + option_to_s + option_others; i++)
		{
			tuple<int, char, int> op = valid_options[i];
			int from = get<0>(op), to = get<2>(op);
			char s = get<1>(op), f, t;
			if (from < 7)
				f = 'B';
			else if (from == 7)
				f = 'T';
			else
				f = 'S';
			if (to < 7)
				t = 'B';
			else
				t = 'S';
			cout << from << " " << s << " " << to << " \t (" << f << " -> " << t << ")\n";
		}
		cout << "deal\n";
		cout << "----------------------------------\n";
	}


	if (options_size != 0)
	{
		*d_cnter = 0;
		int prn = prg->d_uniform(options_size);

		if (print_all)
		{
			cout << "deal counter = " << *d_cnter << endl << endl;
			cout << "uniform distribution on " << options_size << "\n";
			cout << "w=" << weight << ", r=" << resistance << ", d_r=" << deal_resistance << endl;
			cout << "others_w=" << others_w << ", from_s_w=" << from_s_w << ", to_s_w=" << to_s_w << endl;
			cout << "options_size = " << options_size << endl << endl;
			cout << "prn = " << prn;
		}
		if (prn >= options_size)
		{
			if (print_all) {
				cout << ", chosen option = deal\n";
				cin.get();
			}
			return "3";
		}
		int option_num = -1;
		// (B->B), (T->B)
		for (int i = 0; i < option_others; i++)
		{
			option_num++;
			prn -= others_w;
			if (prn < 0)
				break;
		}
		// (S->B)
		if (prn >= 0)
		{
			for (int i = 0; i < option_from_s; i++) {
				option_num++;
				prn -= from_s_w;
				if (prn < 0)
					break;
			}
		}
		// (B->S), (T->S)
		if (prn >= 0)
			option_num += (prn / to_s_w) + 1;

		if (print_all)
			cout << ", option_num is " << option_num << endl;

		tuple<int, char, int> option = valid_options[option_num];

		int from = get<0>(option), to = get<2>(option);
		char s = get<1>(option);
		if (print_all)
		{
			cout << "chosen option = ( " << from << ", " << s << ", " << to << ")\n";
		}
		//suitstackに
		if (to == 8)
		{
			string ans = "2" + to_string(from);
			if (print_all)
			{
				cout << "command is " << ans << endl;
				cin.get();
			}
			return ans;
		}
		// buildstackに		
		else if (s != '_')
		{
			string ans = to_string(18) + s + to_string(to);
			if (print_all)
			{
				cout << "----------------------------------\n";
				cout << "command is " << ans << endl;
				cin.get();
			}
			return ans;
		}
		else {
			string ans = "1" + to_string(from) + to_string(to);
			if (print_all)
			{
				cout << "----------------------------------\n";
				cout << "command is " << ans << endl;
				cin.get();
			}
			return ans;
		}
	}
	else {
		(*d_cnter)++;
		if (print_all) {
			cout << "chosen option = deal\n";
			cout << "deal counter = " << *d_cnter << endl;
			cin.get();
		}
		return "3";
	}
}
