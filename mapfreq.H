/// @file
#ifndef MAP_FREQ_H__
#define MAP_FREQ_H__

#include <map>
#include <iostream>
#include <iomanip>   // for setw
#include "newio.H"

/// hold frequences of a factor
template <typename T>
class mapfreq {
public:
  typedef typename std::map<T,int>::iterator itor;
  typedef typename std::map<T,int>::const_iterator const_itor;
  mapfreq(): mf(),n(0) {};
  ~mapfreq(){};
  
  /** This needs to be fixed to use a more efficient method */
  void add(const T &ad) {
    n+=1;
    mf[ad]+=1;
  }
    
  int k() const {
    return mf.size();
  }
  double h() const;
private:
  std::map<T,int> mf;
  int n;
};

template <typename T>
double mapfreq<T>::h() const {
  double hom=0.0; 
  const_itor a=mf.begin();
  while (a != mf.end()) {
    hom += a->second*a->second;
    a++;   
  }
  return hom/(double(n)*double(n));
}


template <typename T> 
class mappopfreq: public std::vector<std::vector<int> > {
public:
  typedef typename std::map<T,int>::iterator itor;
  typedef typename std::map<T,int>::const_iterator const_itor;
  typedef typename std::map<T, int>::value_type val_type;
  mappopfreq(int pops)
    :n(0),npops_(pops) {
  }
  /** destructor                            */
  ~mappopfreq(){};
  
  /** add an observation in population pop */ 
  void add(const T &ad, int pop) {
    n+=1;
    itor fi=rownames.find(ad);
    if (fi==rownames.end()) { // not here: add
      std::vector<int> addv(npops_,0);
      addv[pop]=1;
      push_back(addv);
      rownames.insert(fi,val_type(ad,size()-1));
    } else {
      this->at(rownames[ad])[pop]+=1;
    }
  }
  /** return the total number of alleles present        */  
  int k() const {
    return rownames.size();
  }
  /** Number of populations                             */
  int npops() const 
  { 
    return npops_;
  }
  /** return the number of alleles present in population pop */
  int alleles(int pop) const {
    int tmp=0;
    for (size_t i=0;i<rownames.size();i++) 
      if (at(i)[pop]>0) tmp+=1;
    return tmp;
  }
  /** return the number of alleles private to population pop */
  int private_alleles(int pop) const {
    int tmp=0;
    for (size_t i=0;i<rownames.size();i++) {
      if (at(i)[pop]>0) {
	int u=true;
	for (int j=0;j<pop;j++) if (at(i)[j]>0) {
	  u=false;
	  break;
	}
	if (u) {
	  for (int j=pop+1;j<npops_;j++) if (at(i)[j]>0) {
	    u=false;
	    break;
	  }
	  if (u) tmp+=1;
	}
      }
    }
    return tmp;
  }
  /** print out a table of frequencies                  */
  std::ostream &print(std::ostream &o)  {
    itor i=rownames.begin();
    while (i!=rownames.end()) {
      o << i->first << " ";
      for (int j=0;j<npops_;j++) 
	o << std::setw(5) << at(i->second)[j];
      o << std::endl;
      i++;
    }
    return o;
  }
private:
  mappopfreq();
  std::map<T,int> rownames;
  int n;
  int npops_;
};


#endif
