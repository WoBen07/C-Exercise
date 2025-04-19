#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

int main(void) {
    sudoku s, solution;
    int field1[] = {2,0,0,0,0,1,0,0,4,0,2,0,0,0,3,0};
    int field2[] = {3,4,1,0,0,2,0,0,0,0,2,0,0,1,4,3};
    int field3[] = {0,3,4,0,4,0,0,2,1,0,0,3,0,2,1,0};
    int field4[] = {3,0,0,6,0,0,0,9,0,
                    0,4,0,0,2,0,0,5,0,
                    0,8,0,0,7,0,1,6,0,
                    9,0,0,3,0,4,7,0,0,
                    0,5,0,0,8,0,0,2,0,
                    0,0,1,9,0,0,0,0,6,
                    0,2,7,0,3,0,0,4,0,
                    0,9,0,0,6,0,0,1,0,
                    0,3,0,0,0,5,0,0,8};
    int field5[] = {0,2,0,0,0,0,0,4,3,
                    0,5,0,3,0,7,6,0,0,
                    0,0,6,0,2,0,0,0,0,
                    0,0,3,0,4,8,0,9,0,
                    0,0,0,0,6,0,0,0,0,
                    0,9,0,1,5,0,2,0,0,
                    0,0,0,0,1,0,3,0,0,
                    0,0,8,5,0,6,0,1,0,
                    7,1,0,0,0,0,0,5,0};
    int empty4[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int empty9[] = {0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,
                    0,0,0,0,6,0,0,0,0,
                    0,0,0,0,0,0,0,0,0};
    if(!sudoku_init(&s, field1, 4) || !sudoku_init(&solution, empty4, 4)) {
        printf("Fehler beim Anlegen des Feldes.\n");
        return 1;
    }
    printf("Sudoku-Feld:\n");
    sudoku_print_field(&s);
    printf("\n");
    printf("Loesung:\n");
    sudoku_solve(&s, 0, &solution);
    sudoku_print_field(&solution);

    printf("\n~~~~~~~~~~~~~~~~~~~~\n\n");

    if(!sudoku_init(&s, field2, 4) || !sudoku_init(&solution, empty4, 4)) {
        printf("Fehler beim Anlegen des Feldes.\n");
        return 1;
    }
    printf("Sudoku-Feld:\n");
    sudoku_print_field(&s);
    printf("\n");
    printf("Loesung:\n");
    sudoku_solve(&s, 0, &solution);
    sudoku_print_field(&solution);

    printf("\n~~~~~~~~~~~~~~~~~~~~\n\n");

    if(!sudoku_init(&s, field3, 4) || !sudoku_init(&solution, empty4, 4)) {
        printf("Fehler beim Anlegen des Feldes.\n");
        return 1;
    }
    printf("Sudoku-Feld:\n");
    sudoku_print_field(&s);
    printf("\n");
    printf("Loesung:\n");
    sudoku_solve(&s, 0, &solution);
    sudoku_print_field(&solution);

    printf("\n~~~~~~~~~~~~~~~~~~~~\n\n");

    if(!sudoku_init(&s, field4, 9) || !sudoku_init(&solution, empty9, 9)) {
        printf("Fehler beim Anlegen des Feldes.\n");
        return 1;
    }
    printf("Sudoku-Feld:\n");
    sudoku_print_field(&s);
    printf("\n");
    printf("Loesung:\n");
    sudoku_solve(&s, 0, &solution);
    sudoku_print_field(&solution);

    printf("\n~~~~~~~~~~~~~~~~~~~~\n\n");

    if(!sudoku_init(&s, field5, 9) || !sudoku_init(&solution, empty9, 9)) {
        printf("Fehler beim Anlegen des Feldes.\n");
        return 1;
    }
    printf("Sudoku-Feld:\n");
    sudoku_print_field(&s);
    printf("\n");
    printf("Loesung:\n");
    sudoku_solve(&s, 0, &solution);
    sudoku_print_field(&solution);

    return 0;
}
