#ifndef TSP_H_INCLUDED
#define TSP_H_INCLUDED
#include <limits.h>

void tsp_solve(int perm[], int opt[], int dist[], int ebene, int len);
int tsp_distanz(int perm[], int dist[], int len);

#endif
