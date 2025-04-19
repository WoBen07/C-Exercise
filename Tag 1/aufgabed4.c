#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copy_binary(char *from, char *to)
{
        FILE *fr = fopen(from, "rb");
        FILE *t = fopen(to, "wb");
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
                        printf("Ende der Datei erreicht!\n");
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


int calc_size_file(char *file)
{
        FILE *f = fopen(file, "rb");
        int size = 0;
        if(f == NULL){
                return 0;
        }
        if (fseek(f, 0, SEEK_END) != 0)
        {
                return 0;
        }
        size = ftell(f);
        if (size < 0 )
        {
                return 0;
        }
        if (fclose(f) == EOF)
        {
                return 0;
        }
        
        return size;
        
}


int search(char *file, char *search)
{
        char *array, *se;
        int size;
        FILE *in = fopen(file, "r");
        size = calc_size_file(file);
        array = malloc(sizeof(char) * size);
        if (array == NULL)
        {
                return 1;
        }
        fread(array, sizeof(char), size, in);
        se = strstr(array, search);
        if (se == NULL)
        {
                printf("Wort: %s nicht gefunden!\n", search);
                if (fclose(in) == EOF)
                {
                        printf("Fehler beim schließen!");
                        return 0;
                }
                return 1;
        }else{
                printf("Wort: %s gefunden!\n", search);
                if (fclose(in) == EOF)
                {
                        printf("Fehler beim schließen!");
                        return 0;
                }
                return 0;
        }
        
        
}

int count(char *file, char *word)
{
        char *array, *se;
        int size, count = 0;
        FILE *in = fopen(file, "r");
        size = calc_size_file(file);
        array = malloc(sizeof(char) * size);
        if (array == NULL)
        {
                return 0;
        }
        fread(array, sizeof(char), size, in);
        se = strstr(array, word);
        if (se == NULL)
        {
                return 0;
        }
        
        while (se != NULL)
        {
                count++;
                se = strstr(se + 1, word);
        }
        if (fclose(in) == EOF)
        {
                printf("Fehler beim schließen!");
                return 0;
        }
        return count;
}

int delete_word(char *file, char *word)
{
        char *array, *se;
        int size;
        unsigned int i;
        FILE *in = fopen(file, "r+");
        size = calc_size_file(file);
        array = malloc(sizeof(char) * size);
        if (array == NULL)
        {
                return 1;
        }
        fread(array, sizeof(char), size, in);
        se = strstr(array, word);
        if (se == NULL)
        {
                printf("Das zu löschende Wort existiert nicht!\n");
                if (fclose(in) == EOF)
                {
                        printf("Fehler beim schließen!");
                        return 0;
                }
                return 0;
        }else{
                
                for (i = 0; i < strlen(word); i++)
                {
                        fseek(in, strlen(se), SEEK_SET);
                        fputc('0', in);
                        se++;
                        
                }
                printf("Erstes Vorkommen von %s geloescht\n", word);
                if (fclose(in) == EOF)
                {
                        printf("Fehler beim schließen!");
                        return 0;
                }
                return 1;
        }
}


int main(void)
{
        char *from = "f.bin";
        char *to = "t.bin";
        char *searchword = "Hallo";
        copy_binary(from, to);
        search("in2.txt", searchword);
        printf("Anzahl von %s: %i\n", searchword, count("in2.txt", searchword));
        delete_word("in2.txt", searchword);
        printf("Anzahl von %s: %i\n", searchword, count("in2.txt", searchword));
        return 0;
}