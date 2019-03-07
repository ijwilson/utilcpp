#ifndef READ454_H__ 
#define  READ454_H__  

#include <vector>
#include <string>
#include <map>
#include <iosfwd>
#include <algorithm>

#include "flowGram.H"

/** Read information from an sff file (or from the output of sffinfo) */
class Read454 {
public:
/** constructors and destructors   */
  Read454(const char *file, bool trim=true);
  ~Read454(){};


  /** The number of samples    */
  int n() const {
    return fg_.size();
  }

  /** return a reference to the vector of flowGrams                  */
  const std::vector<flowGram> &fg() {
    return fg_;
  }
  /** return a reference to a single flowGram                        */
  const flowGram &operator[](size_t ii) const {
    return fg_.at(ii);  // use at and you get range checking 
  }

  /** Filter the flowGrams and keep only some                     */
  void filter(double minrun=0.2, int minlength=20) {
    flowGramFilter f(minrun,minlength);
    fg_.erase(std::remove_if(fg_.begin(),fg_.end(),f),fg_.end());
  }

  /** Filter the flowGrams and keep only some                     */
  void maxfilter(double maxIntensity=10.0) {
    flowGramMaxFilter f(maxIntensity);
    fg_.erase(std::remove_if(fg_.begin(),fg_.end(),f),fg_.end());
  }

  std::vector<int> len() const {
    std::vector<int> l;
    l.reserve(n());
    for (int i=0;i<n();i++) l.push_back(fg_[i].length());
    return l;
  }
  
  double maxIntensity() const {
    double mx=*std::max_element(fg_[0].intensity.begin(),fg_[0].intensity.end());
    for (size_t ii=1;ii<fg_.size();ii++) {
      double lmax =  *std::max_element(fg_[ii].intensity.begin(),fg_[ii].intensity.end());
      if (lmax>mx) mx=lmax;
    }
    return mx;
  }

private:
  Read454();
 
  std::vector<flowGram> fg_; // the vector of flowGrams
};





#endif
