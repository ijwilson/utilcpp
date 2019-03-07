#ifndef GSL_QUANTSILES_IJW___H_
#define GSL_QUANTSILES_IJW___H_

#include <gsl/gsl_statistics.h>
#include <gsl/gsl_sort.h>
#include "tnt/tnt.h"

template <typename T>
TNT::Array1D<double> gsl_quantiles(T *vals, int n,TNT::Array1D<double> probs)
{
  TNT::Array1D<double> res(probs.dim());
  gsl_sort (vals,1,n);
  
  for (int i=0;i<probs.dim();i++) {
    res[i] = gsl_stats_quantile_from_sorted_data(vals,1,n,probs[i]);
  }
  return res;
}

#endif
