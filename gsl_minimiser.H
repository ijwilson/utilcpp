// @file    Time-stamp: <05/10/13 15:16:33 nijw>
#ifndef GSL_MINIMISER_H___
#define GSL_MINIMISER_H___

#include "utilcpp/gsl_exception.H"
#include "utilcpp/gsl_vector.H"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_min.h>
#include <gsl/gsl_multimin.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
/** a class to implement the one dimensional gsl_minimisation routine
 * should work for any function or class that provides an () operator
 * is untested for anything apart from pac classes  */
template<typename T>
class gsl_minimiser {
public:
  /**  the constructor - has to be some fancy footwork to 
   * allow anything to be passed to the gsl_minimiser 
   * do it through the static localf member function 
   * that is passed the address of the current gsl_minimiser
   * as a void pointer                                         */
  gsl_minimiser(T &thefunc, double guess, double left
		, double right,bool find_minim=true):
    f_(thefunc),find_min(find_minim)
  {
    // the default minimiser function    
    mt_= gsl_min_fminimizer_brent;
    s=gsl_min_fminimizer_alloc(mt_);
    f.function=&gsl_minimiser::localf;
    f.params=(void*) this;
    int status = gsl_min_fminimizer_set(s,&f,guess,left,right);
    if (status==GSL_FAILURE) {
      std::ostringstream oss;
      oss << "error in minimise, with values" << guess << " in the region "
	  << "(" << left <<"," << right << ")" << std::endl; 
      throw gsl_exception(oss.str().c_str(),status);
    }
  };
  gsl_minimiser(T &thefunc, double guess, double fg, double left, double fl
		, double right,double fr, bool find_minim=true):
    f_(thefunc),find_min(find_minim)
  {
    // the default minimiser function    
    mt_= gsl_min_fminimizer_brent;
    s=gsl_min_fminimizer_alloc(mt_);
    f.function=&gsl_minimiser::localf;
    f.params=(void*) this;
    int status = gsl_min_fminimizer_set_with_values(s,&f,guess,fg,left,fl,right,fr);
    if (status==GSL_FAILURE) {
      std::ostringstream oss;
      oss << "error in minimise, starting at " << guess << "with value " << fg << " in the region "
	  << "(" << left <<"," << right << ")" << " with initial function values " 
	  <<  fl << " and " << fr << std::endl; 
      throw gsl_exception(oss.str().c_str(),status);
    }
  }
  /** destructor                                             */
  ~gsl_minimiser() {
    gsl_min_fminimizer_free(s);
  }
  /** minimise                                                */
  double minimise(double epsabs=0.001,double epsrel=0.0,int maxiter=100) {
    for (int iter=0;iter<maxiter;iter++) {
      int status=gsl_min_fminimizer_iterate(s);
      if (status) throw gsl_exception("error in minimise",status);
      //     std::cout << iter <<" " << gsl_min_fminimizer_x_minimum(s) << std::endl;
      if (test_precision(epsabs,epsrel)) return gsl_min_fminimizer_x_minimum(s); 
    }
    throw std::range_error("too many iterations in gsl_minimiser");
  }
  double minimum() {
    if (find_min) return gsl_min_fminimizer_f_minimum(s);
    else return -gsl_min_fminimizer_f_minimum(s); 
  }
  /** test whether the required precision has been met       */
  bool test_precision(double epsabs,double epsrel) {
    double a=gsl_min_fminimizer_x_lower(s);
    double b=gsl_min_fminimizer_x_upper(s);
    int status=gsl_min_test_interval(a,b,epsabs,epsrel);
    if (status==GSL_SUCCESS) return true;
    return false;
  }
  /** The local function that is called                      */
  static double localf(double x, void *params) {
    gsl_minimiser *gm;
    gm = (gsl_minimiser*) params;
    if (gm->find_min) return gm->f_(x);
    else return -gm->f_(x);
  }
private:
  T  &f_;
  gsl_function f;
  const gsl_min_fminimizer_type *mt_;
  gsl_min_fminimizer *s;
  bool find_min;
};
// a class to implement the multi-dimensional gsl_minimisation routine
// should work for any function or class that provides an () operator
// is untested for anything apart from pac classes
template<typename T>
class gsl_minimiserMD {
public:
  /**  the constructor - has to be some fancy footwork to 
   * allow anything to be passed to the gsl_minimiser 
   * do it through the static localf member function 
   * that is passed the address of the current gsl_minimiser
   * as a void pointer                                         */
  gsl_minimiserMD(T &thefunc, const std::vector<double> &start, std::vector<double> &stepsize
		  ,bool find_minim=true):
    f_(thefunc),find_min(find_minim)
  {
    // the default minimiser function   
    np = start.size();
    mt_= gsl_multimin_fminimizer_nmsimplex;
    s=gsl_multimin_fminimizer_alloc(mt_,np);
    f.f=&gsl_minimiserMD::localf;
    f.params=(void*) this;
    f.n=np;
    st=to_gsl_vector(start);
    ss=to_gsl_vector(stepsize);
    gsl_multimin_fminimizer_set(s,&f,st,ss);
  }
  /** destructor                                             */
  ~gsl_minimiserMD() {
    gsl_multimin_fminimizer_free(s);
    gsl_vector_free(ss);
    gsl_vector_free(ss);
  }
  /** minimise                                                */
  std::vector<double> minimise(double eps=0.01,int maxiter=100) {
    for (int iter=0;iter<maxiter;iter++) {
      int status=gsl_multimin_fminimizer_iterate(s);
      if (status) throw gsl_exception("error in minimise",status);
      if (test_size(eps)) {
	std::vector<double> x(np);
	for (int i=0;i<np;i++) x[i] = gsl_vector_get(s->x,i);
	return x;
      }
    }
    throw gsl_exception("too many iterations in gsl_minimiser");
  }
  /** test whether the required precision has been met       */
  bool test_size(double eps) {
    double size =  gsl_multimin_fminimizer_size(s);
    int status=gsl_multimin_test_size(size,eps);
    if (status==GSL_SUCCESS) return true;
    return false;
  }
  /** The local function that is called                      */
  /** The actual function to be minimised is passed as the 
   *  parameters of this function - slightly bizarre but the 
   * only way around that I can see                          */
  static double localf(const gsl_vector *x, void *params) {
    gsl_minimiserMD *gm = (gsl_minimiserMD*) params;
    std::vector<double> xx(gm->np);
    for (int i=0;i<gm->np;i++) {
      xx[i] = gsl_vector_get(x,i);
    }
    if (gm->find_min) return gm->f_(xx);
    else return -gm->f_(xx);
  }
private:
  T  &f_;
  gsl_multimin_function f;
  const gsl_multimin_fminimizer_type *mt_;
  gsl_multimin_fminimizer *s;
  bool find_min;
  int np;
  gsl_vector *ss;
  gsl_vector *st;
};

/** function to bracket the maximum of a function on x in [minval,maxval] 
 * If the maximum is at either end then false is returned (with x at the maximum value)
 otherwise true is returned.  Assumed to only have one maximum                     */
template<class T>
bool bracket_maximum0(T &f,double &x, double &fx, double &l,double &fl
		      , double &u, double &fu,double minval=0.0, double maxval=1E7)
{
  fx=f(x);
  u=std::min(maxval,x*2.0);
  fu=f(u);
  if (fu<fx) { // maximum must be less than u 
    l=minval;
    fl=f(minval);
    if (fl>fx) { // maximum could be minval and must be < x
      u=x;fu=fx;
      x=minval+1E-5;fx=f(x);
      if (fx>fl) { 
	return true;
      } else {
	x=l;
	fx=fl;
	return false;
      }
    } else { // root in the interval
      return true;
    }
  } else { // maximum must be > x
    if (fabs(x-maxval)<1E-7) return false; // at maxval already
    l=x;fl=fx;
    x=u;fx=fu;
    u=std::min(maxval,x*2.0);fu=f(u);
    while(fu>fx&&u<maxval) {
      l=x;fl=fx;
      x=u;fx=fu;
      u=std::min(maxval,x*2.0);fu=f(u);
    }  
    if (u>=maxval) {
      x=u;
      fx=fu;
      return false;
    }
    return true;
  }
}

#endif
