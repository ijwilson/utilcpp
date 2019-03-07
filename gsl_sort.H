/**    @file */
#ifndef GSL_SORT_H__
#define GSL_SORT_H__
//#include <vector> - we can't need vector if we have to pass one!

#ifdef USE_R
#ifndef RMATH_STANDALONE
#include "R.h"
#endif
#include "Rmath.h"
#else
#include "gsl/gsl_sort.h"
#endif

#undef sexp

class gsl_index {
public:
  gsl_index() {
  }
  gsl_index(const std::vector<double> &x) {
    sort(x);
  }
  ~gsl_index() {
    if (p) delete[] p;
  }
  void sort(const std::vector<double> &x) {
     
#ifndef USE_R
    p = new size_t[x.size()];   
    gsl_sort_index(p,&x[0],1,x.size());
#else
    p=new int[x.size()];
    std::vector<double> y(x);
    rsort_with_index(&y[0],p,x.size());
#endif  
  }
  size_t operator[](int i ) const {
    return p[i];
  }
  private:
#ifndef USE_R
  size_t *p;
#else 
  int *p;
#endif
};


#endif
