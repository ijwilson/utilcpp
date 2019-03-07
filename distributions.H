#ifndef DISTRIBUTINS_H__IJW
#define DISTRIBUTINS_H__IJW

#include "gsl_rand.H"
double log_D(const double *b, int n);

double log_dmulti_dirichlet(const int *x, double *alpha, int n);
double log_dmulti_dirichlet(const int *x, double alpha, int n);

// parametrisation in terms of Fst
double log_dmulti_dirichlet(const int *x, double *p, double F, int n);
double lddirichlet(double *x, double a, int n);
double ldbeta(double x, double a,double b);
double ldbetaBinomial(int x, int n, double p, double s);

void rdirichlet(double *x,double *alpha, int k, rng &r);
void rdirichlet(double *x,double alpha, int k, rng &r);
int rEwens(int *x, double theta, int n,rng &r);

#endif

