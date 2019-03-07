#include <cassert>
#include <vector>
#include <deque>
#include <utility>
#include <cmath>
#include <algorithm>
#include "util.H"
#include "gsl_rand.H"
#include "utilityfunctionals.H"

//
// The global random number generator
//
// acts as a default for all random number generation.
//
//
#ifdef USE_R
rng globalrandom();
#else
rng globalrandom(1);
#endif  
//
// some global utility functions
//
void sample2int(int wh[2],int from, int to, rng &g)
{
	double len;
	len=(double)(to-from+1);
	wh[0] = from + (int)(g.next()*len);	
	wh[1] = from + (int)(g.next()*(len-1.));
	if (wh[1]>=wh[0]) wh[1]++;
	
	return;
}
//
// 
//
std::pair<int,int> sample2int(int from, int to, rng &g)
{
	double len;
	std::pair<int,int> p;
	len=(double)(to-from+1);
	p.first = from + (int)(g.next()*len);	
	p.second = from + (int)(g.next()*(len-1.));
	if (p.second>=p.first) p.second++;
	assert(p.first>=from&&p.first<=to);
	assert(p.second>=from&&p.second<=to);
	return p;
}
/** Sample a sorted pair of integers in [from,to] with first>second */
std::pair<int,int> sample2intsorted(int from, int to, rng &g)
{
  double len;
  std::pair<int,int> p;
  len=(double)(to-from+1);
  p.second = from + (int)(g.next()*len);	
  p.first = from + (int)(g.next()*(len-1.));
  if (p.first>=p.second) {
    p.first++;
  } else {
    int tmp=p.second;
    p.second=p.first;
    p.first=tmp;
  }
  assert(p.first>=from&&p.first<=to);
  assert(p.second>=from&&p.second<=to);
  assert(p.first>p.second);
  return p;
}
//
//
//
int  gen_from_p(const double *p,int siz, rng &r)
{
  std::vector<double> cprob(siz);
  std::transform(p,p+siz,cprob.begin(),cumsum<double>());
  assert(cprob.back()>0.0);
  return gen_from_cump(cprob,r);
}
//
//
//
/*******************************************************************/
int gen_from_cp(double *cp, int len, rng &r)
// a routine to pick a number from 0 to n-1 based on cumulative
// probabilities in p    
{
  double relprob = r();
  int where = (int)relprob*(len); // approximate start
  relprob*=cp[len-1];
  for (;;) {
    if  (relprob <= cp[where]) {
      if (where==0) return 0;
      if (relprob > cp[where-1]) return where;
      else where--;
    } else {
      where++;
      assert(where < len);
      if (where==len-1) return where;
    }
  }
}
//
// log of the normal distribution p.d.f
//
double ldnorm(double x, double mu, double sigma) 
{
  double lx = (x-mu)/sigma;
  return LMY_SQRT_2PI - log(sigma) - 0.5*lx*lx;
}
///////////////////////////////////////////////////////////////////
std::vector<int> integer_permutations(int k, int n,rng ran)
{
  std::vector<int> u(n);
  for (int i=0;i<n;i++) u[i]=i;
  for (int i=0;i<k;i++) {
    int ri=ran.rint(n);
    int tmp=u[i];
    u[i]=u[ri];
    u[ri]=tmp;
  }
  u.resize(k);
  sort(u.begin(),u.end());
  return u;
}
