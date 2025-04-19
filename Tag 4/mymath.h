#ifndef MYMATH_INCLUDED
#define MYMATH_INCLUDED
#include <math.h>
#include <assert.h>

int iquot(int n, int m);
int isqrt(int n);
int ilog(int a, int n);
double sinus(double x, double eps);
double newton_root(int n, double a, double eps);
double area(double a, double b, double eps);
int iquot_wof(int n, int m);
int isqrt_wof(int n);
double calculate_e(double error);
double calculate_pi(double error);
double cosinus(double x, double error);
#endif
