#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "krypto.h"

/* (7 Minuten)
Decodiert einen vorgegebenen Code mit einem vom Benutzer eingegebenen Viginere-Schluessel und gibt den decodierten Text auf Kommandozeile aus.
*/
int main(void)
{
	char *vkey;
	int keylen;
	char *decode;
	char *code = "IVGZKEEFBIJDNVKBGG";

	printf("Decodieren mit Viginere:\n");
	printf("Code:\n\n");
	printf("%s\n\n", code);

	printf("Bitte geben Sie ein, wie lang der Schluessel sein soll: ");
	keylen = read_len();

	vkey = malloc(sizeof(char) * (keylen + 1));
	if (vkey == NULL) {
		fprintf(stderr, "viginere: malloc failed");
		exit(1);
	}

	printf("\nSchluessel mit %i Zeichen aus lat. Grossbuchstaben eingeben (oder beliebige andere Taste fuer Programmabbruch): ", keylen);
	read_key_viginere(vkey, keylen);

	decode = malloc(sizeof(char) * (strlen(code) + 1));
	if (decode == NULL) {
		fprintf(stderr, "viginere: malloc failed");
		free(vkey);
		exit(1);
	}

	viginere_decode(vkey, code, decode, strlen(code));
	printf("Test Decode zu Schluessel %s:\n\n", vkey);
	printf("%s\n\n", decode);

	free(decode);
	free(vkey);

	return 0;
}
