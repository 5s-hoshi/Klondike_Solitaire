#pragma once
#include <string>
#include <vector>
#include "Solitaire.h"
#include "class_random_sampler.h"
using namespace std;


// str2&3
// (E->S) * W * D_R * R, (S->B) * D_R, (B->B)and(T->B) * D_R * R, (Deal) * R
extern string strategy_5(Solitaire* game, DRWS* prg, unsigned int weight, unsigned int resistance, unsigned int deal_resistance, unsigned int *d_cnter, bool print_all);

extern string strategy_6(Solitaire* game, DRWS* prg, unsigned int weight, unsigned int resistance, unsigned int deal_resistance, unsigned int* d_cnter, bool print_all);

extern string strategy_7(Solitaire* game, DRWS* prg, unsigned int weight, unsigned int resistance, unsigned int deal_resistance, unsigned int* d_cnter, bool print_all);
