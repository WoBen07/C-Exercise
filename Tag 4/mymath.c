#include "mymath.h"
#include <stdio.h>

int iquot(int n, int m)
{
        int u = 0;
        assert(u == 0 && n >= 0 && m > 0);
        assert(u * m <= n);
        while ((u + 1) * m <= n)
        {
                assert(u+1 * m <= n && u * m <= n);
                u = u + 1;
                assert(u * m <= n);
        }
        assert((u + 1) *m > n && u * m <= n);
        return u;
        
}

int iquot_wof(int n, int m)
{
        int u = 0;
        int o = n + 1;
        int a = 0;
        while (u < o - 1)
        {
                a = (u + o) / 2;
                if (a * m <= n)
                {
                        u = a;
                }else {
                        o = a;
                }
                
        }
        return u;
}

int isqrt(int n)
{
        int u = 0, h = 1;
        assert(h == 1 && u == 0 && n >= 0);
        assert(u * u <= n);
        while (h <= n)
        {
                assert(u * u <= n && h <= n && h == (u + 1) * (u + 1));
                
                
                u = u + 1;
                assert(u * u <= n);
                assert(h == (u) * (u));
                
                h = h + 2 * u + 1;
                assert(h == (u + 1) * (u + 1));
                
        }
        assert(u*u <= n);
        assert((u +1) * (u+1) > n);
        return u;
}

int isqrt_wof(int n)
{
        int u = 0;
        while (u * u < n)
        {
                u++;
        }
        return u;
}

int ilog(int a, int n)
{
        int u = 0, o = n + 1, x;
        assert(o == n +1 && u == 0 && n > 0);
        assert(pow(a, u) <= n && pow(a, o) > n);
        while (u < o - 1)
        {
                assert(u < o -1 && pow(a, u) <= n && pow(a, o) > n);
                x = (u + o) / 2;
                if (pow(a, x) <= n)
                {
                        u = x;
                }else {
                        o = x;
                }
                
        }
        assert(pow(a, u) <= n && pow(a, u +1) > n);
        return u;
}

double helper(double x, int k)
{
        int i;
        double res = 1.0;
        for(i = k; i > 0; i--){
                res *= (x / i);
        }
        
        return res;
}

double sinus(double x, double error)
{
        int n = 0;
        double res = 0, tmp = 0;
        while (error < 2 * helper((double)x, 2*n +3))
        {
                tmp = helper(x, 2*n+1);
                        if (n % 2 == 1)
                        {
                                tmp *= -1;
                        }
                res += tmp;
                n++;
                
        }
        return res;
}

double newton_root(int n, double a, double eps)
{
        double lower = 1.0, upper = a, lower_y, upper_y, gradient, offset, new_x;
        while (upper - lower >= eps)
        {
                lower_y = pow(lower, n);
                upper_y = pow(upper, n);

                gradient = (upper_y - lower_y) / (upper - lower);
                offset = pow(lower, n) - (gradient * lower);
                new_x = -(offset / gradient);
                if (new_x <= lower || new_x >= upper)
                {
                        new_x = lower + ((upper - lower) / 2);
                }
                
                if (pow(new_x, n) >= a)
                {
                        upper = new_x;
                }else {
                        lower = new_x;
                }
                

        }
        return lower;
        
}

double area_squares(int x, double a, double b)
{
        double total_area = (b-a) / x, squares = 0;
        int i;
        for (i = 0; i < x; i++)
        {
                squares += total_area * sqrt((a + total_area * i));
        }
        printf("%f\n", squares);
        return squares;

}

double area(double a, double b, double eps)
{
        int amount_squares = 4;
        double wert2 = area_squares(1, a, b), wert1 = area_squares(2, a, b);
        while (wert1 - wert2 > eps)
        {
                wert2 = wert1;
                wert1 = area_squares(amount_squares, a, b);
                amount_squares *= 2;
        }
        printf("returning %f\n", wert1);
        return wert1;
        
        
}

double fak(int n)
{
        double fak = 1;
        int i;
        for (i = 1; i <= n; i++)
        {
                fak *= i;
        }
        return fak;
}

double calculate_e(double error)
{
        int n = 1, i = 0;
        double res = 0, tmp = 1;
        while (error <= tmp)
        {
                tmp = 2 * (1 / fak(n + 1));
                printf("%i : %f : %f\n", i, res, tmp);
                res += 1 / fak(i);
                i++;
                n++;
        }
        return res;
}

double calculate_pi(double error) {
    int k = 0;
    double pi = 0, term = 1;
    
    while (fabs(term) > error) {
        pi += term;
        k++;
        term = pow(-1, k) / (2.0 * k + 1);
    }
    
    return pi * 4;
}

double cosinus_helper(double x, int k)
{
    int i;
    double res = 1;
    for (i = 0; i < k; i++) {
        res *= x / (k - i); 
    }
    return res;
}

double cosinus(double x, double eps)
{
    double cosinus_result = 0;
    int k = 0;
    do {
        cosinus_result += (k % 2 ? -1 : 1) * cosinus_helper(x, 2 * k);
        k++;
    } while (2 * cosinus_helper(x, 2 * k + 2) > eps);
    return cosinus_result;
}

double newton_root(int n, double a, double eps)
{
    double r = (a + 1) / 2, r_old;
    do {
        r_old = r;
        r = ((n - 1) * r + (a / pow(r, n - 1))) / n;
    } while (fabs(r - r_old) > eps);
    return r;
}