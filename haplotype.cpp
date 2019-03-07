#include "haplotype.H"
#include <stdexcept>


HaplotypeArray::HaplotypeArray(size_t nhaps, size_t loci,const std::vector<int> &pos)
  :TNT::Array2D<BiHap>(nhaps,loci),position(pos)
{
  
}


HaplotypeArray::HaplotypeArray(size_t nhaps, size_t loci)
  :TNT::Array2D<BiHap>(nhaps,loci)
{
  
}

/** Add haplotypes from a parent-offspring trio                                      */
void HaplotypeArray::addtriohaps(int startindividual,int locus, const BiHap haps[4])
{
  for (int i=0;i<4;i++) 
    operator[](startindividual+i)[locus]=haps[i];
  // *this[startindividual+i][locus]=haps[i];
}

/** print out the results                                                            */
std::ostream &operator<<(std::ostream &o, const HaplotypeArray &ha)
{
  return ha.print(o);
} 
/** join two haplotype arrays together by row                                        */
HaplotypeArray  operator+(const HaplotypeArray &a, const HaplotypeArray &b)
{
  if (a.loci()!=b.loci())
    throw std::range_error("error, two haplotype arrays have different numebrs of rows");
  
  HaplotypeArray tmp(a.nhaps()+b.nhaps(),a.loci(),a.position);
  int pos=0;
  for (size_t i=0;i<a.nhaps();i++) {
    BiHap *curr=tmp[pos];
      for (size_t j=0;j<a.loci();j++) curr[j]=a[i][j];
    pos +=1;
  }
  for (size_t i=0;i<b.nhaps();i++) {
    BiHap *curr=tmp[pos];
      for (size_t j=0;j<b.loci();j++) curr[j]=b[i][j];
    pos +=1;
  }
  return tmp;
}
/** read in a haplotypearray from a file -- this is built for simulation
 * results and has no error checking at all                              */
HaplotypeArray::HaplotypeArray(size_t nhaps, size_t loci, const char *filename)
  :TNT::Array2D<BiHap>(nhaps,loci)
{
   int M=dim1();
   int N=dim2();
   std::ifstream in(filename);
   for (int i=0; i<M; i++) {
     for (int j=0; j<N; j++)  {
       in >> this->operator[](i)[j] ;
     }
   }
}
void HaplotypeArray::hotspotprint(const std::string &fn, const int *pos) {
    std::ofstream o(fn.c_str());
    o << nhaps() << " " << loci() << std::endl;
    if (pos!=0) {
      for (size_t i=0;i<loci();i++) 
	o << pos[i] << " ";
    } else {
      for (size_t i=0;i<loci();i++) 
	o << i << " ";
    }
    o << std::endl;
    for (size_t i=0;i<nhaps();i++) {
      o << "1 ";
      for (size_t j=0;j<loci();j++) {
	o << this->operator[](i)[j];
      }
      o << std::endl;
    }
  }

/** frequency spectrum of the array made from positions [start,start+;length) */
mapfreq<std::string> HaplotypeArray::getmapfreq(int start, int length) 
{
  mapfreq<std::string> hf;
  for (size_t i=0;i<nhaps();i++) {
    std::ostringstream oss;
    for (int j=0;j<length;j++) oss << val(i,start+j);
    hf.add(oss.str());
  }
  return hf;
}
/** frequency spectrum of the whole haplotype                           */
mapfreq<std::string>  HaplotypeArray::getmapfreq() 
{
  mapfreq<std::string> hf;
  for (size_t i=0;i<nhaps();i++) {
    std::ostringstream oss;
    for (size_t j=0;j<loci();j++) oss << val(i,j);
    hf.add(oss.str());
  }
  return hf;
}
/** vector of the array made from positions [start,start+;length)       */
std::vector<std::string>  HaplotypeArray::tostring(int start, int length)
{
  std::vector<std::string> a;
  a.reserve(nhaps());
  for (size_t i=0;i<nhaps();i++) {
    std::ostringstream oss;
    for (int j=0;j<length;j++) oss << val(i,start+j);
    a.push_back(oss.str());
  }
  return a;
}
