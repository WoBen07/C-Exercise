#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
        int i, x;
        FILE *a = fopen("p24a01.txt", "w");
        srand(time(NULL));
        if (a == NULL)
        {
                printf("Fehler beim öffnen der Datei!");
        }
        for (i = 0; i < 100; i++)
        {
                x = (rand() % 100);
                if (fprintf(a, "%i", x) < 0)
                {
                        printf("Fehler beim Schreiben!");
                }
                
        }
        if (fclose(a) == EOF)
        {
                printf("Fehler beim schließen!");
        }
        a = fopen("p24a01.txt", "a");
        if (a == NULL)
        {
                printf("Fehler beim Schreibvorgang");
        }
        if (fputc('\n', a) == EOF)
        {
                printf("Fehler beim Schreiben von \\n");
        }
        if (fputc('X', a) == EOF)
        {
                printf("Fehler beim Schreiben von X");
        }
        if (fclose(a) == EOF)
        {
                printf("Fehler beim schließen!");
        }
        a = fopen("p24a01.txt", "r");
        if (a == NULL)
        {
                printf("Fehler beim öffnen der Datei!");
        }
        for (i = 0; x != EOF; i++)
        {
                x = fgetc(a);
                if (x == EOF)
                {
                        printf("\nEnd of File erreicht!");
                        break;
                }
                printf("\n%i", x);
        }
        if (fclose(a) == EOF)
        {
                printf("Fehler beim Schließen!");
        }
        
        
        return 0;
}