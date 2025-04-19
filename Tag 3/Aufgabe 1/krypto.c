#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "krypto.h"

void flush_stdin(void)
{
	while (getchar() != '\n');
}

/* Einlesefunktionen */

/* (3 Punkte)
Liest von Kommandozeile einen lateinischen Grossbuchstaben c ein und leert danach den Eingabepuffer.
Stellt mittels Zusicherungen die Gueltigkeit der Benutzereingabe sicher.
Liefert c.
*/
int read_letter() {
	int c;
	c = getchar();
	while (getchar() != '\n');
	assert(c >= 'A' && c <= 'Z');
	return c;
}

/* (5 Punkte)
Liest von Kommandozeile keylen Zeichen aus lateinischen Grossbuchstaben ein und speichert diese in key.
Leert danach den Eingabepuffer.
Stellt mittels Zusicherungen die Gueltigkeit der Benutzereingabe sicher.
*/
void read_key_viginere(char *key, int keylen) {
	/* zu implementieren */
	int i;
	char c;
	for (i = 0; i < keylen; i++)
	{
		c = getchar();
		if (c == '\n')
		{
			printf("Zu wenig Zeichen");
			flush_stdin();
			break;
		}
		assert(c >= 'A' && c <= 'Z');
		key[i] = c;
	}
	key[i] = '\0';
	flush_stdin();
}

/* (5 Punkte)
Liest von Kommandozeile eine positive ganze Zahl len ein.
Wiederholt das Einlesen, solange Benutzereingaben ungueltig sind.
Liefert len.
*/
int read_len() {
	int len;
	while (scanf("%i", &len) != 1 || getchar() != '\n' || len <= 0) {
		printf("Ungueltige Eingabe - neuer Versuch: ");
		while (getchar() != '\n');
	}
	assert(len > 0);
	return len;
}

/* Hilfsfunktionen */

/* (3 Punkte)
Liefert 1, falls text nur lateinische Grossbuchstaben enthaelt.
Liefert sonst 0.
*/
int is_text_upper_case(const char *text) {
	assert(text != NULL);

	int i = 0;
	while (text[i] != '\0') {
		if (text[i] > 'Z' || text[i] < 'A')
			return 0;
		++i;
	}
	return 1;
}

/* (5 Punkte)
Schreibt ab der Position keypos alle Zeichen aus code mit dem Abstand keylen in subcode
*/
void generate_subcode(const char *code, char *subcode, int keylen, int keypos) {
	int i;
    
    for (i = 0; code[keypos + i] != '\0'; i++) {
        
        subcode[i] = code[i * keylen + keypos];
    }
    
    subcode[i] = '\0';
}

/* (5 Punkte)
Liefert den groessten gemeinsamen Teiler von a und b.
*/
int ggT(int a, int b) {
	assert(a > 0 && b > 0);

	int h;
    do {
        h = a % b;
        a = b;
        b = h;
    } while (b != 0);

    return a;
}

/* (3 Punkte)
Liefert 0, wenn ein Eintrag in array nicht positiv ist.
Liefert sonst 1.
*/
int is_array_positive(int array[], int len) {
	assert(array != NULL);
	assert(len > 0);

	int i = 0;
	while (i < len) {
		if (array[i] <= 0)
			return 0;
		++i;
	}

	return 1;
}

/* (5 Punkte)
Liefert den groessten gemeinsamen Teiler von array[0],...,array[len-1]
*/
int ggT_array(int array[], int len) {
	/* zu implementieren */
	int current_ggt;
	int i, j = 0;
	if(len == 1) return array[0];
	current_ggt = ggT(array[0], array[1]);
	for (i = 2; i < len - 1; i++)
	{
		current_ggt = ggT(current_ggt, array[i]);
	}
	
	return current_ggt;
}

/* Viginere-Schluessel generieren */

/* (3 Punkte)
Initialisiert key mit keylen 'A'-Zeichen.
Nuetzlich fuer die Funktion brute-viginere.
*/
void init_key(char *key, int keylen) {
	/* zu implementieren */
	int i;
	for (i = 0; i < keylen; i++)
	{
		key[i] = 'A';
	}
	printf("Test 5");
	key[i] = '\0';
	printf("Test 6");
}

/* (3 Punkte)
Liefert 1, wenn key nur aus 'Z'-Zeichen besteht.
Liefert sonst 0.
Nuetzlich fuer die Funktion brute-viginere.
*/
int is_end_key(char *key) {
	/* zu implementieren */
	int status = 1;
	int i;
	for (i = 0; key[i] != '\0'; i++)
	{
		if (key[i] != 'Z')
		{
			status = 0;
		}
		
	}
	
	return status;
}

/* (5 Punkte)
Erhoeht key um einen Buchstaben in der alphabetischen Reihenfolge.
Stellt ueber Zusicherungen sicher, dass key nicht nur aus 'Z'-Zeichen besteht.
Beispiele:
- increment_key("AAA") == "AAB"
- increment_key("AAZ") == "ABA"
- increment_key("AZM") == "AZN"
- increment_key("XZZ") == "YAA"
Nuetzlich fuer die Funktion brute-viginere.
*/
void increment_key(char *key) {
	assert(key != NULL);
	assert(is_text_upper_case(key));
	assert(!is_end_key(key));

	int i = strlen(key) - 1;
	while (key[i] == 'Z') {
		key[i] = 'A';
		--i;
	}
	++key[i];
	assert(is_text_upper_case(key));
}

/* Verschluesseln */

/* (6 Punkte)
Verschluesselt die ersten len Zeichen von text mit dem Schluessel key und schreibt diese in code.
*/
void caesar_encode(char key, char *code, const char *text, int len) {
	int i;
	for (i = 0; i < len; i++)
	{
		code[i] = ((text[i] + key + 26) % 26) + 'A';
	}
	code[i] = '\0';
}

/* (8 Punkte)
Verschluesselt die ersten len Zeichen von text mit dem Schluessel key und schreibt diese in code.
*/
void viginere_encode(const char *key, char *code, const char *text, int len) {
	int i;
	int keylen = strlen(key);
	for (i = 0; i < len; i++)
	{
		code[i] = ((text[i] + key[i % keylen] +26) % 26) + 'A';
	}
	code[i] = '\0';
}

/* Entschuesseln */

/* (6 Punkte)
Entschluesselt die ersten len Zeichen von code mit dem Schluessel key und schreibt diese in text.
*/
void caesar_decode(char key, const char *code, char *text, int len) {
	/* zu implementieren */
	int i;
	for (i = 0; i < len; i++)
	{
		text[i] = ceasar_decode_char(code[i], key);
	}
	text[i] = '\0';

}

int ceasar_decode_char(char c, char key)
{
	int key1 = key, chr = c, res;
	res = ((chr - key1 + 26) % 26) + 'A';
	return res;
	
}

/* (8 Punkte)
Entschluesselt die ersten len Zeichen von code mit dem Schluessel key und schreibt diese in text.
*/
void viginere_decode(const char *key, const char *code, char *text, int len) {
	/* zu implementieren */
	int keylen, i;
	keylen = strlen(key);
	for (i = 0; i < len; i++)
	{
		text[i] = ceasar_decode_char(code[i], key[i % keylen]);
	}
	text[i] = '\0';
}

/* Brute-Force-Attacke */

/* (6 Punkte)
Entschluesselt fuer alle moeglichen Schluessel die ersten len_decode Zeichen von code.
Gibt diese in der Form "<key>: <text>" jeweils in einer neuen Zeile auf Kommandozeile aus.
Bricht bei Fehlern mit Fehlernachricht auf Kommandozeile ab.
*/
void brute_caesar(const char *code, int len_decode) {
	int i;
	char key = 'A';
	char *text = malloc(sizeof(char) * strlen(code));
	if (text == NULL)
	{
		printf("Fehler");
		free(text);
		exit;
	}
	
	for (i = 0; i < len_decode; i++)
	{
		caesar_decode(key, code, text, strlen(code));
		printf("%c : %s\n", key, text);
		(key)++;
		if ((key) == ('Z' + 1))
		{
			(key) = 'A';
		}
		
	}
	
}

/* (8 Punkte)
Entschluesselt fuer alle moeglichen Schluessel der Laenge keylen die ersten len_decode Zeichen von code.
Gibt diese in der Form "<key>: <text>" jeweils in einer neuen Zeile auf Kommandozeile aus.
Bricht bei Fehlern mit Fehlernachricht auf Kommandozeile ab.
*/
void brute_viginere(const char *code, int keylen, int len_decode) {
	/* zu implementieren */
	char *decode_key = malloc(sizeof(char) * keylen);
	char *text = malloc(sizeof(char) * len_decode + 1);
	int i;
	if (decode_key == NULL || text == NULL)
	{
		printf("Fehler beim allokieren");
		exit(1);
	}
	printf("Test 3");
	text[len_decode] = '\0';
	printf("Test 4");
	init_key(decode_key, keylen);
	printf("Test 2");
	while(is_end_key(decode_key) != 1)
	{
		viginere_decode(decode_key, code, text, len_decode);
		printf("%s: %s\n", decode_key, text);
		increment_key(decode_key);
	}
	viginere_decode(decode_key, code, text, len_decode);
	printf("%s: %s\n", decode_key, text);
	free(decode_key);
	free(text);
	
}

/* Haeufigkeitsanalyse */

/* (6 Punkte)
Berechnet die Haeufigkeiten der Buchstaben in code und speichert diese in count.
Die Anzahl des i-ten Zeichens des lateinischen Alphabets in code wird in count[i-1] gespeichert.
*/
void freq_table(int count[], const char *code) {
	/* zu implementieren */
	int count_letter;
	int letter = 'A' - 1;
	int i, j;
	for (i = 0; i < 26; i++)
	{
		count_letter = 0;
		letter += 1;
		for (j = 0; j < strlen(code); j++)
		{
			if (code[j] == letter)
			{
				count_letter++;
			}
			
		}
		count[i] = count_letter;
	}
	
}

/* (3 Punkte)
Gibt auf Kommandozeile zeilenweise die Eintraege in der Haeufigkeitstabelle in der Form '<Buchstabe>: <Haeufigkeit>' aus.
*/
void print_table(int count[]) {
	int j = 0;
	while (j < LEN) {
		printf("%c: %i\n", 'A' + j, count[j]);
		++j;
	}
}

/* (5 Punkte)
Berechnet die Verschiebung zwischen 'E' und max.
Liefert den zu dieser Verschiebung gehoerenden Schuessel.
*/
char freq_analysis_caesar(char max) {
	/* zu implementieren */
	char offset = (max - 'E') + 'A';
	if (offset < 'A')
	{
		offset += LEN;
	}
	return offset;
}

/* (12 Punkte)
Berechnet die Anzahlen der Distanzen zwischen Substrings der Laenge sublen in code.
Wenn ein Substring mehr als zweimal auftritt, werden hierbei nur die Abstaende zwischen den direkt aufeinanderfolgenden Auftreten beruecksichtigt.
Speichert die Anzahl der Distanz x in distances[x].
Liefert die Anzahl der verschiedenen gefundenen Distanzen.
*/
int find_distances(const char *code, int distances[], int sublen) {
	/* zu implementieren */
	int uniq_dist = 0;
	char *sub = malloc(sizeof(char) * sublen +1);
	char *substr = NULL;
	int offset;
	if(sub == NULL){
		return 0;
	}
	sub[sublen] = '\0';
	while (*(code + sublen))
	{
		strncpy(sub, code, sublen);
		substr = strstr(code + sublen, sub);
		if (!substr)
		{
			++code;
			continue;
		}
		offset = (int)(substr - code);
		if (distances[offset] == 0)
		{
			++uniq_dist;
		}
		++distances[offset];
		++code;
		
	}
	free(substr);
	return uniq_dist;
}

/* (5 Punkte)
Speichert die verschiedenen gefundenen Distanzen hintereinander in collect.
Gibt auf Kommandozeile alle verschiedenen gefundenen Distanzen (getrennt durch Leerzeichen) aus.
*/
void collect_distances(int distances[], int len, int collect[]) {
	assert(distances != NULL);
	assert(collect != NULL);
	assert(len > 0);

	int j = 0;
	int i = 0;
	while (i < len) {
		if (distances[i] > 0) {
			collect[j] = i;
			printf("%i ", i);
			++j;
		}
		++i;
	}
}

