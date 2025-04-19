#include "bitmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int aendereHelligkeit(const bitmapRGB *original, bitmapRGB *ergebnis, double h)
{
        int height = original->height, width = original->width, i, j;
        double newVal = 0;
        pixelRGB *o = original->pixel;
        pixelRGB *he;
        he = (pixelRGB *) malloc(sizeof(pixelRGB) * height * width);
        if (he == NULL)
        {
                return -1;
        }
        ergebnis ->height = height;
        ergebnis ->width = width;
        ergebnis ->pixel = he;
        for (i = 0; i < height; i++)
        {
                for(j = 0; j < width; j++){
                        newVal = o->red + h;
                        if (newVal > 255)
                        {
                                newVal = 254;
                        }
                        he->red = newVal;
                        newVal = o->blue + h;
                        if (newVal > 255)
                        {
                                newVal = 254;
                        }
                        he->blue = newVal;
                        newVal = o->green + h;
                        if (newVal > 255)
                        {
                                newVal = 254;
                        }
                        he->green = newVal;
                        he += 1;
                        o += 1;
                }
        }
        
        
        return 0;
}

int main(int argc, char **argv)
{
        int status;
        double helligkeit = atof(argv[3]);
        bitmapRGB *org = malloc(sizeof(bitmapRGB));
        bitmapRGB *erg = malloc(sizeof(bitmapRGB));
        if (erg == NULL || org == NULL)
        {
                printf("Fehler bei der Speicherreservierung!");
                free(org);
                free(erg);
                return 1;
        }
        if (argc != 4)
        {
                printf("Ungültige Anzahl an Parametern!");
                free(org);
                free(erg);
                return 1;
        }
        status = loadBitmapRGB(argv[1], org);
        if (status != BMP_OK)
        {
                printf("Fehler beim laden der Bitmap");
                free(org);
                free(erg);
                return 1;
        }
        status = aendereHelligkeit(org, erg, helligkeit);
        if (status != 0)
        {
                printf("Fehler beim aenderdn der Helligkeit");
                free(org);
                free(erg);
                return 1;
        }
        status = saveBitmapRGB(argv[2], erg);
        if (status != BMP_OK)
        {
                printf("Fehler beim laden der Bitmap");
                free(org);
                free(erg);
                return 1;
        }
        printf("Erfolg");
        return 0;
}