// @file    Time-stamp: <2005-08-16 19:46:05 nijw>
#ifndef GSL_ROOT_H___
#define GSL_ROOT_H___

#include "utilcpp/gsl_exception.H"
//#include "utilcpp/gsl_vector.H"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_min.h>
#include <gsl/gsl_roots.h>
#include <string>
// a class to implement the one dimensional gsl_minimisation root finder routine
// should work for any function or class that provides an () operator
// is untested for anything exceptwakely class 
template<typename T>
class gsl_root {
public:
  /**  the constructor - has to be some fancy footwork to 
   * allow anything to be passed to the gsl_root 
   * do it through the static localf member function 
   * that is passed the address of the current gsl_root
   * as a void pointer                                          */
  gsl_root(T &thefunc,  double lower, double upper):
    f_(thefunc)
  {
    // the default root finding algorithm    
    mt_= gsl_root_fsolver_bisection;    
    s=gsl_root_fsolver_alloc(mt_);
    f.function=&gsl_root::localf;
    f.params=(void*) this;
    gsl_root_fsolver_set(s,&f,lower,upper);
  }
  /** destructor                                             */
  ~gsl_root() {
    gsl_root_fsolver_free(s);
  }
  /** minimise                                                */
  double findroot(double epsabs=0.001,double epsrel=0.0,int maxiter=100) {
    for (int iter=0;iter<maxiter;iter++) {
      int status=gsl_root_fsolver_iterate(s);
      if (status) throw gsl_exception("error in root finder",status);
      if (test_precision(epsabs,epsrel)) return gsl_root_fsolver_root(s); 
    }
    throw std::string("too many iterations in gsl_root");
  }
    /** test whether the required precision has been met       */
    bool test_precision(double epsabs,double epsrel) {
      double a=gsl_root_fsolver_x_lower(s);
      double b=gsl_root_fsolver_x_upper(s);
      int status=gsl_root_test_interval(a,b,epsabs,epsrel);
      if (status==GSL_SUCCESS) return true;
      return false;
  }
  /** The local function that is called                      */
  static double localf(double x, void *params) {
    gsl_root *gm;
    gm = (gsl_root*) params;
    return gm->f_(x);
  }
private:
  T  &f_;
  gsl_function f;
  const gsl_root_fsolver_type *mt_;
  gsl_root_fsolver *s;
};
#endif
