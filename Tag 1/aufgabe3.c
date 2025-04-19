#include <stdio.h>
#include <stdlib.h>

int save_array_bin(int out[], size_t n, FILE *f)
{
        if(fwrite(out, sizeof(int), n, f) < n){
                return 1;
        }
        return 0;
}

int load_array_bin(int in[], size_t n, FILE *f)
{
        if (fread(in, sizeof(int), n, f) < n)
        {
                return 1;
        }
        return 0;
}

int file_copy(char *from, char *to)
{
        FILE *fr = fopen(from, "r");
        FILE *t = fopen(to, "w");
        int i, x = 0;
        if (fr == NULL)
        {
                printf("Fehler beim Öfnnen!");
                return 2;
        }
        if (t == NULL)
        {
                printf("Fehler beim Öfnen!");
                return 2;
        }
        for (i = 0; x != EOF; i++)
        {
                x = fgetc(fr);
                if (x == EOF)
                {
                        printf("Ende der Datei erreicht!");
                        break;
                }
                if(fputc(x, t) == EOF){
                        if (fclose(fr) == EOF)
                        {
                                printf("Fehler beim schließen!");
                        }
                        if (fclose(t) == EOF)
                        {
                                printf("Fehler beim schließen!");
                        }
                        printf("Fehler beim Schreiben!");
                        return 1;
                }
                
                
                
        }
        if (fclose(fr) == EOF)
        {
                printf("Fehler beim schließen!");
        }
        if (fclose(t) == EOF)
        {
                printf("Fehler beim schließen!");
        }
        return 0;
}

int printfile(FILE *in)
{
        int x = 0, i;
        for (i = 0; i < 10; i++)
        {
                if(feof(in) == 1 && i != 10){
                        return 0;
                }
                while(x != '\n'){
                        x = fgetc(in);
                        if (x == EOF && i != 10 && feof(in) != 0)
                        {
                                printf("Fehler beim Einlesen");
                                return -1;
                        }
                        printf("%c", x);
                }
                x = 0;
        }
        return 1;
}

int main(void)
{
        FILE *x = fopen("in2.txt", "r");
        printfile(x);
        
        
        return 0;
}