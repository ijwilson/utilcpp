/* A set of functions, mostly similar to those 
   from Numerical Recipies in C, 2nd Edition
   by Press et al.                               */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "cutil.h" /*needed for MALLOC and FREE*/
#include "nrutil.h"
#define NR_END 1
#define FREE_ARG char*

#ifdef  __cplusplus
void *mymalloc(size_t size)
{
  return malloc(size);
}
void    myfree(void *ptr)
{
  return free(ptr);
}
  void *myrealloc(void * ptr, size_t siz)
{
  return realloc(ptr,siz);
}
#endif

/*******************************************************/
/*******************************************************/
void nrerror(char error_text[])
/* Numerical Recipes standard error handler */
{
  fprintf(stderr, "Run-time error in ...\n");
  fprintf(stderr, "%s\n", error_text);
  fprintf(stderr, "...now exiting to system...\n");
  exit(21);
}
/*******************************************************/
float *nrvector(long nl, long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
  float          *v;

  v = (float *) MALLOC((size_t) ((nh - nl + 1 + NR_END) * sizeof(float)));
  if (!v)
    nrerror("allocation failure in vector()");
  return v - nl + NR_END;
}
/*******************************************************/
int  *nrivector(long nl, long nh)
/* allocate an int vector with subscript range v[nl..nh] */
{
  int *v;

  if (nh<nl) 
    nrerror("allocation failure a in nrivector()");
  
  v = (int *) MALLOC((size_t) ((nh - nl + 1 + NR_END) * sizeof(int)));
  if (!v)
    nrerror("allocation failure in nrivector()");
  return v - nl + NR_END;
}

/*******************************************************/

int *nrivector0(long nl, long nh)
/* allocate an int vector with subscript range v[nl..nh] */
{
  int *v;
  long i;

  v = (int *) MALLOC((size_t) ((nh - nl + 1 + NR_END) * sizeof(int)));
   if (!v)
    nrerror("allocation failure in nrivector0()");
  for (i=nl;i<=nh;i++) *(v-nl+NR_END+i)=0;
  return v - nl + NR_END;
}
/*******************************************************/
int *ivector(int nl, int nh)
/* allocate an int vector with subscript range v[nl..nh] */
{
  int            *v;
	
  v = (int *) MALLOC((size_t) ((nh - nl + 1 + NR_END) * sizeof(int)));
  if (!v)
    nrerror("allocation failure in ivector()");
  return v - nl + NR_END;
        }
/*******************************************************/
unsigned char  *nrcvector(long nl, long nh)
/* allocate an unsigned char vector with subscript range v[nl..nh] */
{
  unsigned char  *v;

  v = (unsigned char *) MALLOC((size_t) ((nh - nl + 1 + NR_END) * sizeof(unsigned char)));
  if (!v)
    nrerror("allocation failure in cvector()");
  return v - nl + NR_END;
}
/*******************************************************/
unsigned long  *nrlvector(long nl, long nh)
/* allocate an unsigned long vector with subscript range v[nl..nh] */
{
  unsigned long  *v;

  v = (unsigned long *) MALLOC((size_t) ((nh - nl + 1 + NR_END) * sizeof(long)));
  if (!v)
    nrerror("allocation failure in lvector()");
  return v - nl + NR_END;
}
/***********************************************************/
double *nrdvector(long nl, long nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
  double *v;
#ifdef FUSSY
  if (nh<nl) {
    printf("stop here");
  }
#endif
  v = (double *) MALLOC((size_t) ((nh - nl + 1 + NR_END) * sizeof(double)));
  if (!v) {
    nrerror("allocation failure in nrdvector()");
  }
  return v - nl + NR_END;
}
/**********************************************************/

double  *nrdvector0(long nl, long nh)

/* allocate a double vector with subscript range v[nl..nh] */

{

  double         *v;

  long i;



  v = (double *) MALLOC((size_t) ((nh - nl + 1 + NR_END) * sizeof(double)));

  if (!v)

    nrerror("allocation failure in dvector0()");

  for (i=nl;i<=nh;i++) *(v-nl+NR_END+i)=0.0;

  return v - nl + NR_END;

}

/************************************************************/

double *dvector(int nl,int nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
  double         *v;
  v = (double *) MALLOC((size_t) ((nh - nl + 1 + NR_END) * sizeof(double)));
  if (!v)
    nrerror("allocation failure in dvector()");
  return v - nl + NR_END;
}
/************************************************************/

float **nrmatrix(long nrl, long nrh, long ncl, long nch)

/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */

{

  long            i, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;

  float         **m;



  /* allocate pointers to rows */

  m = (float **) MALLOC((size_t) ((nrow + NR_END) * sizeof(float *)));

  if (!m)

    nrerror("allocation failure 1 in matrix()");

  m += NR_END;

  m -= nrl;



  /* allocate rows and set pointers to them */

  m[nrl] = (float *) MALLOC((size_t) ((nrow * ncol + NR_END) * sizeof(float)));

  if (!m[nrl])

    nrerror("allocation failure 2 in matrix()");

  m[nrl] += NR_END;

  m[nrl] -= ncl;



  for (i = nrl + 1; i <= nrh; i++)

    m[i] = m[i - 1] + ncol;



  /* return pointer to array of pointers to rows */

  return m;

}

/****************************************************************************/

double  **nrdmatrix(long nrl, long nrh, long ncl, long nch)

/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */

{

  long            i, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;

  double        **m;



  /* allocate pointers to rows */

  m = (double **) MALLOC((size_t)((nrow + NR_END) * sizeof(double *)));

  if (!m)

    nrerror("allocation failure 1 in matrix()");

  m += NR_END;

  m -= nrl;



  /* allocate rows and set pointers to them */

  m[nrl] = (double *) MALLOC((size_t)((nrow * ncol + NR_END)*sizeof(double)));

  if (!m[nrl])

    nrerror("allocation failure 2 in matrix()");

  m[nrl] += NR_END;

  m[nrl] -= ncl;



  for (i = nrl + 1; i <= nrh; i++)

    m[i] = m[i - 1] + ncol;



  /* return pointer to array of pointers to rows */

  return m;

}

/****************************************************************************/

double  **nrdmatrix0(long nrl, long nrh, long ncl, long nch)

/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */

{
  long            i,j, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;
  double        **m;

  /* allocate pointers to rows */
  m = (double **) MALLOC((size_t)((nrow + NR_END) * sizeof(double *)));
  if (!m)
    nrerror("allocation failure 1 in matrix()");
  m += NR_END;
  m -= nrl;


  /* allocate rows and set pointers to them */
  m[nrl] = (double *) MALLOC((size_t)((nrow * ncol + NR_END)*sizeof(double)));
  if (!m[nrl])
    nrerror("allocation failure 2 in matrix()");
  m[nrl] += NR_END;
  m[nrl] -= ncl;

  for (i = nrl + 1; i <= nrh; i++)
    m[i] = m[i - 1] + ncol;

  for (i=nrl;i<=nrh;i++) 
    for (j=ncl;j<=nch;j++) m[i][j]=0.0;


  /* return pointer to array of pointers to rows */
  return m;
}

/****************************************************************************/

int   **nrimatrix(long nrl, long nrh, long ncl, long nch)

/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */

{

  long i, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;

  int  **m;



  /* allocate pointers to rows */

  m = (int **) MALLOC((size_t) ((nrow + NR_END) * sizeof(int *)));

  if (!m) nrerror("allocation failure 1 in matrix()");

  m += NR_END;

  m -= nrl;



  /* allocate rows and set pointers to them */

  m[nrl] = (int *) MALLOC((size_t) ((nrow * ncol + NR_END) * sizeof(int)));

  if (!m[nrl])

    nrerror("allocation failure 2 in matrix()");

  m[nrl] += NR_END;

  m[nrl] -= ncl;



  for (i = nrl + 1; i <= nrh; i++)

    m[i] = m[i - 1] + ncol;



  /* return pointer to array of pointers to rows */

  return m;

}

/****************************************************************************/

char   **nrcmatrix(long nrl, long nrh, long ncl, long nch)

/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */

{

  long i, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;

  char  **m;



  /* allocate pointers to rows */

  m = (char **) MALLOC((size_t) ((nrow + NR_END) * sizeof(char *)));

  if (!m) nrerror("allocation failure 1 in matrix()");

  m += NR_END;

  m -= nrl;



  /* allocate rows and set pointers to them */

  m[nrl] = (char *) MALLOC((size_t) ((nrow * ncol + NR_END) * sizeof(char)));

  if (!m[nrl])

    nrerror("allocation failure 2 in matrix()");

  m[nrl] += NR_END;

  m[nrl] -= ncl;



  for (i = nrl + 1; i <= nrh; i++)

    m[i] = m[i - 1] + ncol;



  /* return pointer to array of pointers to rows */

  return m;

}

/****************************************************************************/

long   **nrlmatrix(long nrl, long nrh, long ncl, long nch)

/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */

{

  long i, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;

  long  **m;



  /* allocate pointers to rows */

  m = (long **) MALLOC((size_t) ((nrow + NR_END) * sizeof(long *)));

  if (!m) nrerror("allocation failure 1 in matrix()");

  m += NR_END;

  m -= nrl;



  /* allocate rows and set pointers to them */

  m[nrl] = (long *) MALLOC((size_t) ((nrow * ncol + NR_END) * sizeof(long)));

  if (!m[nrl])

    nrerror("allocation failure 2 in matrix()");

  m[nrl] += NR_END;

  m[nrl] -= ncl;



  for (i = nrl + 1; i <= nrh; i++)

    m[i] = m[i - 1] + ncol;



  /* return pointer to array of pointers to rows */

  return m;

}

/****************************************************************************/

long   **nrlmatrix0(long nrl, long nrh, long ncl, long nch)

/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */

{

  long            i,j;

  long           **m;



  m=nrlmatrix(nrl,  nrh,  ncl,  nch);

  for (i=nrl;i<=nrh;i++) for( j=ncl;j<=nch;j++) m[i][j]=0;

  return m;

}

/****************************************************************************/

int   **nrimatrix0(long nrl, long nrh, long ncl, long nch)

/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */

{
  long            i,j;
  int           **m;

  m=nrimatrix(nrl,  nrh,  ncl,  nch);
  for (i=nrl;i<=nrh;i++) for( j=ncl;j<=nch;j++) m[i][j]=0;
  return m;
}
/*******************************************************/
void free_vector(float *v, long nl)
/* free a float vector allocated with vector() */

{

  FREE((FREE_ARG) (v + nl - NR_END));

}

/*******************************************************/
void free_ivector(int *v, long nl)
/* free an int vector allocated with ivector() */
{
  FREE((FREE_ARG) (v + nl - NR_END));
}
/*******************************************************/
void free_cvector(unsigned char *v, long nl)
/* free an unsigned char vector allocated with cvector() */
{
  FREE((FREE_ARG) (v + nl - NR_END));
}
/*******************************************************/
void free_lvector(unsigned long *v, long nl)
/* free an unsigned long vector allocated with lvector() */
{
  FREE((FREE_ARG) (v + nl - NR_END));
}
/*******************************************************/
void free_dvector(double *v, long nl)
/* free a double vector allocated with dvector() */
{
  FREE((FREE_ARG) (v + nl - NR_END));
}
/*******************************************************/
void free_matrix(float **m, long nrl, long ncl)
/* free a float matrix allocated by matrix() */
{
  FREE((FREE_ARG) (m[nrl] + ncl - NR_END));
  FREE((FREE_ARG) (m + nrl - NR_END));
}
/*******************************************************/
void free_dmatrix(double **m, long nrl, long ncl)
/* free a double matrix allocated by dmatrix() */
{
  FREE((FREE_ARG) (m[nrl] + ncl - NR_END));
  FREE((FREE_ARG) (m + nrl - NR_END));
}
/*******************************************************/
void free_cmatrix(char **m, long nrl, long ncl)
/* free a double matrix allocated by dmatrix() */
{

  FREE((FREE_ARG) (m[nrl] + ncl - NR_END));

  FREE((FREE_ARG) (m + nrl - NR_END));

}

/*******************************************************/

void free_imatrix(int **m, long nrl, long ncl)

/* free an int matrix allocated by imatrix() */

{

  FREE((FREE_ARG) (m[nrl] + ncl - NR_END));

  FREE((FREE_ARG) (m + nrl - NR_END));

}

/*******************************************************/

void free_lmatrix(long **m, long nrl, long ncl)

/* free an int matrix allocated by imatrix() */

{

  FREE((FREE_ARG) (m[nrl] + ncl - NR_END));

  FREE((FREE_ARG) (m + nrl - NR_END));

}

/*********************************************************************/

/*********************************************************************/

/*********  My own extras ********************************************/

/*********************************************************************/

int *copy_ivector(int *vec,int start, int finish)

{

  int i,*tmp;



  tmp = nrivector(start,finish);

  for (i=start;i<=finish;i++) tmp[i]=vec[i];

  return tmp;

}

/*********************************************************************/

double *copy_dvector(double *vec,int start, int finish)

{

  int i;

  double *tmp;



  tmp = nrdvector(start,finish);

  for (i=start;i<=finish;i++) tmp[i]=vec[i];

  return tmp;

}

/*******************************************************/

int **copy_imatrix(int **mat,int startr, int endr, int startc, int endc)

{

  int i,j,**tmp;

  tmp=nrimatrix(startr,endr,startc,endc);

  for (i=startr;i<=endr;i++) {

    for (j=startc;j<=endc;j++)

      tmp[i][j]=mat[i][j];

  }

  return tmp;

}

/*******************************************************/

