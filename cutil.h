#ifndef UTIL_H
#define UTIL_H

#include <float.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CHECK
#define MALLOC mymalloc
#define FREE myfree
#define REALLOC myrealloc
#define LOG mylog
  void *mymalloc(size_t size);
  void myfree(void *ptr);
  void *myrealloc(void * ptr, size_t siz);
  void printalloc(char *mess);
#else
#define MALLOC malloc
#define REALLOC realloc
#define FREE free
#define LOG log
#endif

#ifdef WIN32
#define M_PI  3.14159265358979323846
#define  M_LN2 0.69314718055994530942
  double lgamma(double xx);
#endif

#define M_1_SQRT_2PI	0.398942280401432677939946059934
#define LMY_SQRT_2PI -0.918938533204672669540968854562
#define M_SQRT_32	5.656854249492380195206754896838	/* sqrt(32) */
  double sumd(double *x, int n);
  int sum_icol(int **mat,int col, int nrows);
  double log_D(double b[], int n);
  double log_dmulti_dirichlet(int *x, double *alpha, int n);
  /*int **read_int_matrix_from_FILE(FILE *infile,int *nrow,int *ncol);*/
  void changeivector(int *vec,int *ch, int start, int finish);
  double factrl(int n);
  double lfactrl(int n);
  double log_gammapdf(double x, double *a);
  double log_poisson(double t, double theta, int events);
  double log_bico(int n, int x);
  float factln(int n);
  float erffc(float x);
  double cumnorm(double x, double mean, double var);
  double standardcumnorm(double x);
  double bessi(int n, double x);
  double bessi1(double x);
  double bessi0(double x);
  double dbesi0(double x);
  double dbesi1(double x);
  double dbesi(int n, double x);
  double edbesi0(double x);
  double edbesi1(double x);
  double edbesi(int n, double x);
  /*int get_int(char text[]);*/
  double mylog(double x);

  int imax(int  *t, int n);
  double  dmax(double *x, int n);
  void  drange(double *x, int n, double range[2]);
  int posmin(double *t, int n);
  int posmax(double *t, int n);

  double cumpois(double x, double lambda);
  double dpois(double x, double lambda);

  double lcumpois(double x, double lambda);
  double ldpois(double x, double lambda);

  double plnorm(double x, double logmean, double logsd);
  double dlnorm(double x, double logmean, double logsd);
  double logdlnorm(double x, double logmean, double logsd);
  double dnorm(double x, double mu, double sigma);
  double ldnorm(double x, double mu, double sigma);
  double pnorm(double x, double mu, double sigma);

  double dexp(double x, double rate);
  double pexp(double x, double rate);

  double log_multinomial(double *p, int *x, int k);
  double log_multinomial2(double *p, int *x,int n, int k);
  double log_multinomial3(int *x,int l,double *p, int k);

  void hpsort(int n, double ra[]);
  void ihpsort(unsigned long n, int ra[]);

  int ivecmatch(int *v1, int *v2, int l);

  double log_dmulti_dirichletc(int *x, double *alpha, int k, int n);
  double log_dmulti_dirichletb(int *x, double alpha, int k);

  double log_multinomial3temper(int *x,double *p, int k,int n,
				double temper);
  double log_dmulti_dirichletctemper(int *x, double *alpha, int k, int n, double temper);

  double ldbeta(double x, double a, double b);
  double lddirichlet(double *x, double a, int n);
  double get_prob(int which,double *p, int n);
  void myerror(const char *message);
  void mywarning(char *message);

  int *tabulate(int *v, int nv, int *length);
#ifdef __cplusplus
}
#endif

#endif

