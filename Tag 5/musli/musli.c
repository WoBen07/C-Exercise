#include "musli.h"
int strategie_opt = 0;

void print_strategie(strategie *s)
{
        printf("Standartmuesli: %i\nSuperfrucht-Muesli: %i\nLiebes-Muesli: %i\n", s->standart, s->superfrucht, s->liebe);
}

strategie *init_strategie(int standart, int superfrucht, int liebe)
{
        strategie *s;
        s = malloc(sizeof(strategie));
        if (s == NULL)
        {
                printf("Fehler beim Speicherallokieren");
                free(s);
                return NULL;
        }
        s->standart = standart;
        s->superfrucht = superfrucht;
        s->liebe = liebe;
        return s;
}

int calc_gewinn(strategie *s)
{
        int gewinn = 6*s->standart + 8*s->superfrucht + 10*s->liebe;
        return gewinn;
}

int check_opt_amount(strategie *s)
{
        if (s->standart * 0.8 + s->superfrucht * 0.8 + s->liebe * 0.8 > 80)
        {
                return 0;
        }
        if (s->standart * 0.2 + s->superfrucht * 0.2 + s->liebe * 0.1 > 16)
        {
                return 0;
        }
        if (s->standart * 0.0 + s->superfrucht * 0.2 + s->liebe * 0.3 > 7)
        {
                return 0;
        }
        
        
        return 1;
}



void find_opt(void)
{
        int gewinn = 0, i, j, h;
        strategie *s;
        s = malloc(sizeof(strategie));
        if (s == NULL)
        {
                printf("Fehler bei der Allokation");
                free(s);
                exit(1);
        }
        for (i = 0; i <= 100; i++)
        {
                for (j = 0; j <= 70; j++)
                {
                        for (h = 0; h< 50; h++)
                        {
                                
                                s = init_strategie(i, j, h);
                                if(check_opt_amount(s)){
                                        gewinn = calc_gewinn(s);
                                        if (gewinn > strategie_opt)
                                        {
                                                strategie_opt = gewinn;
                                                print_strategie(s);
                                        }
                                }
                        }
                        
                }
                
        }

}

int main(void)
{
        find_opt();
        printf("Bester Gewinn: %i", strategie_opt);
        return 0;
}