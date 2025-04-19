#include "bitmap.h"
#include <stdlib.h>
#include <stdio.h>

int invert(const bitmapRGB *original, bitmapRGB *ergebnis)
{
        int height = original->height, width = original->width, i, j;
        pixelRGB *o = original->pixel;
        pixelRGB *k;
        k = (pixelRGB *) malloc(height * width * sizeof(pixelRGB));
        if (k == NULL)
        {
                return -1;
        }
        ergebnis->height = height;
        ergebnis->width = width;
        ergebnis->pixel = k;
        for (i = 0; i < height; i++)
        {
                for(j = 0; j < width; j++)
                {
                        k->blue = 256 - o->blue;
                        k->red = 256 - o->red;
                        k->green = 256 - o->green;
                        o++;
                        k++;
                }
                
        }
        return 0;
}

int main(int argc, char **argv)
{
        int status;
        bitmapRGB *orig = malloc(sizeof(bitmapRGB));
        bitmapRGB *ergb = malloc(sizeof(bitmapRGB));
        if (argc != 3)
        {
                printf("Ungültige Anzahl an Parametern übergeben!");
                return 1;
        }
        status = loadBitmapRGB(argv[1], orig);
        if (status != BMP_OK)
        {
                printf("Fehler beim Laden der Datei!");
                free(ergb);
                free(orig);
                return 1;
        }else{
                status = invert(orig, ergb);
                if (status != 0)
                {
                        printf("Fehler beim konvertieren!");
                        free(ergb);
                        free(orig);
                        return 1;
                }
                status = saveBitmapRGB(argv[2], ergb);
                if (status != BMP_OK)
                {
                        printf("Fehler beim Speichern der konvertierten Datei");
                        free(ergb);
                        free(orig);
                        return 1;
                }
                printf("Erfolgreich konvertiert!");
                free(ergb);
                free(orig);
                return 0;
        }
        
}