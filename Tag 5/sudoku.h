#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

typedef struct sudoku_ {
    int size;
    int* field;
} sudoku;

int sudoku_init(sudoku *s, int field[], int size);
int sudoku_check_field(int field[], int size);
int sudoku_set_field(sudoku *s, int field[], int size);
void sudoku_print_field(sudoku *s);

int sudoku_is_legal(sudoku *field);
void sudoku_solve(sudoku *s, int current_pos, sudoku *solution);

#endif
