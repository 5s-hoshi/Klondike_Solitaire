#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <regex>
#include <direct.h>
#include "Solitaire.h"
#include "strategy.h"
#include "class_random_sampler.h"
using namespace std;

void HandleMoveToBuildStack(Solitaire& game, string s, bool is_manual);
void HandleMoveToAnswer(Solitaire& game, string s, bool is_manual);
void ClearScreen();
void GetTime(string* s);

// for game
#define PRINT_SOLITAIRE			true
#define PRINT_NUM				1

// for debug
#define SHOW_ALL_CARDS			false
#define SHOW_STRATEGY			true
#define WRITE_DETAIL			false

// for data generation
#define WRITE_SEEDS				false


int main(int argc, char** argv)
{
	chrono::system_clock::time_point start, end;
	start = chrono::system_clock::now();
	DRWS prg_app;


	cin.get();
	cin.get();
	_mkdir("data");
	string dir_name = "./data/";
	string f_name, seeds_f_name;
	// マニュアル操作
	bool is_manual = true;
	unsigned long long won_cnt = 0LL;
	
	if (WRITE_SEEDS) 
	{
		GetTime(&dir_name);
		cout << dir_name << endl;
		_mkdir(dir_name.c_str());
		f_name = dir_name + "/header.txt";
		seeds_f_name = dir_name + "/seeds.txt";
		ofstream outputfile(f_name);

		unsigned long seed[5];
		prg_app.m90getseeds(seed, seed + 1, seed + 2, seed + 3, seed + 4);
		outputfile << "prg seed: ";
		for (int i = 0; i < 5; i++)
		{
			outputfile << seed[i] << " ";
		}
		outputfile.close();
	}
	// 
	prg_app.set_first_location();

	Solitaire game = Solitaire(&prg_app);
	/*
	if (PRINT_SOLITAIRE)
	{
		game.PrintAllDetails();
		cin.get();
	}
	*/
	bool running = true;
	
	unsigned long long cnt = 0;
	do {
		// 画面表示
		if (PRINT_SOLITAIRE)
		{
			ClearScreen();
			cout << "time " << cnt << endl;
			if (PRINT_SOLITAIRE)
				game.PrintAllDetails(SHOW_ALL_CARDS);
		}
		// 行動選択肢表示
		if (PRINT_SOLITAIRE && cnt % PRINT_NUM == 0)
		{
			cout << endl << endl;
			cout << "1. move to buildstacks\n";
			cout << "2. move to suitstacks\n";
			cout << "3. deal to talon\n";
			cout << "0. quit\n";
			cout << "Enter option:\n";
		}

		// 行動選択
		string option_s;
		if (is_manual)
			cin >> option_s;
		
		char option = ' ';
		// cin >> option;
		option = option_s[0];

		// (int)'0' = 48
		switch ((int)option)
		{
		case 48:
			running = false;
			//cin.get();
			break;
		case 49:
			HandleMoveToBuildStack(game, option_s, is_manual);
			//cin.get();
			break;
		case 50:
			HandleMoveToAnswer(game, option_s, is_manual);
			//cin.get();
			break;
		case 51:
			game.Deal(3);
			//deal_counter++;
			break;
		default:
			cout << "\ninvalid option";
			cin.ignore(80, '\n');
			break;
		}
		// PRINT_NUM回ごとに画面表示
		if (PRINT_SOLITAIRE && cnt % PRINT_NUM == 0)
		{
			ClearScreen();
			cout << "time " << cnt << endl;
			game.PrintAllDetails(SHOW_ALL_CARDS);
			cin.get();
		}
		if (game.GameCompleted())
		{
			if (PRINT_SOLITAIRE)
			{
				ClearScreen();
				cout << "\n\n\n\t\t\t CONGRATULATIONS YOU WIN!!!\n\n\n\n\n";
				cout << "time: " << cnt << "\n";
				cin.get();
			}
			won_cnt++;
			if (WRITE_SEEDS)
			{
				ofstream outputfile(f_name, ios::app);
				outputfile << cnt << ",win";
				outputfile.close();
			}

			running = false;
		}
		if (running == false)
		{
			if (WRITE_SEEDS)
			{
				ofstream outputfile(f_name, ios::app);
				outputfile << "," << prg_app.get_locmax() << endl;
				outputfile.close();
			}
		}
		cnt++;
	} while (running);

	return 0;
}

void HandleMoveToBuildStack(Solitaire& game, string s, bool is_manual)
{
	char from = '0', to = '0', suit = '0';
	if (is_manual) 
	{
		cout << endl << endl;
		cout << "From (7 for talon, 8 for suitstacks): ";
		cin >> from;
	}
	else {
		from = s[1];
	}
	if (from == '8')
	{
		if (is_manual) 
		{
			cout << "Suit (h, d, s, c): ";
			cin >> suit;
			cout << "To: ";
			cin >> to;
		}
		else
		{
			suit = s[2];
			to = s[3];
		}

		if (((int)from >= 48 && (int)from <= 56) &&	((int)to >= 48 && (int)to <= 54))
		{
			if (suit == 'h' || suit == 'd' || suit == 's' || suit == 'c')
				game.MakeMoveToRow((int)from - 48, (int)to - 48, suit);
		}
		else
		{
			cout << "invalid input";
			cin.get();
		}
		return;
	}
	if (is_manual) 
	{
		cout << "To: ";
		cin >> to;
	}
	else
		to = s[2];
	
	// (int)'0' = 48, '7' = 55;talon, '8' = 56;suit
	if (((int)from >= 48 && (int)from <= 56) && ((int)to >= 48 && (int)to <= 54))
		game.MakeMoveToRow((int)from - 48, (int)to - 48, suit);
	else
	{
		cout << "invalid input";
		cin.get();
	}

}
void HandleMoveToAnswer(Solitaire& game, string s, bool is_manual)
{
	char from = 0;
	if (is_manual) 
	{
		cout << endl << endl;
		cout << "From (7 for deck): ";
		cin >> from;
	}
	else
		from = s[1];
	
	if ((int)from >= 48 && (int)from <= 55)
		game.MakeToSuitMove((int)from - 48);

	else
	{
		cout << "invalid input";
		cin.get();
	}

}
void ClearScreen()
{
	system("cls");
}
int num_check(char x)
{
	for (int i = 0; i < 10; i++)
	{
		if (x == '0' + i)
		{
			return i;
		}
	}
	return -1;
}
void GetTime(string* s) 
{
	char ch[26];
	auto start = chrono::system_clock::now();
	time_t time = chrono::system_clock::to_time_t(start);
	ctime_s(ch, sizeof ch, &time);
	*s += string(ch);
	// *s += WRITE_TO;
	*s = regex_replace(*s, regex(":"), "'");
	*s = regex_replace(*s, regex(" "), "_");
	*s = regex_replace(*s, regex("\n"), "");
}