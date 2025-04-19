#include "krypto.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        char *code = "OHGWXFZXEWWTLPBKGBVAMATUXGDTNYXGPBKWBGZXWBXPBKGBVAMUKTNVAXGNFEXNMXGSNBFIHGBXKXGWBXPBKGBVAMFHXZXG";
        int len_decode;
        char key;
        char *text = malloc(sizeof(char) * strlen(code));
        if (text == NULL)
        {
                free(text);
                return 1;
        }
        
        printf("strlen(code): %i\n", strlen(code));
        printf("Bitte Decodielaenge eingeben:\n");
        len_decode = read_len();
        brute_caesar(code, len_decode);
        printf("Bitte Key eingeben:\n");
        key = getchar();
        flush_stdin();
        caesar_decode(key, code, text, len_decode);
        printf("%s mit %c", text, key);
        free(code);
        free(text);
        return 0;
        
}