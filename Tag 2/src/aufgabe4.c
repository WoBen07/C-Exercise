#include "bitmap.h"
#include "bittypes.h"
#include <stdio.h>
#include <stdlib.h>

int halbieren(const bitmapRGB *original, bitmapRGB *ergebnis)
{
             
    int original_height = original->height;
    int original_width = original->width;
    int half_height = original_height / 2;
    int half_width = original_width / 2;
    int i = 0, j = 0;
    int original_row;
        int original_col;

    pixelRGB *o = original->pixel;
    pixelRGB *h = (pixelRGB *) malloc(sizeof(pixelRGB) * half_height * half_width);

    if (h == NULL) {
        return -1;
    }

    ergebnis->height = half_height;
    ergebnis->width = half_width;
    ergebnis->pixel = h;

    for (i = 0; i < half_height; i++) {
        for (j = 0; j < half_width; j++) {
                 original_row = 2 * i;
        original_col = 2 * j;
            h[i * half_width + j].red = (o[original_row * original_width + original_col].red +
                                          o[original_row * original_width + original_col + 1].red +
                                          o[(original_row + 1) * original_width + original_col].red +
                                          o[(original_row + 1) * original_width + original_col + 1].red) / 4;

            h[i * half_width + j].green = (o[original_row * original_width + original_col].green +
                                            o[original_row * original_width + original_col + 1].green +
                                            o[(original_row + 1) * original_width + original_col].green +
                                            o[(original_row + 1) * original_width + original_col + 1].green) / 4;

            h[i * half_width + j].blue = (o[original_row * original_width + original_col].blue +
                                           o[original_row * original_width + original_col + 1].blue +
                                           o[(original_row + 1) * original_width + original_col].blue +
                                           o[(original_row + 1) * original_width + original_col + 1].blue) / 4;
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
        status = halbieren(org, erg);
        if (status != 0)
        {
                printf("Fehler beim Halbieren");
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