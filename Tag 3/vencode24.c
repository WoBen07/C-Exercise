#include "krypto.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
        char *key;
        int keylen;
        char *text = "INDERVORLESUNGWURDEEINEIDEEVONKASISKIZURERMITTLUNGDERSCHLUESSELLAENGEEINESVIGINERECODESVORGESTELLTEINWICHTIGERTEILDAVONISTDIEBESTIMMUNGDERABSTAENDEZWISCHENSUBSTRINGSFESTERLAENGEIMVORLIEGENDENCODE";
        char *code = malloc(sizeof(char) * strlen(text));
        if (code == NULL)
        {
                printf("Fehler bei der Speicherallokation");
                free(code);
                return 1;
        }
        printf("Bitte Schluessellaenge eingeben:\n");
        keylen = read_len();
        key = malloc(sizeof(char) * keylen);
        if (key == NULL)
        {
                printf("Fehler");
                free(key);
                return 1;
        }
        printf("Bitte Key eingeben:\n");
        read_key_viginere(key, keylen);
        viginere_encode(key, code, text, strlen(text));
        printf("Text: %s Key: %s", code, key);
        return 0;
}