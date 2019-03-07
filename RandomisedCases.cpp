#include "RandomisedCases.H"
#include "newio.H"


randomisedCases::randomisedCases(rng &r, int ncases, int ntotal, int replicates, bool Diploids)
  :xx(replicates) {
  if (Diploids==true) {
    std::vector<int> AllDiploids(ntotal);
    for (int i=0;i<ntotal;i++) AllDiploids[i]=i;
    for (int i=0;i<replicates;i++) {
      permute(AllDiploids,r);
      std::sort(AllDiploids.begin(),AllDiploids.begin()+ncases);
      for (int j=0;j<ncases;j++) {
        xx[i].push_back(2*AllDiploids[j]);
        xx[i].push_back(2*AllDiploids[j]+1);
      }
    }
  } else {
    std::vector<int> All(ntotal);
    for (int i=0;i<ntotal;i++) All[i]=i;
    for (int i=0;i<replicates;i++) {
      permute(All,r);
      std::sort(All.begin(),All.begin()+ncases);
      for (int j=0;j<ncases;j++) {
        xx[i].push_back(All[j]);
      }
    }
  }
}
/** constructor for regional randomised cases. 
 * In this class the randomisation is by region, so that the cases and controls are
 * mixed within each region rather than all pooled together      */                                                                                    
regionalRandomisedCases::regionalRandomisedCases(rng &r, const std::vector<int> &region
						 , const TNT::Array2D<int> &CaseStatus, int CaseType
						 ,int trait, int replicates, bool Diploids)
  :randomisedCases(replicates)
 
{
  int MaxRegion = *std::max_element(region.begin(),region.end());
  std::vector<std::vector<int> > RegionalVector(MaxRegion);
  std::vector<int> ncases(MaxRegion,0);
 
  if (Diploids==true) {
    // assume that regions and cases are counted in pairs
    for (size_t i=0;i<region.size();i+=2) {
      RegionalVector[region[i]-1].push_back(i);
      if (CaseStatus[trait][i]==CaseType)  ncases[region[i]-1]++;
    }
    //   printvector(std::cerr,ncases);
    //   std::cerr << std::endl << std::accumulate(ncases.begin(),ncases.end(),0) << std::endl;
    for (int i=0;i<replicates;i++) {
      for (int j=0;j<MaxRegion;j++) {
        permute(RegionalVector[j],r);
        //   std::sort(RegionalVector[j].begin(),RegionalVector[j].begin()+ncases[j]);
        for (int k=0;k<ncases[j];k++) {
          xx[i].push_back(RegionalVector[j][k]);
          xx[i].push_back(RegionalVector[j][k]+1);
        }
      }
      std::sort(xx[i].begin(),xx[i].end());
      assert(std::accumulate(ncases.begin(),ncases.end(),0)==static_cast<int>(xx[i].size())/2);
      //  for (size_t j=1;j<xx[i].size();j++) assert(xx[i][j-1]<xx[i][j]);
    } 
   
  } else {
    for (size_t i=0;i<region.size();i+=2) {
      RegionalVector[region[i]-1].push_back(i);
      if (CaseStatus[trait][i]==CaseType)  ncases[region[i]-1]++;
    }
    for (int i=0;i<replicates;i++) {
      for (int j=0;j<MaxRegion;j++) {
        permute(RegionalVector[j],r);
        std::sort(RegionalVector[j].begin(),RegionalVector[j].begin()+ncases[j]);
        for (int k=0;k<ncases[j];k++) {
          xx[i].push_back(RegionalVector[j][k]);
        }
      }
    } 
  }
}
