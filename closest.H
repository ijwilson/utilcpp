#ifndef CLOSEST_H_
#define CLOSEST_H_

/**  a utility class to get the indices of the closest SNPs to a position
 * without messing about with an extra vector and gsl functions  */
#include "gsl_sort.H"
#include <iostream>

class closest { 
public:
  closest(const std::vector<int> &pos, int startPos)
    :p(pos.size()) {
    std::vector<double> distance(pos.size());
    for (size_t i=0;i<pos.size();i++) {
      distance[i] = static_cast<double>(abs(pos[i]-startPos));
    }
    gsl_sort_index(&p[0],&distance[0],1,distance.size());
  }
  int operator[](int w) const {
    return p[w];
  }
  int size() const {
    return static_cast<int>(p.size());
  }
  std::ostream &print(std::ostream &o) {
    for (size_t ii=0;ii<p.size();ii++) o << p[ii] << " ";
    o << std::endl;
    return o;
  }
  
private:
  std::vector<size_t> p;  
};

#endif
