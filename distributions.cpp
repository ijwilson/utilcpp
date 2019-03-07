#ifndef DISTRIBUTIONS_H__
#define DISTRIBUTIONS_H__

#include <cmath>

#include "utilityfunctionals.H"
#include "util.H"
#include "gsl_rand.H"

lfactorl lfactrl_dist;

/***************************************************************************/
 double log_D(const double *b, int n)
 {
   double sum=0.0,temp=0.0;
  
   for (int i=0;i<n;i++) {
     sum += b[i];
     temp += lgamma(b[i]);
   }
   temp -= lgamma(sum);
   return temp;
 }
/**************************************************************************
 * An overloaded log_D when you have a constant b
***************************************************************************/
double log_D(double b, int n)
{
  double sum=double(n)*b;
  return double(n)*lgamma(b) -lgamma(sum);
}
/**********************************************************************/
/*    The log density for the multinomial-Dirichlet distribution for 
      vector of counts x, where x[i] is the number of observations i 
	  with parameters alpha, a vector of length k                     */
/**********************************************************************/ 
double log_dmulti_dirichlet(const int *x, double *alpha, int n)
{
  int tot=0;
  double temp;
  
  double *ax = new double[n];
  temp = -log_D(alpha,n);
  for (int i=0;i<n;i++) {
    tot+=x[i];
    ax[i]= alpha[i]+(double)x[i];
    temp -= lfactrl_dist(x[i]);
  }
  temp += lfactrl_dist(tot)+log_D(ax,n);
  delete [] ax;
  return temp;
}
/**********************************************************************/
/*    The log density for the multinomial-Dirichlet distribution for 
      vector of counts x, where x[i] is the number of observations i 
	  with parameters alpha, a vector of length k                     */
/**********************************************************************/ 
double log_dmulti_dirichlet(const int *x, double alpha, int n)
{
  int tot=0;
   
  double *ax = new double[n];
  double temp = -log_D(alpha,n);
  for (int i=0;i<n;i++) {
    tot+=x[i];
    ax[i]= alpha+(double)x[i];
    temp -= lfactrl_dist(x[i]);
  }
  temp += lfactrl_dist(tot)+log_D(ax,n);
  delete [] ax;
  return temp;
}
/**********************************************************************/
/*    The log density for the multinomial-Dirichlet distribution for 
      vector of counts x, where x[i] is the number of observations i 
	  with parameters alpha, a vector of length k                     */
/**********************************************************************/ 
double log_dmulti_dirichlet(const int *x, double *p, double F, int n)
{
  int tot=0;
  
  double *alpha=new double[n];
  for (int i=0;i<n;i++) alpha[i]=p[i]*(1./F-1.);
  double *ax = new double[n];
  double temp = -log_D(alpha,n);
  for (int i=0;i<n;i++) {
    tot+=x[i];
    ax[i]= alpha[i]+(double)x[i];
    temp -= lfactrl_dist(x[i]);
  }
  temp += lfactrl_dist(tot)+log_D(ax,n);
  delete [] ax;
  delete[] alpha;
  return temp;
}
/***************************************************************************/
double lddirichlet(double *x, double a, int n)
{
  /* expected frequencies   */
  double *alpha = new double[n];

  double lx=0.0;
  for (int i=0;i<n;i++) {
    alpha[i]=a;
    lx += (a-1.0)*log(x[i]);
  }
  lx -= log_D(alpha,n);
  delete[] alpha;
  return lx;
}
/**************************************************************************/
void rdirichlet(double *x,double *alpha, int k, rng &r)
{
  double sm=0.0;
  for (int i=0;i<k;i++) {
    x[i]=r.sexp(alpha[i]);
    sm+=x[i];
  }
  for (int i=0;i<k;i++) x[i]/=sm;

  return;
}
/**************************************************************************/
void rdirichlet(double *x,double alpha, int k, rng &r)
{
  double sm=0.0;
  for (int i=0;i<k;i++) {
    x[i]=r.sexp(alpha);
    sm+=x[i];
  }
  for (int i=0;i<k;i++) x[i]/=sm;

  return;
}
/***************************************************************************/
double ldbeta(double x, double a,double b)
{
  return (a-1.)*log(x)+(b-1)*log(1.-x)
		-lgamma(a)-lgamma(b)+lgamma(a+b);
}
/***************************************************************************/
double ldbetaBinomial(int x, int n, double p, double s)
/** 
 *          p(x) = B(x+s p,n-x+s (1-p)) Choose(n,x) / B(s p,s (1-p))  
 *
 *      for x = 0, ..., n where B() is the beta function. 
 *
 */
{ 
  return lgamma(n+s) - lgamma(x+s*p) - lgamma(n-x+s*(1.-p)) 
    -lgamma(s*p)-lgamma(s*(1.-p)) +lgamma(s) 
    +lfactrl_dist(n)-lfactrl_dist(x)-lfactrl_dist(n-x);  
}


/*******************************************************************/
/*  get a sample from the Ewen's sampling formula                  */
/* This sample looks like a sequential sample with the first observed
 * allele in position 0   */
/*******************************************************************/
int rEwens(int *x, double theta, int n, rng &r)
{
  double *a= new double[n+1];
  x[0]=0;
  a[0]=1.0;
  a[1]=theta;
  int k=2;
  for (int i=1;i<n;i++) {
    x[i]=gen_from_pb(a,k,r);
    if (x[i]==k-1) {
      a[k-1]=1.0;
      a[k]=theta;
      k++;
    } else {
      a[x[i]] += 1.0;
    }
  }
  delete [] a;
  return k-1;
}


double cumnorm(double x, double mean, double var);


#endif
