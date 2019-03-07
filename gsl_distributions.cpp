#include <string>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include "newio.H"
#include "gsl_distributions.H"
template <typename T> T readparfrombrackets(std::istream &in) ;

/** read a pair of numbers from brackets         */
template <typename T1,typename T2>
std::pair<T1,T2> readpairfrombrackets(std::istream &in)
{
  std::pair<T1,T2> a;
  int ch = skipblank(in);

  if (ch != '(')
    throw std::domain_error("expected a '(' in readpairfrombrackets");
  in >> a.first;

  ch = skipblank(in);
  if (ch != ',')
    throw std::domain_error("expected a ',' in readpairfrombrackets");
  in >> a.second;

  ch = skipblank(in);
  if (ch != ')')
    throw std::domain_error("expected a ')' to close brackets in readpairfrombrackets");

  return a;
}

template <typename T>
T readparfrombrackets(std::istream &in)
{
  T a;

  int ch = skipblank(in);
 

  if (ch != '(')
    throw std::domain_error("expected a '(' in readparfrombrackets");
  in >> a;
  
  ch = skipblank(in);

  if (ch != ')')
    throw std::domain_error("expected a ')' to close brackets in readparfrombrackets");

  return a;
}

template <typename T>
std::vector<T> readparsfrombrackets(std::istream &in)
{
  std::vector<T> a;

  int ch = skipblank(in);


  if (ch != '(')
    throw std::domain_error("expected a '(' in readparfrombrackets");

  for (;;) {
    T tmp;
    in >> tmp;
    a.push_back(tmp);
    ch = skipblank(in);
      if (ch==')') break;
    if (ch==EOF)
      throw std::domain_error("expected a ')' to finish in readparsfrombrackets");
  }
  return a;
}
/**
 *   "virutal constructor" for distributions
 */
ctsdistribution *ctsdistributionparse(std::istream &is)
{
  is.clear();
  int ch = skipblank(is);

  if (ch==EOF)
    throw std::domain_error("no data in stream while parsing ctsdistribution");

  std::string name;

  if (isdigit(ch)) {  // reading a constant
    is.putback(ch);
    double x;
    is >> x;
    return new Constant(x);
  }

  is.putback(ch);
  for (;;) {
    ch = is.get();
    if (ch=='('||ch==EOF) break;
    name.push_back(ch);
  }

  is.putback(ch);
   if (name=="Improperuniform"||name=="improperuniform") {
    return new improperuniform();
  } else if (name=="positiveuniform"||name =="Positiveuniform") {
    return new improperuniform();
  } else if (name=="normal"||name=="Normal") {
    std::pair<double,double> a = readpairfrombrackets<double,double>(is);
    return new normal(a.first,a.second);
  }
  else if (name=="lognormal"||name=="Lognormal"||name=="LogNormal") {
    std::pair<double,double> a = readpairfrombrackets<double,double>(is);
    return new lognormal(a.first,a.second);
  }  else if (name=="Constant"||name=="constant") {
    double a = readparfrombrackets<double>(is);
    return new Constant(a);
  } else if (name=="Gamma"||name=="gamma") {
    std::pair<double,double> a = readpairfrombrackets<double,double>(is);
#ifdef OLDGAMMA
    a.second=1./a.second;
#endif
    return new Gamma(a.first,a.second);
  } else if (name=="Uniform"||name=="uniform")
    if (ch==EOF) {
        return new improperuniform();
        } else {
    std::pair<double,double> a = readpairfrombrackets<double,double>(is);
    return new uniform(a.first,a.second);
  } else if (name=="Beta"||name=="beta") {
    std::pair<double,double> a = readpairfrombrackets<double,double>(is);
    return new Beta(a.first,a.second);
  } else if (name=="Poisson"||name=="poisson") {
    double mu = readparfrombrackets<double>(is);
    return new Poisson(mu);
  } else if (name=="Geometric"||name=="geometric") {
    double p = readparfrombrackets<double>(is);
    return new Geometric(p);
  } else {
    std::ostringstream oss;
    oss << name << " not known as a distribution name in ctsdistributionparse" << std::endl;
    throw std::domain_error(oss.str().c_str());
  }
}

ctsdistribution *fromstring(const std::string &s)
{
  if (s=="") return 0;
   std::istringstream a(s);
  
   return ctsdistributionparse(a);
}
mvctsdistribution *fromstringmv(const std::string &s)
{
  if (s=="") return 0;
   std::istringstream a(s);
   return mvctsdistributionparse(a);
}



// ddistribution *ddistributionparse(std::istream &is)
// {
//   is.clear();
//   int ch = skipblank(is);

//   if (ch==EOF)
//     throw std::domain_error("no data in stream while parsing ddistribution");

//   std::string name;

//   if (isdigit(ch)) {  // reading a constant
//     is.putback(ch);
//     int x;
//     is >> x;
//     return new Constant(x);
//   }

//   is.putback(ch);
//   for(;;) {
//     ch = is.get();
//     if (ch=='('||ch==EOF) break;
//     name.push_back(ch);
//   }

//   is.putback(ch);
//   if (name=="Improperuniform"||name=="improperuniform") {
//     return new improperuniform();
//   } else if (name=="positiveuniform"||name =="Positiveuniform") {
//     return new improperuniform();
//   } else if (name=="normal"||name=="Normal") {
//     std::pair<double,double> a = readpairfrombrackets<double,double>(is);
//     return new normal(a.first,a.second);
//   }  else if (name=="Constant"||name=="constant") {
//     double a = readparfrombrackets<double>(is);
//     return new Constant(a);
//   } else if (name=="Gamma"||name=="gamma") {
//     std::pair<double,double> a = readpairfrombrackets<double,double>(is);
//     return new Gamma(a.first,a.second);
//   } else if (name=="Uniform"||name=="uniform") {
//     std::pair<double,double> a = readpairfrombrackets<double,double>(is);
//     return new uniform(a.first,a.second);
//   } else if (name=="Beta"||name=="beta") {
//     std::pair<double,double> a = readpairfrombrackets<double,double>(is);
//     return new Beta(a.first,a.second);
//   } else {
//     std::ostringstream oss;
//     oss << name << " not known as a distribution name in ctsdistributionparse" << std::endl;
//     throw std::domain_error(oss.str().c_str());
//   }
// }

/** Sample a new value from a log scale - return the difference in log likelihoods for
 * the prior            */
double samplenewlogscale(const ctsdistribution *c,double &x, double tune, rng &r)
{
  double old=x;

  if (c->isconstant()) {
    x=c->sample(r);
    return 0.0;
  }

  if (c->min()<c->max()) {  // can assume a uniform or a beta
    x=c->sample(r);
    return 0.0;
  }

  x=exp(log(x)+tune*(r()-0.5));
  return c->log_pdf(x)-c->log_pdf(old)+log(x)-log(old);
}
/** Sample a new value from a log scale - return the difference in log likelihoods for
 * the prior            */
double samplenewlogscale(std::pair<double,ctsdistribution *> &val, double tune, rng &r)
{
  double old=val.first;

  if (val.second->isconstant()) {
    val.first=val.second->sample(r);
    return 0.0;
  }

  if (val.second->min()<val.second->max()) {  // can assume a uniform or a beta
    val.first=val.second->sample(r);
    return 0.0;
  }

  val.first=exp(log(val.first)+tune*(r()-0.5));
  return val.second->log_pdf(val.first)-val.second->log_pdf(old)+log(val.first)-log(old);
}
/** Read a multivariate distribution.
  * Note that a multivariate constant is read in using
  * (a,b,c,d) i.e. surrounded by brackets
  */
mvctsdistribution *mvctsdistributionparse(std::istream &is)
{
  is.clear();
  int ch = skipblank(is);

  if (ch==EOF)
    throw std::domain_error("no data in stream while parsing ctsdistribution");

  is.putback(ch);
  if (isdigit(ch)) {  // reading a constant
      //  std::vector<double>  a=readparsfrombrackets<double>(is);
      return new mvconstant(readparsfrombrackets<double>(is));
  }
  std::string name;
  for(;;) {
    ch = is.get();
    if (ch=='('||ch==EOF) break;
    name.push_back(ch);
  }
  is.putback(ch);

  if (name=="Dirichlet"||name=="dirichlet") {
    std::vector<double>  a=readparsfrombrackets<double>(is);
    return new dirichlet(a);
      } else if (name=="NormalGamma"||name=="normalgamma") {
    std::vector<double>  a=readparsfrombrackets<double>(is);
    return new NormalGamma(a);
  } else {
    std::ostringstream oss;
    oss << name << " not known as a distribution name in mvctsdistributionparse" << std::endl;
    throw std::domain_error(oss.str().c_str());
  }
}
