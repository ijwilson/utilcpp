#ifndef RANDOM_H
#define RANDOM_H

#ifdef __cplusplus
    extern "C" {
#endif

#define ranDum dkiss
#define starTup start_kiss

double dkiss(void);
void start_kiss(int seed);
void restart_kiss(unsigned int *vals);
void kiss_state(unsigned int *vals);

int  gen_from_p(double  *p, int n);
int  gen_from_probs(double  *p, int n);
int  gen_from_probs2(double  *p, int n,double *prob);
int  gen_from_probinteg(int *p, int n);
void multiple_gen_from_probs(int *x, int c, double  *p, int k);
void multiple_gen_from_absprobs(int *x, int c, double  *p, int k);
void multiple_gen_from_probstemper(int *x, double  *p, int k, int n,double temper);

double bnldev(double pp,int n);
int faircoin(void);
void permute_integers(int *data, int n);
double normal(void);
double poidev(double xm);

double rgamma(double a, double scale);
void rdirichlet(double *x, double a, int n);
void rmultidirichletc(int *x, double *a, int k, int n);
void rexchangeablemultidirichletc(int *x, double alpha, int k, int n);
void rmultidirichletctemper(int *x, double *a, int k, int n,double temper);
double rnorm(double mu, double sigma);
double rlnorm(double logmean, double logsd);

int runiformint(int from, int to);
void sample2int(int wh[2],int from, int to);
int rEwens(int *x, double theta, int n);
double rbeta(double a,double b);
#ifdef __cplusplus
	}
#endif
#endif



