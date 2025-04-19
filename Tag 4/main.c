#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "mymath.h"

void testAufgabe1() {
    int n, m;
    for(n = 0; n <= 100; n++) {
        for(m = 1; m <= n; m++) {
            assert(iquot_wof(n,m) == floor((double)n/m));
        }
    }
}

void testAufgabe2() {
    int n;
    for(n = 0; n <= 100; n++) {
        assert(isqrt(n) == floor(sqrt(n)));
    }
}
void testAufgabe4() {
    int i;
    double error = 0.001;
    for (i = 0; i < 5; i++) {
        assert(cos(i) - 1000*error < cosinus(i, error) && cos(i) + 1000*error > cosinus(i, error));
        
    }
    assert(cosinus(3.1415,0.01) > cos(3.1415) - 1 && cosinus(3.1415, 0.01 < cos(3.1415) - 1));
}
int main(void)
{
    testAufgabe1();
    printf("Hurra! Die Implementierung von Aufgabe 1 scheint zu funktionieren.\n");
    testAufgabe2();
    printf("Super! Die Implementierung von Aufgabe 2 scheint auch zu passen.\n");
    printf("%f\n", calculate_e(0.000001));
    printf("%f\n", calculate_pi(0.00001));
    testAufgabe4();
    printf("Aufgabe 4 ist erledigt, sehr gut!.\n");
    return 0;
}