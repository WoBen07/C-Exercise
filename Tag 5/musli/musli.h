#ifndef MUSLI
#define MUSLI
#include <stdlib.h>
#include <stdio.h>



typedef struct _strategie {
        int standart;
        int superfrucht;
        int liebe;
}strategie;

void print_strategie(strategie *s);
strategie *init_strategie(int standart, int superfrucht, int liebe);
int calc_gewinn(strategie *s);
void find_opt(void);






#endif