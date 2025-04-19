#include "krypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
        char *code = "JANISISTTOLL";
        char *key = 'A';
        char *decode = malloc(sizeof(char) * strlen(code));
        if (decode == NULL)
        {
                printf("Fehler bei Speicherallokation");
                return 1;
        }
        
        printf("Bitte gib einen Schlüssel ein:\n");
        key = getchar();
        if (key == NULL || key < 'A' || key > 'Z')
        {
                printf("Ungültige User-eingabe");
                return 1;
        }
        caesar_encode(key, decode, code, strlen(code));
        printf("Text: %s mit Code: %c", decode, key);
        free(decode);
        return 0;
}