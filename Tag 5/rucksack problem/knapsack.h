#ifndef KNAPSACK_H_INCLUDED
#define KNAPSACK_H_INCLUDED
#include <stdio.h>

struct item
{
	int id;
	int gewicht;
};

void rucksack_solve(int menge[], int opt[], struct item items[], int schranke,
                    int ebene, int anzahl);
int rucksack_calc_gewicht(int menge[], struct item items[], int anzahl);
void rucksack_print_menge(int menge[], int anzahl);

#endif
