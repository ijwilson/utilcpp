#include "GenEnums.H"
#include <bitset>
#include <cassert>

std::ostream &operator<<(std::ostream &o, GenoBase gg)
{
  switch(gg) {
  case NN: 
    o << "NN";return o;
  case AA:
    o << "AA";return o;
  case GG:
    o << "GG";return o;
  case TT:
    o << "TT";return o;
  case CC:
    o << "CC";return o;
  case AG:
    o << "AG";return o;
  case AC:
    o << "AC";return o;
  case CG:
    o << "CG";return o;
  case AT:
    o << "AT";return o;
  case CT:
    o << "CT";return o;
  case GT:
    o << "CT";return o;
  default:
    o << "Error: " << int(gg) << " this base not defined";
    exit(1);
    return o;
  }
}
    
std::ostream &operator<<(std::ostream &o, BiGeno bg)
{
  switch(bg) {
  case miss: 
    o << "missing";return o;
  case Hom0:
    o << "00";return o;
  case Het:
    o << "01";return o;
  case Hom1:
    o << "11";return o;
  default:
    o << "Error: " << int(bg) << " this base not defined";
    exit(1);
    return o;
  }
}

BiGeno changetobiallele(GenoBase gen, Base allele1)
{
  if (gen==NN) return miss;
  switch(allele1) {
  case A: // "A"
    switch(gen) {
    case AA: 
	return Hom1;
    case AC: case AG: case AT:
      return Het;
    default:
      return Hom0;
    }
  case G: // "G"
    switch(gen) {
    case GG:
      return Hom1;
    case AG: case CG: case GT:
      return Het;
    default:
      return Hom0;
    }
  case T: // "T"
    switch(gen) {
    case TT:
      return Hom1;
    case AT: case CT: case GT:
      return Het;
    default:
      return Hom0;
    }
  default:
    switch(gen) {
    case CC:
      return Hom1;
    case AC: case CG: case CT:
      return Het;
    default:
      return Hom0;
    }
  }
}
std::ostream &operator<<(std::ostream &o, const Base &b)
{
  switch (b) {
  case A: o << "A"; break;
  case G: o << "G"; break;
  case C: o << "C"; break;
  case T: o << "T"; break;
  default:
    std::cout << "Error Base " << (int)b <<  " not known";
    exit(EXIT_FAILURE);
  }
  return o;
}
std::ostream &operator<<(std::ostream &o, const BiHap &b)
{
  return o << int(b);
}
std::istream& operator>>(std::istream &s, BiHap &a)
{
  int ai;
  s >> ai;
  a=BiHap(ai);
  return s;
}

void swtch(BiHap *a) 
{
  if (*a==zero) *a=one;
  else *a=zero;
}


HapCounter::HapCounter(const BiHap *b, int l)
  :pos(0),hap(l)
{
  for (int i=0;i<l;i++) {
    if (b[i]==N   ) {
      missing.push_back(i);
      hap[i]=zero;
    } else hap[i]=b[i];
  }
  if (missing.size()>MAX_BITSET) 
    throw std::overflow_error("length too long for hapcounter");    
};




void HapCounter::update() {
  std::bitset<MAX_BITSET> bpos(pos);
  for (size_t i=0;i<missing.size();i++) {
    hap[missing[i]] = BiHap(int(bpos[1024-i]));  
  }
}
/** is this BiHap phased ? - should be inline   */
bool phased(BiHap a)
{
  if (a==zero||a==one) return true;
  return false;
}


void QuadCounter::update() {
  std::bitset<MAX_BITSET> bpos(pos);
  for (size_t i=0;i<missing.size();i++) {
    int ch=int(bpos[1024-i]);
    hap[missing[i]] =  hap[3*len+missing[i]]=BiHap(ch);
    hap[len+missing[i]]= hap[2*len+missing[i]]=BiHap(1-ch);
  }
}

QuadCounter::QuadCounter(const BiHap *b1, const BiHap *b2, const BiHap *c1, const BiHap *c2, int l)
  :pos(0),hap(4*l),len(l)
{
  //  int count=0;
  for (int i=0;i<len;i++) {
    if (b1[i]==hetquad) {
      assert(b2[i]==hetquad&&c1[i]==hetquad&&c2[i]==hetquad);
      missing.push_back(i);
      hap[i]=zero;
      hap[len+i]=one;
      hap[2*len+i]=one;
      hap[3*len+i]=zero;
    } else {
      hap[i]=b1[i];
      hap[len+i]=b2[i];
      hap[2*len+i]=c1[i];
      hap[3*len+i]=c2[i];
    }
  }
  if (missing.size()>MAX_BITSET) 
    throw std::overflow_error("length too long for hapcounter");    
};


template<>
BiHap flip(const BiHap &a)
{
  if (a==one) return zero;
  else if (a==zero) return one;
  throw std::domain_error("expect zero or one for BiHap!");
}
