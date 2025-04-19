#include "krypto.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        char *code = "KBFSTJQFNSUIPUYITRGSKBGWFSGJQBMOUWUYKNWFGFOWVHNIPUFSTGEVNIGGUSNZCSPUGSKBGGXWIWPSTSECFSUJQFISUHGZNHGWPKKQJHKUGFVSKZFOXCPWUHFWGPGGVWOAWBIRGFCPUHCSPRGNYWUQJSPGWPUHTWPUUTGGVSTZCSPUGWOJQFNWGUGBFSPQQRG";
        char *text = malloc(sizeof(char) * strlen(code));
        char *key, *realkey;
        int keylen, i, j;
        int *count;
        char *subcode;
        if (text == NULL)
        {
                printf("Fehler");
                free(text);
                return -1;
        }
        printf("Bitte Schluessellaenge eingeben:\n");
        keylen = read_len();
        printf("Keylen: %i\n", keylen);
        key = malloc(sizeof(char) * keylen);
        realkey = malloc(sizeof(char) * keylen);
        subcode = malloc(sizeof(char) * strlen(code));
        count = malloc(sizeof(char) * strlen(subcode));
        if (subcode == NULL || count == NULL || key == NULL || realkey == NULL)
        {
                printf("Fehler");
                free(subcode);
                return -1;
        }
        for (i = 0; i < keylen; i++)
        {
                printf("%i", i);
                
                generate_subcode(code, subcode, keylen, i);
                printf("Tabelle fuer Pos.: %i\n", i);
                freq_table(count, subcode);
                print_table(count);
                printf("Bitte Buchstaben fuer Stelle %i eingeben:\n", i);
                key[i] = getchar();
                flush_stdin();
                realkey[i] = freq_analysis_caesar(key[i]);
        }
        key[i] = '\0';
        realkey[i] = '\0';
        printf("Schleife zueende\n");
        viginere_decode(realkey, code, text, strlen(code));
        printf("Decodierter Text: %s mit Schluessel: %s", text, realkey);
        free(text);
        free(key);
        free(subcode);
        free(count);
        return 0;
        

}