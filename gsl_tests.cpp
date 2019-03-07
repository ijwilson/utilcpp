#include <string>

#include "gsl_distributions.H"
#include "gsl_rand.H"

int main(void) 
{
  rng r(1);
  std::string ds("gamma(5,.5)");

  ctsdistribution *a  = fromstring(ds);
  vector<double> x(500000,2.0);
  vector<double> dd=a->sample(r,500000);
  a->sample(r,x);
 
  std::cout << std::accumulate(dd.begin(),dd.end(),0.0)/500000 << std::endl;
  std::cout << std::accumulate(x.begin(),x.end(),0.0)/500000 << std::endl;
 

  std::string  ps("Poisson(.01)");

  ctsdistribution *poi  = fromstring(ps);

  for (int i=0;i<=10;i++) 
    std::cout << i << " " << exp(poi->log_pdf(static_cast<double>(i))) 
	      << std::endl;

  std::cout << "======================================\n";
  std::string aa="   aaaabbb";
  ltrim(aa);
  std::cout << aa << std::endl;
  
  std::string bbb="Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\n\n"
"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\n";
  
  std::cout << stringwrap(bbb,72,"   ") << std::endl;


  return EXIT_SUCCESS;
}
