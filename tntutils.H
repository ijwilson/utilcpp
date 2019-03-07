// @file Time-stamp: <2007-07-17 15:29:30 dijw>
#ifndef TNT_UTILS_H__
#define TNT_UTILS_H__
#include <map>
#include <vector>
#include <iosfwd>
#include <fstream>
#include "tnt/tnt.h"
#include "newio.H"
/** A vertical slice through the array                      */
template <typename T>
TNT::Array2D<T> slice(const TNT::Array2D<T> &a, int from, int length) {
  TNT::Array2D<T> r(a.dim1(),length);
  for (int i=0;i<a.dim1();i++) {
    for (int j=0;j<length;j++) {
      r[i][j]=a[i][from+j];
    }
  }
  return r;
}


/** transoform a vector of vectors (and transpose)                  */
template <typename T>
TNT::Array2D<T> transform(const std::vector<std::vector<T> > &a, bool transpose=true) {
  
  if (transpose) {
    TNT::Array2D<T> x(a.size(),a[0].size(),0);
  
    for (size_t i=0;i<a.size();i++) {
      for (size_t j=0;j<a[i].size();j++) {
	x[i][j]=a[i][j];
      }
    }
    return x;
  } else {
  TNT::Array2D<T> x(a[0].size(),a.size(),0);
  
  for (size_t i=0;i<a.size();i++) {
    for (size_t j=0;j<a[i].size();j++) {
      x[j][i]=a[i][j];
    }
  }
  return x;
  }
} 

/** A slice from a 1D Array                                */
template <typename T>
TNT::Array1D<T> slice(const TNT::Array1D<T> &a, int from, int length) {
  TNT::Array1D<T> r(length);
  for (int j=0;j<length;j++) r[j]=a[from+j];
  return r;
} 

/** A slice from a vector                                  */
template <typename T>
std::vector<T> slice(const std::vector<T> &a, int from, int length) {
  std::vector<T> r(length);
  for (int j=0;j<length;j++) r[j]=a[from+j];
  return r;
} 

template <typename T>
TNT::Array1D<double> colsum(const TNT::Array2D<T> &a) 
{
  TNT::Array1D<double> r(a.dim2(),0.0);
  for (int i=0;i<a.dim1();i++) {
    for (int j=0;j<a.dim2();j++) {
      r[j] += a[i][j];
    }
  }
  return r;
}
template<typename T>
TNT::Array1D<int> factor(const TNT::Array1D<T> &v,int &nv)
{
  std::map<T,int> lmap;
  TNT::Array1D<int> res(v.dim());
  nv=0;
  for (int i=0;i<v.dim();i++) {
    if (lmap.find(v[i])==lmap.end()) {
      lmap[v[i]]=nv++;
      res[i]=nv-1;
    } else {
      res[i]=lmap[v[i]];
    }
  }
  return res;
}

template<typename T>
TNT::Array2D<T> readcharintegermatrixTNT(const char *filename)
{
    std::ifstream in(filename);
    if (!in) {
      std::cerr << "Could not open file " << filename << 
	" exiting" << std::endl;
      exit( EXIT_FAILURE);
    }
    int n;
    std::vector<std::vector<T> > im=readcharintmat(in,n,true);
    return transform(im,true);
} 


template<typename T>
TNT::Array1D<T> readvecTNT(const char *filename)
{
    std::ifstream in(filename);
    
    std::vector<T>  im=readintvector(in,true);
    TNT::Array1D<T> a(im.size());
    for (size_t i=0;i<im.size();i++) a[i]=im[i];
    return a;
} 



#endif
