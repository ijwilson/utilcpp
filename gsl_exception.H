#ifndef GSL_EXCEPTION_H__
#define GSL_EXCEPTION_H__

#include "gsl/gsl_errno.h"
#include <exception>
#include <stdexcept>

/// error class when wrapping gsl functions
class gsl_exception {
public:
  gsl_exception(const char *errtxt,int errtype):message(errtxt),errortype(errtype) {
  };
public:
  const char *message;
  int errortype;
};





#endif
