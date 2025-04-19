#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
        FILE *a = fopen("p24a02.txt", "w");
        int i, array[9], array2[9];
        if (a == NULL)
        {
                printf("Fehler beim Öfnen der Datei!");
        }
        for (i = 0; i < 9; i++)
        {
                array[i] = rand() % 100;
                printf("%i\n", array[i]);
        }
        for (i = 0; i < 9; i++)
        {

                if (fprintf(a, "%i\n", array[i]) < 0)
                {
                        printf("fehler beim Schreiben!");
                }
                
        }
        if (fclose(a) == EOF)
        {
                printf("Fehler beim schließen!");
        }
        a = fopen("p24a02.txt", "r");
        if (a == NULL)
        {
                printf("Fehler beim Öfnen der Datei!");
        }
        printf("\n\n\n");
        for (i = 0; i < 9; i++)
        {
                if(fscanf(a, "%i", &array2[i]) < 0){
                        printf("Fehler beim Lesen");
                }
                printf("%i\n", array2[i]);
        }
        if (fclose(a) == EOF)
        {
                printf("Fehler beim schließen!");
        }
        
        return 0;
}