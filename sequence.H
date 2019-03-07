// @file 
#ifndef SEQUENCE_H__
#define SEQUENCE_H__

#include <stdexcept>
#include <cstdlib>
#include <string>

template<class T>
class seq {
public:
  /** default ctor                                                             */
  seq():Length(0),Start(0),End(0),Gap(0),Log(false) {}
  /** copy constructor                                                         */
  seq(const seq &a):Length(a.Length),Start(a.Start),End(a.End)
		   ,Gap(a.Gap),Log(a.Log) {}
  /** another ctor                                                             */
  seq(const T &begin, const T &end, size_t lngth,bool log=false)
    :Length(lngth),Start(begin),End(end),Log(log) {
    calcGap();
  } 
  /** ctor for a single argument                                               */
  seq(const T &val)
    :Length(1),Start(val),End(val),Gap(0.0),Log(false) {
  }
  /** Destructor                                                               */
  ~seq() {};
  
  T operator[](int n) {
    if (n>=length()) throw std::domain_error("n > length of sequence");
    if(Log)
      return exp(log(Start)+Gap*n);
    else 
      return Start+Gap*n;
  }

  T operator()() const {
    return Start;
  }
  /** Copy operator                                                            */
  seq &operator=(const seq &rhs) {
    if (this==&rhs) return *this;
    Length=rhs.Length;
    Gap=rhs.Gap;
    End=rhs.End;
    Start=rhs.Start;
    Log=rhs.Log;
    return *this;
  }

  T gap() const {
    return Gap;
  }
  T& gap() {
    return Gap;
  }
  T& start() {
    return Start;
  }
  T start() const {
    return Start;
  }
  T& end() {
    return End;
  }
  T end() const {
    return End;
  }
  int& length() {
    return Length;
  }
  int length() const {
    return Length;
  }
  void setLog() {
    Log=true;
  }
  bool isLogarithmic() const {
    return Log;
  }
  /** calculate the gap                                                      */
  void calcGap() {  
   if (Log) 
     Gap=(log(End)-log(Start))/T(Length-1.);
   else 
     Gap=(End-Start)/T(Length-1.);
  }
  std::ostream &print(std::ostream &o) const {
    o << "(" << start() << "," << end() 
      << "," << length();
    if (isLogarithmic()) o << ",log"; 
    o << ")" ;
    return o;
  }
 private:
  int Length;
  T Start;
  T End;
  T Gap;
  bool Log;
 };


template <class T>
  std::istream &operator>>(std::istream &in, seq<T> &a)
{
  char tmp;

  in >> tmp;
  if (isdigit(tmp)||tmp=='.') {
    in.putback(tmp);
    in >> a.start();
    a.end()=a.start();
    a.length()=1;
    a.gap()=0.0;
    return in;
  }
  if (tmp != '(')
    throw std::domain_error("should be a number or ( in >> seq");
  in >> a.start();
  in >> tmp; 
  if (tmp != ',')
    throw std::domain_error("should have a , in >> seq");
  in >> a.end();
  in >> tmp;
  if (tmp != ',')
    throw std::domain_error("should have a , in >> seq");
  in >> a.length();
  in >> tmp;
  if (tmp==')') {
    a.calcGap();
    return in;
  } 

 std::string s;
  in >> s;
  if (s=="Log)" ||s=="log)") a.setLog();
    else throw std::domain_error("unexpected form of seq");

  a.calcGap();


  return in;
}

template <class T>
std::ostream &operator<<(std::ostream &o, const seq<T> &a)
{
  return a.print(o);
}

#endif
