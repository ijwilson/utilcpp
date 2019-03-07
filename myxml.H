/** @file */
#ifndef MYXML_H__
#define MYXML_H__


#include <iostream>
#include <fstream>
#include <iterator>
#include <stack>
#include <algorithm>
#include "tnt/tnt.h"
#include "util.H"
#include "utilityfunctionals.H"

using std::ostream;
using std::istream;
using std::istream_iterator;
using std::ostream_iterator;
using std::stack;
using std::ifstream;
using std::string;
using std::vector;
using std::stack;


const char opening='<';
const char closing='>';
const char endval='/';

/** A function to keep a stack of XML tags.  Will automatically
 * close the stack when it goes out of scope                  */
class xmlStack {
public:
  xmlStack(ostream &o,const char *name,bool xml=true,bool header=false)
    :oo(o),useXML(xml) {
    if (header&&xml) 
       oo << "<?xml version=\"1.0\" encoding=\"US-ASCII\"?>\n";
    
    if (useXML) {
      oo << opening << name << closing << std::endl;
      tags.push(name);
    }
  }
  void open(const char *name) {
    if (useXML) {
      oo << opening << name << closing << std::endl;
      tags.push(name);
    }
  }
  void close() {
    if (useXML) {
      oo <<  opening << endval << tags.top() << closing << std::endl;
      tags.pop();
    }
  }
  /** Destructor - make sure that all values are written             */
  ~xmlStack() {
    while( !tags.empty() ) {
      oo << opening << endval << tags.top() << closing << std::endl;
      tags.pop();
   }      
  }
  template <typename T>
  void writeElement( const char *name, const T &value,const char sep=' ') {
    if (useXML)
      oo << opening << name << closing << sep;
    oo << value << sep;
    if (useXML)
      oo << opening << endval << name << closing << sep;
    oo << std::endl;
  }
  template <typename T>
  void writeElement( const char *name, const TNT::Array1D<T> &value,const char *sep=" ") {
   if (useXML)
     oo << opening << name << closing << std::endl;
    oo << value.dim() << std::endl;
    std::copy(&value[0],&value[0]+value.dim(),ostream_iterator<T>(oo,sep)); 
    oo << std::endl;
    if (useXML)
      oo << opening << endval << name << closing << std::endl;
  }
  template <typename T>
  void writeElement( const char *name, const vector<T> &value
		  ,const char *sep=" ",const char *atend="\n") {
    if (useXML)
      oo << opening << name << closing  << std::endl;
    std::copy(value.begin(),value.end(),ostream_iterator<T>(oo,sep)); 
    if (useXML)
      oo << opening << endval << name << closing;
    oo << atend;
    
  }
  template <typename T>
  void writeElementTNT( const char *name, const vector<T> &value
		       ,const char *sep=" ",const char *atend="\n") {
    if (useXML)
      oo << opening << name << closing << std::endl;
    oo << value.size() <<  std::endl;
    std::copy(value.begin(),value.end(),ostream_iterator<T>(oo,sep)); 
    if (useXML)
      oo << opening << endval << name << closing;
    oo << atend;
    
  } 
  template <typename ITOR>
  void writeElementTNT( const char *name, ITOR begin, ITOR end
		       ,const char *sep=" ",const char *atend="\n") {
    if (useXML)
      oo << opening << name << closing << std::endl;
    oo << std::distance(begin,end) <<  std::endl;
    std::copy(begin,end,ostream_iterator<typename ITOR::value_type>(oo,sep)); 
    if (useXML)
      oo << std::endl << opening << endval << name << closing;
    oo << atend;    
  }
private:
  stack<string> tags;
  ostream &oo;
  bool useXML;
};




template <typename T>
void writeElement(ostream &o, const char *name, const T &value,const char sep=' ') {
  o << opening << name << closing << sep;
  o << value << sep;
  o << opening << endval << name << closing << sep;
}
template <typename T>
void writeElement(ostream &o, const char *name, const vector<T> &value
		  ,const char *sep=" ",const char *atend="\n") {
  o << opening << name << closing << sep;
  std::copy(value.begin(),value.end(),ostream_iterator<T>(o,sep)); 
  o << opening << endval << name << closing << atend;
}

template <typename T>
void writeElementTNT(ostream &o, const char *name, const vector<T> &value
		  ,const char *sep=" ",const char *atend="\n") {
  o << opening << name << closing << sep;
  o << value.size() << sep;
  std::copy(value.begin(),value.end(),ostream_iterator<T>(o,sep)); 
  o << opening << endval << name << closing << atend;
}


template <typename T> 
T conv(const string &s,T dum) {
  return static_cast<T>(s);
}

template<>
int  conv(const string &s,int dum) {
 return atoi(s.c_str());
}

template<>
double conv(const string &s,double dum) {
  char * pEnd;
  return strtod(s.c_str(),&pEnd);
}

/** read a vector of Ts from the file given by filename
 *  between <tag> and </tag >                              */
template<int N,typename T> 
TNT::Array2D<T> readBetweenTags(const char *filename, const char *tag) {
  TNT::Array2D<T> tmp;
  
  ifstream in(filename);
    if (!in) {
      std::cerr << "file " << filename <<" not found\n";
      error("exiting from C++ code 'readBetweenTags'");
    }

    string starttag = "<"  + string(tag) + ">";
    string endtag   = "</" + string(tag) + ">";
      
    istream_iterator<string> End;
    istream_iterator<string> curr =  std::find(istream_iterator<string>(in),End,starttag);

    if (curr==End) {
      //   REprintf("could not find tag %s in file %s\n",starttag.c_str(),*filename);
      return;
    }

    // not many checks on this !
    in >> tmp;

    return;
  }

class simpleTAG {
public:
  simpleTAG(ostream &o, string tag):oo(o),name(tag) {
    oo << "<" << name << ">\n";
  };
  ~simpleTAG() {
    oo << "</" << name << ">/n";
  }
private:    
  ostream &oo;
  string name;
};

/** read a vector of Ts from the file given by filename
 *  between <tag> and </tag >                              */
template<int N,typename T> 
void readBetweenTags(const char *filename, char **tag, int *dim,vector<T> &vec) {
  T dum;
    vec.clear();
    for (int i=0;i<N;i++) dim[i]=0;
    ifstream in(*filename);
    if (!in) {
      std::cerr << "file " << filename  << " not found\n";
      error("exiting from C++ code 'readBetweenTags'");
    }

    string starttag = "<"  + string(*tag) + ">";
    string endtag   = "</" + string(*tag) + ">";
      
    istream_iterator<string> End;
    istream_iterator<string> curr =  std::find(istream_iterator<string>(in),End,starttag);

    if (curr==End) {
      //   REprintf("could not find tag %s in file %s\n",starttag.c_str(),*filename);
      return;
    }
    for (int i=0;i<N;i++) {
      curr++;
      if (*curr==endtag||curr==End) {
	error("error  in C++ code 'readBetweenTags' when looking for dimensions in readBetweenTags in file"
	      ,filename," - not enough dimensions\n");
      } 
      dim[i]=conv(*curr,0);
    }

    for (;;) {
      curr++;
      if (*curr==endtag) break;
      if (curr==End) {
	std::cerr << "could not find end tag "  << endtag.c_str() << std::endl;
	return;
      } 
      vec.push_back(conv(*curr,dum));
    }
    if (N>0) {
      int expectedLength=1;
      for (int i=0;i<N;i++) expectedLength*=dim[i];
        if (expectedLength!=vec.size()) {
        REprintf("error - vector for tag %s in file %s is the wrong length\nshould be %d and have %d\n"
		,*tag,*filename,expectedLength,vec.size());
        warning("Vector length for tag is wrong length in C++ code 'readBetweenTags'");
	}
    }
    return;
  }

/** Get the next tag to be read from the input stream         */
string nextTag(istream &in);


#endif
