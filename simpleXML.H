#ifndef simpleXML_H__
#define simpleXML_H__

#include <istream>
#include <iostream>
#include <map>

using std::istream;
using std::ostream;
using std::string;
using std::map;

/** This is a very simple file to read a flat xml format file - particularly
 * useful for the output of simulations.
 * entries can either by TNT::Array1D<T>, TNT::Array2D<T> or scalar values
 * that are read from between tags 
 * the name of the tag gives the values
 *
 * the values are rad from the file by priming a parser with what to expect - in 
 * any order and then reading these in until the end of a replicate tag
 * 
 *
 */


class nodeBase {
public:
  nodeBase():isSet_(false) {
  };
  virtual ~nodeBase() {
  };
  virtual void read(istream &is)=0;
  void reset() {
    isSet_=false;
  }
  bool isSet() const {
    return isSet_;
  }
  virtual ostream &print(ostream &o) const =0;
protected:
  bool isSet_;
};


template <typename T>
class xmlnode: public nodeBase {
public:
  xmlnode(T *val)
    :nodeBase(),x(val) {
  }
  virtual void read(istream &is) {
    is >> *x;
    isSet_=true;
  }
  virtual ostream &print(ostream &o) const {
    return o << *x;
  }
private:
  T *x;      // The actual value, somewhere else in the program
};


class xmlstringnode: public nodeBase {
public:
  xmlstringnode(string *val)
    :nodeBase(),x(val) {
  }
  virtual void read(istream &is) {
    getline(is,*x,'<');
    is.putback('<');
    isSet_=true;
  }
  virtual ostream &print(ostream &o) const {
    return o << *x;
  }
private:
  string *x;      // The actual value, somewhere else in the program
};  



class simpleXML {
  typedef std::map<std::string, nodeBase *>::iterator itor;
  typedef std::map<std::string, nodeBase *>::const_iterator citor;
public:
  simpleXML() {
  }
  template <typename T>
  void add(T *val,const char *text) {
    XMLmap[text]=new xmlnode<T>(val);
  }
  void add(string *val,const char *text) {
    XMLmap[text]=new xmlstringnode(val);
  }

  void reset() {
    itor i=XMLmap.begin();
    while(i!=XMLmap.end()) 
      (i++)->second->reset();
  }

  bool read(istream &in);
  ostream &write(ostream &o);
  
private:
  std::map<std::string, nodeBase *> XMLmap;
};


string readtag(istream &is);
void skiptag(istream &is,const string &tag); 
void skiptotag(istream &is,const string &tag);



#endif
