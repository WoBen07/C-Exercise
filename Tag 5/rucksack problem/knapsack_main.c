#include <stdio.h>
#include <stdlib.h>

#include "knapsack.h"

int main(int argc, char *argv[])
{
	int i;
	int *menge;
	int *opt;
	struct item *items;
	int schranke;
	int anzahl;

	if (argc < 3) {
		printf("\nNicht genuegend Parameter uebergeben! Benutzung:");
		printf("\n Erster Parameter: Obere Schranke");
		printf("\n Weitere Parameter: Die Gewichte\n");
		exit(1);
	}

	for (i = 1; i < argc; i++) {
		if (atoi(argv[i]) == 0) {
			printf("\nUngueltige Eingabe:");
			printf("\nWerte muessen ganzzahlig und ungleich 0 sein\n");
			exit(1);
		} else if (atoi(argv[i]) < 0) {
			printf("\nUngueltige Eingabe:");
			printf("\nWerte muessen positiv sein sein sein\n");
			exit(1);
		}
	}

	anzahl = argc - 2;
	schranke = atoi(argv[1]);

	menge = calloc(anzahl, sizeof(int));
	if (menge == NULL) {
		printf("Main: Fehler bei Speicherallokation");
		exit(1);
	}
	opt = calloc(anzahl, sizeof(int));
	if (opt == NULL) {
		printf("Main: Fehler bei Speicherallokation");
		free(menge);
		exit(1);
	}
	items = calloc(anzahl, sizeof(struct item));
	if (items == NULL) {
		printf("Main: Fehler bei Speicherallokation");
		free(menge);
		free(opt);
		exit(1);
	}

	for (i = 2; i < argc; i++) {
		items[i - 2].id = i - 2;
		items[i - 2].gewicht = atoi(argv[i]);
	}

	printf("\nGegenstaende:");
	for (i = 0; i < anzahl; i++) {
		printf("\n%i: %i", i, items[i].gewicht);
	}

	printf("\n\n");
	rucksack_solve(menge, opt, items, schranke, 0, anzahl);

	printf("\nOptimale Teilmenge:");
	rucksack_print_menge(opt, anzahl);
	printf("\nGewicht: %i\n", rucksack_calc_gewicht(opt, items, anzahl));

	free(menge);
	free(opt);
	free(items);

	exit(0);
}
