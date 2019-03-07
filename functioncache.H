#ifndef FUNCTION_CACHE_H__
#define FUNCTION_CACHE_H__
#include <map>
#include <vector>

template <typename FUNC> 
class cache {
public:
  cache(FUNC &f) :func(f) {};
  ~cache(){}
  double operator()(double x);
private:
  FUNC &func;
};



template <typename FUNC> 
class simplecache: public cache<FUNC> {
public:
  simplecache(FUNC f,double lw, double up, int n)
    :cache<FUNC>(f),bins(n)
    ,v(n,99.9),lower(lw)
    ,upper(up) {
    gap=(upper-lower)/double(bins-1);
  }
  double operator()(double x) {
    if (x<lower||x>upper) return func(x);
    pos = (x-lower)/double(bins-1);
    if (v[pos]==99.9) {
      xmid=lower+(pos+0.5)*gap;
      v[pos]=func(xmid);
    }
    return v[pos];
  }
private:
  double lower,upper,gap;
  int bins;
  std::vector<double> v;
};

template <typename FUNC> 
class logcache: public cache<FUNC> {
public:
  logcache(FUNC f,double lw, double up, int n):
    cache<FUNC>(f),bins(n),v(n,99.9) {
    lower=log(lw);
    upper=log(up);
    gap=(upper-lower)/double(bins-1);
  }
  double operator()(double x) {
    lx=log(x);
    if (lx<lower||lx>upper) return func(x); 
    pos = (lx-lower)/double(bins-1);
    if (v[pos]==99.9) {
      xmid=lower+(pos+0.5)*gap;
      v[pos]=func(xmid);
    }
    return v[pos];
  }

private:
  double lower,upper,gap;
  int bins;
  std::vector<double> v;
};


 
// template <class T> 
// class autocache {
//   std::map<double, double>::iterator ci;
// public:
//   autocache(double maxdiffx=1E-5,double maxerr=1E-3);
//   double operator()(double x) {
//     // find the lower and upper bounds
//     ci lwb = ch.lower_bound(x);
//     if (lwb==ch.end()) { // before the first value
//       // is it close to the first value?
//       if (ch.begin()->first-x<maxdiffx) return ch.begin()->second; 
//       // otherwise return a new value
//       double f=func(x);
//       ch.insert(std::pair<double,double>(x,f));
//       return f;
//     } 
//     ci upb = ch.upper_bound(x);
//     if (upb == ch.end()) { //above the max value
//        // is it close to the last value?
//       upb--;
//       if (x-upb->first<maxdiffx) return upb->second; 
//       // otherwise return a new value
//       double f=func(x);
//       ch.insert(ch.end(),std::pair<double,double>(x,f));
//       return f;
//     }
//     // is it needed ?
    


//     // calculate a new value 
    
//   };


// private:
//   std::map<double,double> ch;
//   T &func;
// };



#endif
