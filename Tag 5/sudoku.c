#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

int sudoku_init(sudoku *s, int field[], int size) {
    return sudoku_set_field(s, field, size);
}

int isqrt(int n) {
    int u = 0;
    while((u+1) * (u+1) <= n) {
        u++;
    }
    return u;
}

int sudoku_check_field(int field[], int size) {
    int i;
    if(size <= 0 || isqrt(size)*isqrt(size) != size) return 0;
    for(i = 0; i < size*size; i++) {
        if(field[i] < 0 || field[i] > size) return 0;
    }
    return 1;
}

int sudoku_set_field(sudoku *s, int field[], int size) {
    int i;
    int *f;
    if(!sudoku_check_field(field, size)) return 0;
    s -> size = size;
    f = malloc(size*size*sizeof(int));
    if(f == NULL) return 0;
    for(i = 0; i < size*size; i++) {
        f[i] = field[i];
    }
    s -> field = f;
    return 1;
}

void sudoku_print_field(sudoku *s) {
    int i, j;
    for(j = 0; j < s -> size; j++) {
        printf("----");
    }
    for(j = 0; j < isqrt(s -> size)-1; j++) {
        printf("-");
    }
    printf("-\n");
    for(i = 0; i < s -> size; i++) {
        printf("| ");
        for(j = 0; j < s -> size; j++) {
            if(s -> field[i * (s -> size) + j] == 0) printf("  |");
            else printf("%i |", s -> field[i * (s -> size) + j]);
            if((j+1) % isqrt(s -> size) == 0 && j != s -> size - 1) printf("|");
            printf(" ");
        }
        printf("\n");
        if((i+1) % isqrt(s -> size) == 0 && i != s -> size - 1) {
            for(j = 0; j < s -> size; j++) {
                printf("====");
            }
            for(j = 0; j < isqrt(s -> size)-1; j++) {
                printf("=");
            }
            printf("=\n");
        } else {
            for(j = 0; j < s -> size; j++) {
                printf("----");
            }
            for(j = 0; j < isqrt(s -> size)-1; j++) {
                printf("-");
            }
            printf("-\n");
        }
    }
}

int sudoku_smallfields_legal(sudoku *s) {
    int i, j, idiv, jdiv;
    int current;
    int size = s -> size;
    int square = isqrt(size);
    int *seen = malloc(size * sizeof(int));
    if(seen == NULL) return -1;
    for(idiv = 0; idiv < square; idiv++) {
        for(jdiv = 0; jdiv < square; jdiv++) {
            for(i = 0; i < size; i++) {
                seen[i] = 0;
            }
            for(i = 0; i < size; i++) {
                for(j = 0; j < size; j++) {
                    if(i/square == idiv && j/square == jdiv) {
                        current = s -> field[i * size +j];
                        if(current != 0 && seen[current-1] == 0) {
                            seen[current-1] = 1;
                        } else if(current != 0 && seen[current-1] != 0) {
                            free(seen);
                            return 0;
                        }
                    }
                }
            }
        }
    }
    free(seen);
    return 1;
}
