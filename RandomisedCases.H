#ifndef RANDOMISED_CASES_H__
#define RANDOMISED_CASES_H__

#include "tnt/tnt.h"


#include "gsl_rand.H"

/** Class to get the indices (0 offset ) for a set of 
 * randomised cases.    */
class randomisedCases {

public:
  randomisedCases(rng &r, int ncases, int ntotal, int replicates, bool Diploids=true);

  randomisedCases(int replicates):xx(replicates) {
  };

  const std::vector<int> &operator[](int i) const {
    return xx[i];
  }
  /** Return the i-th randomised case                    */
  const std::vector<int> &at(int i) const {
    return xx[i];
  }

protected:
  /** Hold a vector of vectors to the randomised cases    */
  std::vector<std::vector<int> > xx;

};
/** A derived class to do this in a regional way */
class regionalRandomisedCases:public randomisedCases {

public:
  regionalRandomisedCases(rng &r, const std::vector<int> &region
			  , const TNT::Array2D<int> &CaseStatus, int CaseType, int trait,
			  int replicates, bool Diploids=true);

};


#endif
