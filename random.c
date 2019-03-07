#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nrutil.h"
#include "random.h"
#include "cutil.h"
/****************************************************************************************/
/*
 *  Mathlib : A C Library of Special Functions
 *  Copyright (C) 1998 Ross Ihaka
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  SYNOPSIS
 *
 *    #include "Mathlib.h"
 *    double sexp(void);
 *
 *  DESCRIPTION
 *
 *    Random variates from the standard exponential distribution.
 *
 *  REFERENCE
 *
 *    Ahrens, J.H. and Dieter, U. (1972).
 *    Computer methods for sampling from the exponential and
 *    normal distributions.
 *    Comm. ACM, 15, 873-882.
 */
/****************************************************************************************/
double sexp(void)
{
    /* q[k-1] = sum(aLOG(2.0)**k/k!) k=1,..,n, */
    /* The highest n (here 8) is determined by q[n-1] = 1.0 */
    /* within standard precision */
    static double q[] =
    {
		0.6931471805599453,
			0.9333736875190459,
			0.9888777961838675,
			0.9984959252914960,
			0.9998292811061389,
			0.9999833164100727,
			0.9999985691438767,
			0.9999998906925558,
			0.9999999924734159,
			0.9999999995283275,
			0.9999999999728814,
			0.9999999999985598,
			0.9999999999999289,
			0.9999999999999968,
			0.9999999999999999,
			1.0000000000000000
    };
    double a, u, ustar, umin;
    int i;

    a = 0.0;
    u = ranDum();
    for (;;) {
		u = u + u;
		if (u > 1.0)
			break;
		a = a + q[0];
    }
    u = u - 1.0;

    if (u <= q[0])
		return a + u;

    i = 0;
    ustar = ranDum();
    umin = ustar;
    do {
		ustar = ranDum();
		if (ustar < umin)
			umin = ustar;
		i = i + 1;
    }
    while (u > q[i]);
    return a + umin * q[0];
}
/******************************************************************/
/*  variables for the kiss random number generator                */
/******************************************************************/
unsigned int k,m,x,y,z,w,carry,r;
/******************************************************************/
/*  kickstart the kiss random number generator                    */
/******************************************************************/
void start_kiss(int seed)
{
  /*fprintf(stderr,"Started the RNG\n");*/
     x=seed;y=102;z=12;w=34535;
     x = x * 69069 + 1;
     y ^= y << 13;
     y ^= y >> 17;
     y ^= y << 5;
     k = (z >> 2) + (w >> 3) + (carry >> 2);
     m = w + w + z + carry;
     z = w;
     w = m;
     carry = k >> 30;
}
/******************************************************************/
void restart_kiss(unsigned int *vals)
{
	k=vals[0];m=vals[1];x=vals[2];y=vals[3];
	z=vals[4];w=vals[5];carry=vals[6];r=vals[7];
}
/******************************************************************/
void kiss_state(unsigned int *vals)
{
	vals[0]=k;vals[1]=m;vals[2]=x;vals[3]=y;
	vals[4]=z;vals[5]=w;vals[6]=carry;vals[7]=r;
}
/******************************************************************/
/*   Keep It Simple Stupid random number generator from George
     Marsaglia's DIEHARD cdrom                                    */
/******************************************************************/
unsigned int kiss(void)
{
     x = x * 69069 + 1;
     y ^= y << 13;
     y ^= y >> 17;
     y ^= y << 5;
     k = (z >> 2) + (w >> 3) + (carry >> 2);
     m = w + w + z + carry;
     z = w;
     w = m;
     carry = k >> 30;
     return x+y+z;
}
/******************************************************************/
double dkiss(void)
{
    return ((double)kiss()+0.5)/4294967296.0;
}
/******************************************************************/
int faircoin(void)
{
    if (ranDum()<0.5) return 0;
    else return 1;
}
/*******************************************************************/
int gen_from_p(double  *p, int n)
/* a routine to pick a number from 1 to n based on cumulative
probabilities in p    */
{
	double prob;
	int where;

	prob = ranDum();
	where = (int)prob*n+1;
	for (;;) {
		if  (prob <= p [where]) {
			if (where==1) return 1;
			if (prob > p[where-1]) return where;
			else where--;
		} else {
			where++;
			if (where>=n) return n;
		}
	}
}
/*******************************************************************/
void multiple_gen_from_probs(int *x, int c, double  *p, int k)
/* a routine to pick a number from 1 to n based on cumulative
probabilities in p    */
{
	int i;
	double *cprob,sum=0.0;

	cprob = nrdvector(0,k);
	cprob[0]=0.0;
	for (i=1;i<=k;i++) sum += p[i];
	if (sum <=0.0)
		myerror("error with probabilities in multiple_gen...");
	for (i=1;i<=k;i++) cprob[i]=cprob[i-1]+p[i]/sum;
	for (i=1;i<=c;i++)
		x[i]=gen_from_p(cprob,k);
	free_dvector(cprob,0);
}
/*******************************************************************/
void multiple_gen_from_probstemper(int *x,  double  *p, int k,int n,double temper)
/* a routine to pick a number from 1 to n based on cumulative
probabilities in p - but scaled by the tempering val temper    */
{
	int i;
	double *cprob,sum=0.0;

	cprob = nrdvector(0,k);
	cprob[0]=0.0;
	for (i=1;i<=k;i++) {
		cprob[i] = pow(p[i],temper);
		sum+=cprob[i];
	}
	if (sum <=0.0)
		myerror("error with probabilities in multiple_gen...");
	for (i=1;i<=k;i++) cprob[i]=cprob[i-1]+cprob[i]/sum;
	for (i=1;i<=n;i++) x[i]=gen_from_p(cprob,k);
	free_dvector(cprob,0);
}
/*******************************************************************/
void multiple_gen_from_absprobs(int *x, int c, double  *p, int k)
/* a routine to pick a number from 1 to n based on cumulative
probabilities in p    */
{
	int i;
	double *cprob;

	cprob = nrdvector(0,k);
	cprob[0]=0.0;

	for (i=1;i<=k;i++) cprob[i]=cprob[i-1]+p[i];
	for (i=1;i<=c;i++) x[i]=gen_from_p(cprob,k);
	free_dvector(cprob,0);
}
/**************************************************************/
int  gen_from_probs(double  *p, int n)
{
  double *cprob,sum=0.0;
  int where,i;

  cprob=(double*)MALLOC((n+1)*sizeof(double));
  if (!cprob) myerror("error allocating cprob");
  cprob[0]=0.0;

  for (i=1;i<=n;i++) sum += p[i];
  /* if (sum<0.0001) printf("sum = %g\n",sum); */
  if (sum <=0.0) {
	  printf("sum = %g\n",sum);
	  myerror("error:  sum of probabilities less than or equal to 0 in gen_from_probs");
  }
  for (i=1;i<=n;i++) cprob[i]=cprob[i-1]+p[i]/sum;

  where = gen_from_p(cprob,n);
  FREE(cprob);
  return where;
}
/**************************************************************/
int gen_from_probs2(double  *p, int n,double *prob)
{
  double *cprob,sum=0.0;
  int where,i;

  cprob = nrdvector(0,n);
  cprob[0]=0.0;

  for (i=1;i<=n;i++) sum += p[i];
  if (sum <=0.0) {
    /* printf("sum = %g\n",sum);
    write_dvector(stdout," ",p,1,n); */
	  printf("warning:  sum of probabilities less than or equal to 0 in gen_from_probs2\n");
	  *prob=1E-100;
	  return runiformint(1,n);
  }
  for (i=1;i<=n;i++) cprob[i]=cprob[i-1]+p[i]/sum;

  where = gen_from_p(cprob,n);
  free_dvector(cprob,0);
  *prob=p[where]/sum;
  return where;
}
/**************************************************************/
int  gen_from_probinteg(int *p, int n)
{
  double *cprob,sum=0.0;
  int where,i;

  cprob = nrdvector(0,n);
  cprob[0]=0.0;

  for (i=1;i<=n;i++) sum += (double)p[i];
  if (sum <=0.0) return 0;
  for (i=1;i<=n;i++) cprob[i]=cprob[i-1]+(double)p[i]/sum;

  where = gen_from_p(cprob,n);
  free_dvector(cprob,0);
  return where;
}
/**************************************************************/
double bnldev(double  pp,int n)
{
    int j;
    static int nold=(-1);
    double am,em,g,angle,p,bnl,sq,t,y;
    static double pold=(-1.0),pc,plog,pclog,en,oldg;

    p = (pp<=0.5 ? pp: 1.0-pp);

    am = n*p;
    if (n<25) {
        bnl=0.0;
        for (j=1;j<=n;j++)
            if (ranDum() < p) ++bnl;
    }
    else if (am < 1.0) {
        g = exp(-am);
        t=1.0;
        for (j=0;j<=n;j++)
        {
            t *= ranDum();
            if (t<g) break;
        }
        bnl = (j<=n ? j : n);
    }
    else {
        if (n!=nold) {
            en=n;
            oldg=lgamma(en+1.0);
            nold=n;
        }
        if (p!=pold) {
            pc = 1.0-p;
            plog=LOG(p);
            pclog=LOG(pc);
            pold=p;
        }
        sq=sqrt(2.0*am*pc);
        do {
            do {
                angle = M_PI*ranDum();
                y=tan(angle);
                em=sq*y+am;
            } while (em <0.0 || em >= (en + 1.0));
            em = floor(em);
            t=1.2*sq*(1.0+y*y)*exp(oldg-lgamma(em+1.0)
                -lgamma(en-em+1.0)+em*plog+(en-em)*pclog);
        } while (ranDum() > t);
        bnl = em;
    }
    if (p!= pp) bnl=n-bnl;
    return bnl;
}
/****************************************************************************/
void permute_integers(int *data, int n)
/*requres a random number generator*/
{
	int temp,i,which;

	for (i=1;i<=n;i++) {
		which = 1+(int)(ranDum()*n);
		temp = *(data+i);
		*(data+i) = *(data+which);
		*(data+which)=temp;
	}
}
/*************************************************************************/
double normal(void)
{
	static int iset = 0;
	static double gset;
	double  fac,rsq,v1,v2;

	if (iset == 0) {
		do {
			v1 = 2.0*ranDum( )-1.0;
			v2 = 2.0*ranDum( )-1.0;
			rsq = v1*v1+v2*v2;
		} while (rsq >= 1.0 || rsq == 0);
		fac = sqrt(-2.0*LOG(rsq)/rsq);
		gset = v1 * fac;
		iset = 1;
		return v2*fac;
	} else {
		iset = 0;
		return gset;
	}
}
/****************************************************************/
double poidev(double xm)
{
	static double sq,alxm,g,oldm=(-1.0);
	double em,t,y;

	if (xm < 12) {
		if (xm != oldm) {
			oldm = xm;
			g = exp(-xm);
		}
		em = -1;
		t = 1.0;
		do {
			++em;
			t*=ranDum();
		} while (t > g);
	} else {
		if (xm!=oldm) {
			oldm = xm;
			sq = sqrt(2.0*xm);
			alxm = LOG(xm);
			g=xm*alxm-lgamma(xm+1.0);
		}
		do {
			do {
				y=tan(M_PI*ranDum());
				em = sq*y+xm;
			} while (em < 0.0);
			em = floor(em);
			t=0.9*(1.0+y*y)*exp(em*alxm-lgamma(em+1.0)-g);
		} while (ranDum()>t);
	}
	return(em);
}
/****************************************************************/
double rnorm(double mu, double sigma)
{
    return mu + sigma * normal();
}
/****************************************************************/
double rlnorm(double logmean, double logsd)
{
    return exp(rnorm(logmean, logsd));
}
/****************************************************************/
void rdirichlet(double *x, double a, int n)
{
    int i;
    double sum=0.0;

    for (i =1;i<=n;i++) {
		x[i]=rgamma(a,1.0);
		sum+=x[i];
    }
    for (i =1;i<=n;i++) x[i]/=sum;
}
/****************************************************************/
double rbeta(double a,double b)
{
	double x,y;
	x=rgamma(a,1.0);
	y=rgamma(b,1.0);
	return x/(x+y);
}
/****************************************************************/
void rdirichletb(double *x, double *a, int k)
{

    int i;
    double sum=0.0;

    for (i =1;i<=k;i++) {
		x[i]=rgamma(a[i],1.0);
		sum+=x[i];
    }
    for (i =1;i<=k;i++) x[i]/=sum;
}
/***********************************************************************/
/*  generate a vector of n multinomially dirichlet distributed random  */
/*  variables a vector of parameters alpha of length k                 */
void rmultidirichletc(int *x, double *a, int k, int n)
{
    int i;
    double sum=0.0,*xtmp;

	xtmp=nrdvector(1,k);
    for (i=1;i<=k;i++) {
		xtmp[i]=rgamma(a[i],1.0);
		sum+=xtmp[i];
    }
	for (i =1;i<=k;i++) xtmp[i]/=sum;
	multiple_gen_from_probs(x,n,xtmp,k);
	free_dvector(xtmp,1);
}
/***********************************************************************/
/*  generate a vector of n exchangeable multinomially dirichlet
distributed random  with parameters alpha and k                        */
/***********************************************************************/
void rexchangeablemultidirichletc(int *x, double alpha, int k, int n)
{
    int i;
    double *xtmp;

	xtmp=nrdvector(1,k);
	for (i=1;i<=k;i++) xtmp[i]=alpha;
	for (i=1;i<=n;i++) {
		x[i]=gen_from_probs(xtmp,k);
		xtmp[x[i]]+=1;
	}
	free_dvector(xtmp,1);
}
/***********************************************************************/
void rmultidirichletctemper(int *x, double *a, int k, int n,double temper)
{
    int i;
	double *atmp;
	atmp=nrdvector(1,k);
	for (i=1;i<=k;i++) atmp[i]=pow(a[i],temper);
	rmultidirichletc(x,atmp,k,n);
	free_dvector(atmp,1);
}
/****************************************************************/
static double a1 = 0.3333333;
static double a2 = -0.250003;
static double a3 = 0.2000062;
static double a4 = -0.1662921;
static double a5 = 0.1423657;
static double a6 = -0.1367177;
static double a7 = 0.1233795;
static double e1 = 1.0;
static double e2 = 0.4999897;
static double e3 = 0.166829;
static double e4 = 0.0407753;
static double e5 = 0.010293;
static double q1 = 0.04166669;
static double q2 = 0.02083148;
static double q3 = 0.00801191;
static double q4 = 0.00144121;
static double q5 = -7.388e-5;
static double q6 = 2.4511e-4;
static double q7 = 2.424e-4;
static double sqrt32 = 5.656854;
static double aa = 0.;
static double aaa = 0.;
double rgamma(double a, double scale)
#define repeat for(;;)
/* Taken from R */
{
	static double b, c, d, e, p, q, r, s, t, u, v, w, x;
	static double q0, s2, si;
	double ret_val;

	if (a < 1.0) {
		/* alternate method for parameters a below 1 */
		/* 0.36787944117144232159 = exp(-1) */
		aa = 0.0;
		b = 1.0 + 0.36787944117144232159 * a;
		repeat {
			p = b * dkiss();
			if (p >= 1.0) {
				ret_val = -LOG((b - p) / a);
				if (sexp() >= (1.0 - a) * LOG(ret_val))
					break;
			} else {
				ret_val = exp(LOG(p) / a);
				if (sexp() >= ret_val)
					break;
			}
		}
		return scale * ret_val;
	}
	/* Step 1: Recalculations of s2, s, d if a has changed */
	if (a != aa) {
		aa = a;
		s2 = a - 0.5;
		s = sqrt(s2);
		d = sqrt32 - s * 12.0;
	}
	/* Step 2: t = standard normal deviate, */
	/* x = (s,1/2)-normal deviate. */
	/* immediate acceptance (i) */

	t = normal();
	x = s + 0.5 * t;
	ret_val = x * x;
	if (t >= 0.0)
		return scale * ret_val;

	/* Step 3: u = 0,1 - uniform sample. squeeze acceptance (s) */
	u = dkiss();
	if (d * u <= t * t * t) {
		return scale * ret_val;
	}
	/* Step 4: recalculations of q0, b, si, c if necessary */

	if (a != aaa) {
		aaa = a;
		r = 1.0 / a;
		q0 = ((((((q7 * r + q6) * r + q5) * r + q4)
			* r + q3) * r + q2) * r + q1) * r;

		/* Approximation depending on size of parameter a */
		/* The constants in the expressions for b, si and */
		/* c were established by numerical experiments */

		if (a <= 3.686) {
			b = 0.463 + s + 0.178 * s2;
			si = 1.235;
			c = 0.195 / s - 0.079 + 0.16 * s;
		} else if (a <= 13.022) {
			b = 1.654 + 0.0076 * s2;
			si = 1.68 / s + 0.275;
			c = 0.062 / s + 0.024;
		} else {
			b = 1.77;
			si = 0.75;
			c = 0.1515 / s;
		}
	}
	/* Step 5: no quotient test if x not positive */

	if (x > 0.0) {
		/* Step 6: calculation of v and quotient q */
		v = t / (s + s);
		if (fabs(v) <= 0.25)
			q = q0 + 0.5 * t * t * ((((((a7 * v + a6)
			* v + a5) * v + a4) * v + a3)
			* v + a2) * v + a1) * v;
		else
			q = q0 - s * t + 0.25 * t * t + (s2 + s2)
			* LOG(1.0 + v);


		/* Step 7: quotient acceptance (q) */

		if (LOG(1.0 - u) <= q)
			return scale * ret_val;
	}
	/* Step 8: e = standard exponential deviate */
	/* u= 0,1 -uniform deviate */
	/* t=(b,si)-double exponential (laplace) sample */

	repeat {
		e = sexp();
		u = dkiss();
		u = u + u - 1.0;
		if (u < 0.0)
			t = b - si * e;
		else
			t = b + si * e;
		/* Step  9:  rejection if t < tau(1) = -0.71874483771719 */
		if (t >= -0.71874483771719) {
			/* Step 10:  calculation of v and quotient q */
			v = t / (s + s);
			if (fabs(v) <= 0.25)
				q = q0 + 0.5 * t * t * ((((((a7 * v + a6)
				* v + a5) * v + a4) * v + a3)
				* v + a2) * v + a1) * v;
			else
				q = q0 - s * t + 0.25 * t * t + (s2 + s2)
				* LOG(1.0 + v);
			/* Step 11:  hat acceptance (h) */
			/* (if q not positive go to step 8) */
			if (q > 0.0) {
				if (q <= 0.5)
					w = ((((e5 * q + e4) * q + e3)
					* q + e2) * q + e1) * q;
				else
					w = exp(q) - 1.0;
				/* if t is rejected */
				/* sample again at step 8 */
				if (c * fabs(u) <= w * exp(e - 0.5 * t * t))
					break;
			}
		}
	}
	x = s + 0.5 * t;
	return scale * x * x;
}
/*******************************************************************/
int runiformint(int from, int to)
{
	double len;
	if (to==from) return to;
	len=(double)(to-from+1);
	return from + (int)(ranDum()*len);
}
/*********************************************************************/
/*  A function to sample 2 integers without
         replacement from (from,to)                                  */
/*********************************************************************/
void sample2int(int wh[2],int from, int to)
{
	double len;
	len=(double)(to-from+1);
	wh[0] = from + (int)(ranDum()*len);
	wh[1] = from + (int)(ranDum()*(len-1.));
	if (wh[1]>=wh[0]) wh[1]++;
	return;
}
/*******************************************************************/
/*  get a sample from the Ewen's sampling formula                  */
/*******************************************************************/
int rEwens(int *x, double theta, int n)
{
  double *a;
  int k=1,i;
  
  a=nrdvector(1,n+1);
  x[1]=1;
  a[1]=1.0;
  a[2]=theta;
  k=2;
  for (i=2;i<=n;i++) {
    x[i]=gen_from_probs(a,k);
    if (x[i]<=k-1) a[x[i]]+=1.0;
    else {
      a[k]=1;
      k+=1;
      a[k] = theta;
    }
  }
  free_dvector(a,1);
  return k-1;
}
/*******************************************************************/
/*int *rEwens(int *k,double theta, int n)
{
	int *temp,i;
	double *p;

	temp=nrivector(1,n);
	p=nrdvector(1,n);

	*k=1;
	temp[1]=1;
	p[1]=1;
	p[2]=theta;
	for (i=2;i<=n;i++) {
		temp[i]=gen_from_probs(p,(*k)+1);
		if (temp[i]>*k) {
			*k=(*k)+1;
			p[*k]=1.;
			p[(*k)+1]=theta;
		} else p[temp[i]]+=1.;
	}
	free_dvector(p,1);
	return temp;
}*/
/******************************************************************/
