/**  @file */
#ifndef GSL_VECTOR_H__
#define GSL_VECTOR_H__

#include <gsl/gsl_vector.h>
#include <vector>

template <typename T>
gsl_vector *to_gsl_vector(const std::vector<T> &x) 
{
  gsl_vector * v = gsl_vector_alloc (x.size());
  
  for (int i = 0; i < x.size(); i++) 
    gsl_vector_set(v, i,x[i]);
  
  return v;
}
  




#endif
