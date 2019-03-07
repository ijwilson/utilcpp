#ifndef ROCKER_H_
#define ROCKER_H_

#include <list>
#include <deque>
#include "utilcpp/gsl_rand.H"

/** A class to provide an abstraction to a switch (rocker switch)
 * that may be on or off and may have side effects on other objects 
 * Note that the class T must provide a swtch method.             */ 
template <class T> 
class rocker {
public:
  /** constructor for 2, 3 and 4 switches  */
  rocker(T *a, T *b, T *c=0, T *d=0) {
    Effects.push_back(a);
    Effects.push_back(b);
    if (c!=0) {
      Effects.push_back(c); 
      if (d !=0) Effects.push_back(d);
    }
  }
  rocker(T *a) {
    Effects.push_back(a);
   };
  void rockswtch() {
    typename std::list<T *>::iterator i=Effects.begin();
    while (i!=Effects.end()) {
      swtch(*i);
      i++;
    }
  };
  void set(bool a=true) {
    if (*Effects.front()!=a) rockswtch();
  }
  bool value() {
    return bool(*(Effects.front()));
  } 
private:
  std::list<T*> Effects;
};


#include <bitset>
#include <vector>
using std::bitset;
/** A class to hold the rockerposition using vector of bitsets  */
class rockerposition {
public:
  rockerposition(int n): b_(1+n/32,0L) {
  };
  void flip(int l){
    b_[l/32].flip(l%32);
  };
  bool position(int wh) const {
    return  b_[wh/32][wh%32];
  };
private:
  std::vector<bitset<32> > b_;
};

#endif
