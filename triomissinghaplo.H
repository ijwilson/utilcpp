#ifndef TRIOMISSINGHAPLO_H__
#define TRIOMISSINGHAPLO_H__

#include <set>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include "tnt/tnt.h"
#include "GenEnums.H"
#include "haplotype.H"
#include "rocker.H"

/// a class for a rocker switch for looking at phased haplotypes
class setrocker: public rocker<BiHap> {
public:
  setrocker(BiHap *a, BiHap *b, BiHap *c=0, BiHap *d=0)
    :rocker<BiHap>(a,b,c,d),missing_(true) {
  }
  setrocker(BiHap *a)
    :rocker<BiHap>(a),missing_(true) {
  };
  void reset() {
    missing_=true;
  }
  bool missing() const {
    return missing_;
  }
  void set() {
    missing_=false;
  }
private:
  bool missing_;  
  setrocker();  // not used 

};


class triomissinghaplo {
  typedef  std::map<std::pair<int,int>, setrocker * >::iterator itor;
  typedef  std::map<std::pair<int,int>, setrocker * >::const_iterator citor;
  typedef  std::list<setrocker>::iterator blitor;
  
public:
  /** constructor                                        */
  triomissinghaplo(const HaplotypeArray &d, int st, int len);
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
    blitor ii=rock.begin();
    while (ii!=rock.end()) {
      ii->reset();
      ii++;
    }
  }
  /** change                                              */
  void change(int i,int j,const BiHap &val) {
    itor pos=miss.find(std::pair<int,int>(i,j));
    if (pos!=miss.end()) {
      assert(pos->second->missing());
      if (data[i][j]!=val) {
	pos->second->rockswtch();
	assert(data[i][j]==val);
      } 
      pos->second->set();
      assert(pos->second->missing()==false);
    } else {
      std::cerr << "error, can't change non missing type" << std::endl;
    }
  }
//   /** values                                              */
//   const BiHap &operator()(int i,int j) const {
//     return data[i][j];
//   }
  /** values                                              */
  const BiHap &value(int i,int j) const {
    return data[i][j];
  }
  /** is a particular position (still) missing            */
  bool ismissing(int i,int j) const {
    citor ii=miss.find(std::pair<int,int>(i,j));
    if (ii==miss.end()) return false;
    return ii->second->missing();
  }
  /** nmissing                  */
  int nmissing() const {
    return miss.size();
  }
  /** nmissing                  */
  int countmissing() const {
    int count=0;
    for (int i=0;i<n();i++) {
      for (int j=0;j<sites();j++) {
	count += ismissing(i,j);
      }
    }
    return count;
  }
  /** checking functions                                  */
  bool check() {
    /*   std::cout << rock.size() << " switches ";
    std::cout << miss.size() << " missing ";*/
    itor ii=miss.begin();
    while(ii!=miss.end()) {
      //  std::cout << "(" << ii->first.first <<"," << ii->first.second << ") ";
      if (ii->second->missing()) return false;
      ii++;
    }
    return true;
  }
  /** print                                               */
  std::ostream &print(std::ostream &o) const {
    o << data;
    return o;
  }
private:
  HaplotypeArray data;   // only one copy of the data!
  std::map<std::pair<int,int>, setrocker * > miss;
  std::list<setrocker> rock;
};

std::ostream &operator<<(std::ostream &o, const triomissinghaplo &m);

#endif
