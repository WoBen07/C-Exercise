#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "mymath.h"

void testAufgabe1() {
    int n, m;
    for(n = 0; n <= 100; n++) {
        for(m = 1; m <= n; m++) {
            assert(iquot(n,m) == floor((double)n/m));
        }
    }
}

void testAufgabe2() {
    int n;
    for(n = 0; n <= 100; n++) {
        assert(isqrt(n) == floor(sqrt(n)));
    }
}

void testAufgabe3() {
    int n, a;
    for(n = 1; n <= 100; n++) {
        for(a = 2; a <= n; a++) {
            assert(ilog(a, n) == floor(log((double)n)/log((double)a)));
        }
    }
}

void testAufgabe4() {
    int i;
    double error = 0.001;
    for (i = 0; i < 5; i++) {
        assert(sin(i) - 1000*error < sinus(i, error) && sin(i) + 1000*error > sinus(i, error));
        
    }
    assert(sinus(3.1415,0.01) > sin(3.1415) - 1 && sinus(3.1415, 0.01 < sin(3.1415) - 1));
}

void testAufgabe5() {
    double error, a;
    int n;
    for(n = 2; n <= 5; n++) {
        for (a = 1; a < 5; a += 0.25) {
            for(error = 1; error >= 0.0000001; error = error/10) {
                assert(fabs(pow(a, 1. / n) - newton_root(n, a, error)) <= error);
            }
        }
    }
}

void testAufgabe6() {
    double error;
    double a;
    for(a = 1; a <= 5; a += 0.25) {
        for(error = 1; error >= 0.0000001; error = error/10) {
            assert(fabs(area(a, a + 1, error) - 2. / 3 * (pow(a+1, 1.5) - pow(a, 1.5))) <= error);
        }
    }
}

int main() {
    testAufgabe1();
    printf("Hurra! Die Implementierung von Aufgabe 1 scheint zu funktionieren.\n");
    testAufgabe2();
    printf("Super! Die Implementierung von Aufgabe 2 scheint auch zu passen.\n");
    testAufgabe3();
    printf("Cool, die Implementierung von Aufgabe 3 passt anscheinend auch.\n");
    testAufgabe4();
    printf("Aufgabe 4 ist erledigt, sehr gut!.\n");
    
    testAufgabe5();
    printf("Fast am Ziel! Die Implementierung von Aufgabe 5 scheint zu funktionieren.\n");
    printf("Jetzt nicht wundern: Die Flaechenberechnung fuer Aufgabe 6 braucht etwas laenger...\n");
    testAufgabe6();
    printf("Wunderbar! Anscheinend ist fuer die Uebungsaufgaben alles korrekt implementiert.\n\n");
    printf("Die Reise muss hier aber noch nicht zu Ende sein: \n");
    printf("Die Klausur-Aufgabe wird aus den freiwilligen Zusatzaufgaben ausgewaehlt!\n");
    
    return 0;
}
