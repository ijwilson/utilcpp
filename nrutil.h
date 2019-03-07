#ifndef _NR_UTILS_H_
#define _NR_UTILS_H_
#include <stdlib.h>

#ifdef __cplusplus
    extern "C" {
#endif

void *nrmalloc(int n);
void nrerror(char error_text[]);
/*void myerror(char error_text[]);
void mywarning(char *message);*/
float *nrvector(long nl, long nh);
int *nrivector(long nl, long nh);
unsigned char *nrcvector(long nl, long nh);
unsigned long *nrlvector(long nl, long nh);
double *nrdvector(long nl, long nh);
float **nrmatrix(long nrl, long nrh, long ncl, long nch);
double **nrdmatrix(long nrl, long nrh, long ncl, long nch);
int **nrimatrix(long nrl, long nrh, long ncl, long nch);
char   **nrcmatrix(long nrl, long nrh, long ncl, long nch);
long   **nrlmatrix(long nrl, long nrh, long ncl, long nch);
float **nrsubmatrix(float **a, long oldrl, long oldrh, long oldcl, long oldch,
	long newrl, long newcl);
float **nrconvert_matrix(float *a, long nrl, long nrh, long ncl, long nch);
float ***nrf3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
void free_vector(float *v, long nl);
void free_ivector(int *v, long nl);
void free_cvector(unsigned char *v, long nl);
void free_lvector(unsigned long *v, long nl);
void free_dvector(double *v, long nl);
void free_matrix(float **m, long nrl, long ncl);
void free_dmatrix(double **m, long nrl, long ncl);
void free_imatrix(int **m, long nrl, long ncl);
void free_cmatrix(char **m, long nrl, long ncl);
void free_lmatrix(long **m, long nrl, long ncl);

int *ivector(int nl,int nh);
double *dvector(int nl,int nh);

int *nrivector0(long nl, long nh);
double *nrdvector0(long nl, long nh);
int   **nrimatrix0(long nrl, long nrh, long ncl, long nch);
double  **nrdmatrix0(long nrl, long nrh, long ncl, long nch);
long   **nrlmatrix0(long nrl, long nrh, long ncl, long nch);

int *copy_ivector(int *vec,int start, int finish);
int **copy_imatrix(int **mat,int startr, int endr, int startc, int endc);

double *copy_dvector(double *vec,int start, int finish);



#ifdef __cplusplus
}
#endif


#endif /* _NR_UTILS_H_ */

