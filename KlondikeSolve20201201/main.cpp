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


#define INF				0xffffffffffffffff

// for game
#define PRINT_SOLITAIRE			true
#define PRINT_NUM				1
#define MANUAL					false
#define MANUAL_NUM				INF
#define DEAL_MAX				16
#define STRATEGY_NUM			7
// #define WEIGHT					1U
// #define DEAL_RESISTANCE			1U
// #define RESISTANCE				1U
#define EPSILON					5U										// percent

int WEIGHT, RESISTANCE, DEAL_RESISTANCE;
#define GRID_SEARCH_VARIABLE	&WEIGHT
#define GRID_VARIABLE_NAME		"weight"
#define GRID2_SEARCH_VARIABLE	&RESISTANCE
#define GRID2_VARIABLE_NAME		"resistance"
#define GRID3_SEARCH_VARIABLE	&DEAL_RESISTANCE
#define GRID3_VARIABLE_NAME		"deal_resistance"

// for debug
#define SHOW_ALL_CARDS			false
#define SHOW_STRATEGY			true
#define WRITE_DETAIL			false

// for data generation
#define WRITE_SEEDS				false
#define	GRID_SEARCH_FIRST		15
#define GRID_SEARCH_END		    15 + 1									// end - 1‚Ü‚Å
#define	GRID2_SEARCH_FIRST		17
#define GRID2_SEARCH_END		17 + 1									// end - 1‚Ü‚Å
#define	GRID3_SEARCH_FIRST		1
#define GRID3_SEARCH_END		1 + 1									// end - 1‚Ü‚Å


// #define WRITE_TO				".txt"
#define PLAY_TIMES				1000
#define KIND_OF_SEEDS			10000



int func1(int e, int t);




int main(int argc, char** argv)
{
	chrono::system_clock::time_point start, end;
	start = chrono::system_clock::now();

	RandomSampler prg_base;
	cin.get();
	cin.get();
	_mkdir("data");
	string dir_name = "./data/";
	string f_name, seeds_f_name;
	bool is_manual;
	unsigned long long won_cnt = 0LL;
	unsigned long app_seeds[5];

	if (WRITE_SEEDS) 
	{
		GetTime(&dir_name);
		cout << dir_name << endl;
		_mkdir(dir_name.c_str());
		f_name = dir_name + "/header.txt";
		seeds_f_name = dir_name + "/seeds.txt";
		ofstream outputfile(f_name);

		unsigned long seed[5];
		prg_base.m90getseeds(seed, seed + 1, seed + 2, seed + 3, seed + 4);
		outputfile << "play times = " + to_string(PLAY_TIMES) + " \t a number of seeds = " + to_string(KIND_OF_SEEDS) << endl;
		outputfile << "w" << GRID_SEARCH_FIRST << "to" << GRID_SEARCH_END - 1 << ", ";
		outputfile << "r" << GRID2_SEARCH_FIRST << "to" << GRID2_SEARCH_END - 1 << ", ";
		outputfile << "dr" << GRID3_SEARCH_FIRST << "to" << GRID3_SEARCH_END - 1 << endl;
		outputfile << "strategy = " << STRATEGY_NUM << ", ";
		/*
		if (STRATEGY_NUM == 5)
		{
			if (GRID_VARIABLE_NAME != "weight")
				outputfile << "w=" << WEIGHT << ", \t ";
			if (GRID_VARIABLE_NAME != "resistance")
				outputfile << "r=" << RESISTANCE << ", \t ";
			if(GRID_VARIABLE_NAME != "deal_resistance")
				outputfile << "d_r=" << DEAL_RESISTANCE;
		}
		*/
		outputfile << endl;
		outputfile << "prg_base's seed: ";
		for (int i = 0; i < 5; i++)
		{
			outputfile << seed[i] << " ";
		}
		outputfile << endl << endl;
		outputfile.close();
	}
	

	

	for (*GRID_SEARCH_VARIABLE = GRID_SEARCH_FIRST; *GRID_SEARCH_VARIABLE < GRID_SEARCH_END; (*GRID_SEARCH_VARIABLE)++)
	{
		for (*GRID2_SEARCH_VARIABLE = GRID2_SEARCH_FIRST; *GRID2_SEARCH_VARIABLE < GRID2_SEARCH_END; (*GRID2_SEARCH_VARIABLE)++)
		{
			for (*GRID3_SEARCH_VARIABLE = GRID3_SEARCH_FIRST; *GRID3_SEARCH_VARIABLE < GRID3_SEARCH_END; (*GRID3_SEARCH_VARIABLE)++) 
			{
				won_cnt = 0;
				////////////////////////////
				//		csv file_name
				////////////////////////////

				if (WRITE_SEEDS) 
				{
					int tmp1 = (GRID_SEARCH_END - 1) / 10, tmp2 = *GRID_SEARCH_VARIABLE / 10;
					int tmp3 = (GRID2_SEARCH_END - 1) / 10, tmp4 = *GRID2_SEARCH_VARIABLE / 10;
					int tmp5 = (GRID3_SEARCH_END - 1) / 10, tmp6 = *GRID3_SEARCH_VARIABLE / 10;
					int zeros = 0;
					while (tmp1 > 0)
					{
						tmp1 /= 10;
						zeros++;
					}
					while (tmp2)
					{
						tmp2 /= 10;
						zeros--;
					}
					f_name = dir_name + "/w=";
					for (int i = 0; i < zeros; i++) {
						f_name += "0";
					}
					f_name += to_string(*GRID_SEARCH_VARIABLE);
					zeros = 0;
					
					while (tmp3 > 0)
					{
						tmp3 /= 10;
						zeros++;
					}
					while (tmp4)
					{
						tmp4 /= 10;
						zeros--;
					}
					f_name += "r=";
					for (int i = 0; i < zeros; i++)
					{
						f_name += "0";
					}
					f_name += to_string(*GRID2_SEARCH_VARIABLE);
					zeros = 0;
					
					while (tmp5 > 0)
					{
						tmp5 /= 10;
						zeros++;
					}
					while (tmp6)
					{
						tmp6 /= 10;
						zeros--;
					}
					f_name += "dr=";
					for (int i = 0; i < zeros; i++) {
						f_name += "0";
					}
					f_name += to_string(*GRID3_SEARCH_VARIABLE);
					f_name += ".csv";
				}

				for (int j = 0; j < 5; j++) {
					app_seeds[j] = prg_base.m90random31();
				}
				DRWS prg_app(app_seeds[0], app_seeds[1], app_seeds[2], app_seeds[3], app_seeds[4]);


				if (WRITE_SEEDS)
				{
					ofstream outputfile(seeds_f_name, ios::app);
					outputfile <<"w=" << *GRID_SEARCH_VARIABLE << "r=" << *GRID2_SEARCH_VARIABLE << "dr=" << *GRID3_SEARCH_VARIABLE;
					for (int j = 0; j < 5; j++)
						outputfile << app_seeds[j] << ",";
					outputfile.close();
				}

				for (int i = 0; i < KIND_OF_SEEDS; i++)
				{
					prg_app.set_first_location();

					if (KIND_OF_SEEDS > 20 && i * 100 % KIND_OF_SEEDS == 0)
					{
						ClearScreen();
						switch (STRATEGY_NUM)
						{
						case 5:
							cout << "weight = " << WEIGHT << ", resistance = " << RESISTANCE << ", deal_resistance = " << DEAL_RESISTANCE << endl;
							break;
						case 6:
							cout << "weight = " << WEIGHT << ", resistance = " << RESISTANCE << ", deal_resistance = " << DEAL_RESISTANCE << endl;
							break;
						case 7:
							cout << "weight = " << WEIGHT << ", resistance = " << RESISTANCE << ", deal_resistance = " << DEAL_RESISTANCE << endl;
							break;
						default:
							cout << "error\n";
							break;
						}
						cout << f_name << endl << endl;
						int percent = i * 100 / KIND_OF_SEEDS;
						cout << "0%       50%       100%\n";
						cout << "|---------+---------|\n";
						for (int j = 0; j < i / (KIND_OF_SEEDS / 20) + 1; j++)
						{
							cout << "#";
						}
						cout << endl;
						cout << percent << "%" << endl;

					}

					Solitaire game = Solitaire(&prg_app);
					if (PRINT_SOLITAIRE)
					{
						game.PrintAllDetails();
						cin.get();
					}
					bool running = true;
					unsigned int d_cnter = 0;
					unsigned long long cnt = 0LL;


					if (WRITE_SEEDS)
					{
						ofstream outputfile(f_name, ios::app);
						outputfile << i << "th,";
						outputfile.close();
					}

					do {
						cnt++;
						is_manual = cnt >= MANUAL_NUM && MANUAL;
						if (0)
						{
							ClearScreen();
							cout << "time " << cnt << endl;
							if (PRINT_SOLITAIRE)
								game.PrintAllDetails(SHOW_ALL_CARDS);
						}
						if (PRINT_SOLITAIRE && cnt % PRINT_NUM == 0)
						{
							cout << endl << endl;
							cout << "1. move to buildstacks\n";
							cout << "2. move to suitstacks\n";
							cout << "3. deal to talon\n";
							cout << "0. quit\n";
							cout << "Enter option:\n";
						}
						string option_s;
						if (cnt >= MANUAL_NUM && MANUAL)
							cin >> option_s;
						else {
							switch (STRATEGY_NUM)
							{
							case 5:
								option_s = strategy_5(&game, &prg_app, WEIGHT, RESISTANCE, DEAL_RESISTANCE, &d_cnter, SHOW_STRATEGY);
								break;
							case 6:
								option_s = strategy_6(&game, &prg_app, WEIGHT, RESISTANCE, DEAL_RESISTANCE, &d_cnter, SHOW_STRATEGY);
								break;
							case 7:
								option_s = strategy_7(&game, &prg_app, WEIGHT, RESISTANCE, DEAL_RESISTANCE, &d_cnter, SHOW_STRATEGY);
								break;
							default:
								cout << "\ninvalid strategy";
								cin.get();
								break;
							}
						}

						char option = ' ';
						// cin >> option;
						option = option_s[0];

						/*
						if (option_s[0] != '3')
							deal_counter = 0;
						*/

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
						else if (cnt >= PLAY_TIMES)
						{
							if (PRINT_SOLITAIRE)
							{
								ClearScreen();
								cout << "\n\n\n\t\t\t GAME OVER \n\n\n\n\n";
								cout << "time over\n";
								cin.get();
							}
							if (WRITE_SEEDS)
							{
								ofstream outputfile(f_name, ios::app);
								outputfile << "," << "time_over";
								outputfile.close();
							}
							running = false;
						}
						else if (d_cnter >= DEAL_MAX)
						{
							if (PRINT_SOLITAIRE)
							{
								ClearScreen();
								cout << "\n\n\n\t\t\t GAME OVER \n\n\n\n\n";
								cout << "16-deal loops\n";
								cin.get();
							}
							if (WRITE_SEEDS)
							{
								ofstream outputfile(f_name, ios::app);
								outputfile << "," << "16-deel_loop";
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
					} while (running);


				}
				if (WRITE_SEEDS)
				{
					ofstream outputfile(seeds_f_name, ios::app);
					outputfile << prg_app.get_locmax() << endl;
					outputfile.close();
				}
				/*
				ofstream outputfile(file_name, ios::app);
				outputfile << "\n\nresult: \t " << won_cnt << " times won.\n";
				outputfile << (double)won_cnt * 100 / KIND_OF_SEEDS << "%\n";
				outputfile.close();
				//*/
			}
		}
	}
	

	ClearScreen();
	cout << f_name << endl << endl;
	cout << "0%       50%       100%\n";
	cout << "|---------+---------|\n";
	for (int i = 0; i < 21; i++)
		cout << "#";
	cout << endl;
	cout << 100 << "%" << endl;
	cout << "done!\n";
	
	end = chrono::system_clock::now();

	double elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	if (WRITE_SEEDS)
	{
		f_name = dir_name + "/header.txt";
		ofstream outputfile(f_name, ios::app);
		outputfile << "elapsed time: " << elapsed << endl;
		outputfile.close();
	}
	
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

int func1(int t, int e)
{
	if (t > PLAY_TIMES / 2)
		return 0;
	else
		return e;
}
