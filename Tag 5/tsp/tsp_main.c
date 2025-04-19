#include <stdio.h>
#include <stdlib.h>

#include "tsp.h"

int main(int argc, char *argv[])
{
	int i;
	int j;
	int k;
	int len;
	int *dist;
	int *opt;
	int *perm;

	if (argc < 3) {
		printf("\nNicht genuegend Parameter! Benutzung:");
		printf("\n 1. Parameter: Anzahl der Staedte n");
		printf("\n Weitere Parameter: ");
		printf("Wechselseitige Entfernungen in der Form");
		printf("\n Distanz(1,2),...,Distanz(1,n),Distanz(2,3)");
		printf(",...,Distanz(2,n),...,Distanz(n-1,n)");
		printf("\n");
		exit(1);
	}

	for (i = 1; i < argc; i++) {
		if (atoi(argv[i]) == 0) {
			printf("\nUngueltige Eingabe:\nWerte muessen ganzzahlig");
			printf(" und ungleich 0 sein");
			printf("\n");
			exit(1);
		} else if (atoi(argv[i]) < 0) {
			printf("\nUngueltige Eingabe:\nWerte muessen positiv sein");
			printf("\n");
			exit(1);
		}
	}

	len = atoi(argv[1]);

	if (len < 3) {
		printf("\nAnzahl der Staedte muss mindestens 3 sein");
		printf("\n");
		exit(1);
	}

	if (len * (len - 1) / 2 != argc - 2) {
		printf("\nAnzahl der wechselseitigen Entfernungen passt nicht zur Anzahl der Staedte");
		printf("\n");
		exit(1);
	}

	dist = calloc(len * len, sizeof(int));
	if (dist == NULL) {
		printf("\nFehler bei Speicherallokation");
		printf("\n");
		exit(1);
	}
	opt = calloc(len, sizeof(int));
	if (opt == NULL) {
		printf("\nFehler bei Speicherallokation");
		printf("\n");
		free(dist);
		exit(1);
	}
	perm = calloc(len, sizeof(int));
	if (perm == NULL) {
		printf("\nFehler bei Speicherallokation");
		printf("\n");
		free(dist);
		free(opt);
		exit(1);
	}

	k = 0;

	for (i = 0; i < len; i++) {
		perm[i] = -1;
		opt[i] = i;
		dist[i * len + i] = 0;

		for (j = i + 1; j < len; j++) {
			dist[i * len + j] = atoi(argv[2 + k]);
			dist[j * len + i] = dist[i * len + j];
			k++;
		}
	}

	printf("\nDistanzen: \n");
	for (i = 0; i < len; i++) {
		for (j = 0; j < len; j++) {
			printf("%3i", dist[i * len + j]);
		}
		printf("\n");
	}

	printf("\n Start: (");
	for (i = 0; i < len-1; i++) {
		printf("%i, ", perm[i]);
	}
	printf("%i)", perm[i]);

	tsp_solve(perm, opt, dist, 0, len);

	printf("\n Loesung: (");
	for (i = 0; i < len-1; i++) {
		printf("%i, ", opt[i]);
	}
	printf("%i)", opt[i]);
	printf("\n Minimum: %i\n", tsp_distanz(opt, dist, len));
	printf("\n");

	free(dist);
	free(opt);
	free(perm);

	exit(0);
}
