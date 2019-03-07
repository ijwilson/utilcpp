/// @file
#ifndef GEN_ENUMS_IJW_H__
#define GEN_ENUMS_IJW_H__

#define MAX_BITSET 1024 

#include <iostream>
#include <stdexcept>
#include <vector>

enum GenoBase {
  NN=0,
  AA=2,
  GG=4,
  CC=6,
  TT=8,
  AG=3,
  AC=5,
  CG=7,
  AT=9,
  CT=11,
  GT=13
};

enum Base {
  A=0,
  G=1,
  C=2,
  T=3
};

enum BiHap {
  zero=0,
  one=1,
  hetquad=2,
  hetpair=3,
  eithertrans=4,
  hettrip=5,
  N=6
};
bool phased(BiHap a);
void swtch(BiHap *a);



enum BiGeno {
  Hom0=0,
  Het=1,
  Hom1=2,
  miss=3
};

// class QuadCounter {
// public:
//   const BiHap *operator()() {
//     return &hap[0];
//   }
//   QuadCounter(const BiHap *b1, const BiHap *b2, const BiHap *c1, const BiHap *c2, int l);
//   const BiHap *operator++() {
//     if (pos==1U<<missing.size()-1) return 0;
//     pos++;
//     update();
//     return  &hap[0];
//   }
//   std::vector<int> missing;
//   unsigned long pos;
//   std::vector<BiHap> hap;
// private:
//   void update();
//   int len;
// };

class HapCounter {
public:
  const BiHap *operator()() {
    return &hap[0];
  }
  HapCounter(const BiHap *b, int l);
  const BiHap *operator++() {
    pos++;
    update();
    return  &hap[0];
  }
  std::vector<int> missing;
  unsigned long pos;
  std::vector<BiHap> hap;
private:
  void update();
};


BiGeno changetobiallele(GenoBase gen, Base allele1);

std::ostream &operator<<(std::ostream &o, GenoBase gg);
std::ostream &operator<<(std::ostream &o, BiGeno bg);
std::ostream &operator<<(std::ostream &o, const Base &b);
std::ostream &operator<<(std::ostream &o, const BiHap &b);
std::istream& operator>>(std::istream &s, BiHap &a);


template <typename T>
T flip(const T &a) 
{
  if (a==0) return 1;
  return 0;
}

template<>
BiHap flip(const BiHap &a);

#endif
