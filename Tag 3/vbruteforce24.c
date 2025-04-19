#include "krypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
        int keylen, len_decode;
        char *code = "FWGKKGUSPGEVCTVTCSPUVSKUGBVZKQJSTGVRCOPWPHGFGGUOPHBIYSTRGBYCUWGOWTJCGFV";
        char *text;
        char *supposed_key;
        printf("Bitte die Laenge des Keys eingeben:\n");
        keylen = read_len();
        len_decode = strlen(code);
        printf("Test 1");
        brute_viginere(code, keylen, len_decode);
        supposed_key = malloc(sizeof(char) * keylen);
        text = malloc(sizeof(char) * len_decode);
        if (supposed_key == NULL || text == NULL)
        {
                printf("Fehler beim Allokieren");
                return 1;
        }
        printf("Bitte den vermuteten Schluessel eingeben:\n");
        read_key_viginere(supposed_key, keylen);
        printf("Der Text mit dem vermuteten Key:\n");
        viginere_decode(supposed_key, code, text, len_decode);
        printf("%s", text);
        return 0;

}