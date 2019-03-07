#ifndef MISSINGHAPLO_H__
#define MISSINGHAPLO_H__

#include <set>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include "tnt/tnt.h"

template<typename T>
class missinghaplo {
  typedef  std::set<std::pair<int,int> >::iterator itor;
public:
  /** constructor                                        */
  missinghaplo(TNT::Array2D<T> &d, T mval):data(d),missval(mval) {
    for (int i=0;i<d.dim1();i++) {
      for (int j=0;j<d.dim2();j++) {
	if (data[i][j]==missval) 
	  miss.insert(std::pair<int,int>(i,j));
      }
    }
  }
  /** n */
  int n() const { return data.dim1();}
  /** sites */
  int sites() const {return data.dim2();}
  /** tostring                                            */
  std::string tostring(int ind) const {
    std::ostringstream oss;
    std::copy(data[ind],data[ind]+sites(),std::ostream_iterator<int>(oss,""));
    return oss.str();
  }
  /** reset all the missing values                        */
    void reset() {
    itor i=miss.begin();
    while (i!=miss.end()) {
      data[i->first][i->second]=missval;
      i++;
    }
  }
  /** change                                              */
  void change(int i,int j,T val) {
    if (miss.find(std::pair<int,int>(i,j))!=miss.end()) {
      data[i][j]=val;
    } else {
      std::cerr << "error, can't change non missing type" << std::endl;
    }
  }
  /** values                                              */
  T &operator()(int i,int j) const {
    return data[i][j];
  }
 /** values                                              */
  T &value(int i,int j) const {
    return data[i][j];
  }
  /** can only write to missing values                    */
   T &operator()(int i,int j) {
     if (miss.find(std::pair<int,int>(i,j))==miss.end()) 
       std::cerr << "error, changing non-missing type" << std::endl;
     return data[i][j];
  }
  /** is a particular position (still) missing            */
  bool ismissing(int i,int j) {
    if (data[i][j]==missval) return true;
    return false;
  }
  /** nmissing                  */
  int nmissing() const {
    return miss.size();
  }
  /** print                                               */
  std::ostream &print(std::ostream &o) const {
    o << data;
    return o;
  }
private:
  TNT::Array2D<T> &data;   // only one copy of the data!
  std::set<std::pair<int,int> > miss;
  T missval;
};

template <typename T>
std::ostream &operator<<(std::ostream &o, const missinghaplo<T> &m)
{
  return m.print(o);
}


#endif
