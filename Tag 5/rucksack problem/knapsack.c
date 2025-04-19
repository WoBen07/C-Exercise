#include "knapsack.h"

void rucksack_print_menge(int menge[], int anzahl)
{
        int i;
        for (i = 0; i < anzahl; i++)
        {
                printf("%i\n", menge[i]);
        }
        
}

int rucksack_calc_gewicht(int menge[], struct item items[], int anzahl)
{
        int i;
        int gewicht = 0;
        for (i = 0; i < anzahl; i++)
        {
                if (menge[i] > 0)
                {
                        gewicht += items[i].gewicht;
                }
                
        }
        return gewicht;
}

void rucksack_solve(int menge[], int opt[], struct item items[], int schranke, int ebene, int anzahl)
{
        int i, j, k, h;
        for (i = 0; i < anzahl; i++)
        {
                opt[0] = items[i].gewicht;
                for (j = 1; j < schranke; j++)
                {
                        opt[j] = items[j].gewicht;
                }
                if (c)
                {
                        /* code */
                }
                
                
        }
        
}