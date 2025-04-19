#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "krypto.h"

int main(void)
{
        char *code = "OHGWXFZXEWWTLPBKGBVAMATUXGDTNYXGPBKWBGZXWBXPBKGBVAMUKTNVAXGNFEXNMXGSNBFIHGBXKXGWBXPBKGBVAMFHXZXG";
        int *count = malloc(sizeof(int) * LEN);
        char *text = malloc(sizeof(char) * strlen(code));
        char key, key2;
        if (count == NULL || text == NULL)
        {
                printf("Fehler beim allokieren");
                return 0;
        }
        freq_table(count, code);
        print_table(count);
        printf("Bitte gib einen Buchstaben ein:\n");
        key = getchar();
        key2 = freq_analysis_caesar(key);
        caesar_decode(key2, code, text, strlen(code));
        printf("Decodierter Text mit Key %c:\n", key2);
        printf("%s", text);
        return 0;

}