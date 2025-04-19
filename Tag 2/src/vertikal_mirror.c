#include "bitmap.h"
#include <stdlib.h>
#include <stdio.h>

int vertikal_mirror(const bitmapRGB *original, bitmapRGB *drehung)
{       
        int height = original->height, width = original->width, i ,j;
        pixelRGB *o = original->pixel;
        pixelRGB *d;
        d = (pixelRGB *) malloc(sizeof(pixelRGB) * height * width);
        if (d == NULL)
        {
                return -1;
        }
        drehung->height = height;
        drehung->width = width;
        drehung->pixel = d;
        
        for(i = 0; i < width; i++){
                for(j = 0; j < height; j++){
                        d[i * width + j].red = o[(height - i - 1) * width + j].red;
                        d[i * width + j].green = o[(height - i - 1) * width + j].green;
                        d[i * width + j].blue = o[(height - i - 1) * width + j].blue;
                }
        }
        
        return 0;
}

int main(int argc, char **argv)
{
        int status;
        bitmapRGB *org = malloc(sizeof(bitmapRGB));
        bitmapRGB *erg = malloc(sizeof(bitmapRGB));
        if (erg == NULL || org == NULL)
        {
                printf("Fehler bei der Speicherreservierung!");
                free(org);
                free(erg);
                return 1;
        }
        if (argc != 3)
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
        status = vertikal_mirror(org, erg);
        if (status != 0)
        {
                printf("Fehler beim Drehen");
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