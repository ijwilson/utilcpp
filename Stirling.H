#ifndef STIRLING__H__
#define STIRLING__H__


#include <stdexcept>

class Stirling {
public:
  double operator()(int n, int k) {
    if (n>nmax)
      throw std::range_error("error, n too large in Stirling");
    if (k>n) std::range_error("error, k>n  in Stirling");
    return  mat[(n-1)+nmax*(k-1)];
  }
  ~Stirling() {
    delete[] mat;
  }

  Stirling(int n);
private:
  int nmax;  
  double *mat;
};

#endif
