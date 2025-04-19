#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "krypto.h"

/* (7 Punkte)
Kasiski-Attacke auf einen mit Viginere verschluesselten Text: 
- Der Benutzer gibt die Laenge der Substrings ein, zwischen denen die Distanzen im Code berechnet werden sollen
- Dann werden diese Distanzen berechnet und deren ggT als vermutete Schluessellaenge auf Kommandozeile ausgegeben
*/
int main(void)
{
	int sublen;
	int *distances;
	int *collect;
	int distance;
	int number;
	char *code2 = "FWGKKGUSPGEVCTVTCSPUVSKUGBVZKQJSTGVRCOPWPHGFGGUOPHBIYSTRGBYCUWGOWTJCGFV";/*keylen: 2, key: CO */
	char *code = "KBFSTJQFNSUIPUYITRGSKBGWFSGJQBMOUWUYKNWFGFOWVHNIPUFSTGEVNIGGUSNZCSPUGSKBGGXWIWPSTSECFSUJQFISUHGZNHGWPKKQJHKUGFVSKZFOXCPWUHFWGPGGVWOAWBIRGFCPUHCSPRGNYWUQJSPGWPUHTWPUUTGGVSTZCSPUGWOJQFNWGUGBFSPQQRG";/*keylen: 2, key: CO */
	
	printf("Kasiski-Attacke auf einen mit Viginere verschluesselten Text (Schuessellaenge berechnen):\n");
	printf("Code:\n\n");
	printf("%s\n\n", code);

	printf("Bitte geben Sie ein, wie lang die Substrings zur Berechnung der Distanzen sein sollen: ");
	sublen = read_len();

	distances = malloc(sizeof(int) * (strlen(code) - sublen));
	if (distances == NULL) {
		fprintf(stderr, "kasiski: malloc failed");
		exit(1);
	}
	number = find_distances(code, distances, sublen);
	if (number == 0) {
		printf("Keine Distanzen gefunden:\n");
		exit(1);
	}

	collect = malloc(sizeof(int) * number);
	if (collect == NULL) {
		fprintf(stderr, "kasiski: malloc failed");
		exit(1);
	}
	printf("Folgende Distanzen wurden gefunden:\n");
	collect_distances(distances, strlen(code) - sublen, collect);

	distance = ggT_array(collect, number);
	printf("\nDie Distanzen haben folgenden ggT = vermutete Schluessellaenge: %i\n", distance);

	return 0;
}
