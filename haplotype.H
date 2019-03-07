//  @file   Time-stamp: <2012-01-18 12:15:48 nijw>
#ifndef HAPLOTYPE_H__
#define HAPLOTYPE_H__

#include <tnt/tnt.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <map>
#include "GenEnums.H"
#include "mapfreq.H"
#include "utilcpp/newio.H"

/** a class to hold haplotype information
 * Note that this is a vector of char pointers 
 * This is also one of the few places where memory is 
 * allocated in the whole haplotype structure        */
class HaplotypeArray: public TNT::Array2D<BiHap> {
public:
  HaplotypeArray(size_t nhaps, size_t loci,const std::vector<int> &pos);
  /** constructor to allocate the memory                                 */
  HaplotypeArray(size_t nhaps, size_t loci);
  HaplotypeArray(size_t nhaps, size_t loci, const char *filename);
  HaplotypeArray(const char *filename) {
    std::ifstream inf(filename);
    inf >> *this;
  }

  ~HaplotypeArray() {
  }

  HaplotypeArray(std::istream &in) {
    in >> *this;
    position = readvector(in,0);
  }

  void addtriohaps(int startindividual,int locus, const BiHap haps[4]);

  std::ostream &print(std::ostream &o) const  {
    return o << *this;
  }

  void hotspotprint(const std::string &fn, const int *pos=0);

  size_t loci() const {
    return dim2();
  } 
  size_t sites() const {
    return dim2();
  }

  size_t nhaps() const {
    return dim1();
  }
  size_t n() const {
    return dim1();
  }

  void write(const char *filename,bool writeposition=false) {
    std::ofstream out(filename);
    out << *this;
    if (writeposition) {
      out << position.size() << std::endl;
      printvector(out,position);
    }
    out.close();
  }
  void writeposition(const char *filename) {
    std::ofstream out(filename);
    assert(position.size()!=0);
    std::copy(position.begin(),position.end(),std::ostream_iterator<long>(out,"\n"));
    out.close();
  }

  BiHap val(int i, int j) const {
    return this->operator[](i)[j];
  }
  BiHap &val(int i, int j)  {
    return this->operator[](i)[j];
  }
  /** frequency spectrum of the array made from positions [start,start+;length) */
  mapfreq<std::string>  getmapfreq(int start, int length);
  /** vector of the array made from positions [start,start+;length)       */
  std::vector<std::string>  tostring(int start, int length);
  /** frequency spectrum of the whole haplotype                           */
  mapfreq<std::string>  getmapfreq();
  /** data                                                                */
  std::vector<int> position;
};

class PopHaplotypeArray: public HaplotypeArray {
public:
  PopHaplotypeArray(std::istream &in)
    :HaplotypeArray(in) {
    in >> population;
  }
  PopHaplotypeArray(const HaplotypeArray &a1, const HaplotypeArray &a2)
    :HaplotypeArray(a1.nhaps()+a2.nhaps(),a1.loci(),a1.position)
    ,population(a1.nhaps()+a2.nhaps())
  {
    if (a1.loci()!=a2.loci()) 
      throw std::domain_error("can't add together arrays with different"
			      " loci in PopHaplotypeArray");
    for (size_t i=0;i<a1.nhaps();i++) {
      population[i]=0;
      for (size_t j=0;j<a1.loci();j++) 
	val(i,j)=a1[i][j];
    }
    for (size_t i=0;i<a2.nhaps();i++) {
      population[a1.nhaps()+i]=1;
      for (size_t j=0;j<a2.loci();j++) 
	val(a1.nhaps()+i,j)=a2[i][j];
    }
  }
  /** and the destructor                                                  */
  ~PopHaplotypeArray() {
  }
  /** and output        r                                                 */
  void write(const char *filename) {
    std::ofstream out(filename);
    out << *this;
    out << position.size() << std::endl;
    printvector(out,position,"\n");
    out << population;
    out.close();
  }
  /** additional data  */
  TNT::Array1D<int> population;
};

std::ostream &operator<<(std::ostream &o, const HaplotypeArray &ha);
HaplotypeArray  operator+(const HaplotypeArray &a, const HaplotypeArray &b);


#endif
