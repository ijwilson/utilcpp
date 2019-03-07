/// @file
#ifndef OPTWRAPB_H__
#define OPTWRAPB_H__

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>

/// error class for dealing with options
class options_exception: public std::out_of_range {
public:
  options_exception(const std::string &err):
    out_of_range(err) {
  };
};

void Tokenise(const std::string& str,
              std::vector<std::string>& tokens,
              const std::string& delimiters = " ");

/** A pure virtual options base class */
class optBase {
public:
  optBase(const char *desc)
    :description(desc) {};
  virtual ~optBase() {};
  virtual std::ostream &print(std::ostream &o,const char *before="",const char *after="")=0;
  virtual std::ostream &printdesc(std::ostream &o) {
    return o << description;
   }
  virtual void switchval()=0;
  virtual void read(const std::string &val)=0;
  virtual std::string desc() const {
    return description;
  }
  virtual  const char *getstr() const=0;
  virtual bool isnull() const=0;
  virtual void settrue()=0;
protected:
  std::string description;
};

/** A single option  */
template <typename T>
class opt: public optBase {
public:
  opt(T *val,const char *desc, const T &def)
    :optBase(desc),x(val) {
    *x=def;
  }
  virtual void switchval() {
    throw options_exception("trying to switch an option that expects a value");
  }
  virtual void settrue() {
    throw options_exception("trying to switch an option that expects a value");
  }
  virtual std::ostream &print(std::ostream &o,const char *before="",const char *after="") {
    o << before << *x << after;
    return o;
  };
  virtual void read(const std::string &val) {
    std::istringstream iss(val);
     iss >> *x;
     // check to see everything is read
     if (!iss.eof()) {
       std::ostringstream oss;
       char rest[256];
       iss.getline(rest,256);
       oss << "Did not read whole of option '" << description << "' have " << rest << " left";
       throw options_exception(oss.str().c_str());
     }
  }
  opt(T *val);
  virtual const char *getstr() const {
    std::ostringstream oss;
    oss << *x;
    return oss.str().c_str();
  }
  /** Get the value                 */
  T getvalue() const {
    return *x;
  }
  /** Used to allow  GuiOption to reset to a blank for an opt<string> */
  void setvalue(const T &val) {
    *x=val;
   }
  virtual bool isnull() const {
    std::string val(getstr());
    if (val=="") return true;
    return false;
  }

private:
  T *x;      // The actual value, somewhere else in the program
};

class switchopt: public optBase {
public:
  switchopt(bool *val,const char *desc)
    :optBase(desc),x(val) {
    *x=false;
  }
  virtual void switchval() {
    //  std::cerr << "switching value with description: " << desc() << ".  \nCurrently " << getstr();
    if ((*x)==true) (*x)=false;
    else (*x)=true;
    //    std::cerr << ".  Changed to " << getstr() << std::endl;;
  }
 virtual void settrue() {
   *x=true;
  }
  std::ostream &print(std::ostream &o,const char *before="",const char *after="") {
    o << before;
    if (*x) o << "true";
    else o << "false";
    o << after;
    return o;
  }
  virtual void read(const std::string &val) {
    // do the complicated thing to get rid of leading or trailing spaces
    std::istringstream iss(val);
    std::string Value;    
    iss >> Value;
    if (Value=="true"||Value=="T"||Value=="TRUE") *x=true;
    else *x=false;
           // throw options_exception("trying to read an option that is just a switch\n");
 }
  virtual  const char *getstr() const {
    if (*x) return "true";
    else return "false";
  }
  virtual bool isnull() const {
    return false;
  }
void setvalue(bool val) {
    *x=val;
  }
private:
  bool *x;      // The actual value, somewhere else in the program
};
/** The main option class that acts as a container for the options
 */
class options {
public:
  typedef std::map<std::string, optBase *>::iterator itor;
  typedef std::map<std::string, optBase *>::const_iterator citor;
  typedef std::vector<optBase *>::iterator pos_itor;
public:
  options(const char *text, const char *ver="0.01")
    :description(text),vers(ver) {
  }
  ~options() {
    std::map<std::string, optBase *>::iterator i=optmap.begin();
    while (i!=optmap.end()) {
      delete i->second;
      i++;
    }
    std::vector<optBase *>::iterator j=posOpt.begin();
     while (j!=posOpt.end()) {
       delete *j;
      j++;
     }
    // write some code to delete the options
  }
  template <typename T>
  void add(T *val,const char *text,const char *desc, const T &def) {
    optmap[text]=new opt<T>(val,desc,def);
  }
  // specialise for the string
  void add(std::string *val,const char *text,const char *desc, const char *def) {
    optmap[text]=new opt<std::string>(val,desc,std::string(def));
  }
  // specialise for a bool
  void add(bool *val,const char *text,const char *desc) {
    optmap[text]=new switchopt(val,desc);
  }
  // add a positional option
  template <typename T>
  void add(T *val, const char *text, const T &def) {
    std::ostringstream oss;
    oss << "Position " << posOpt.size()+1 << " " << text;
    posOpt.push_back( new opt<T>(val,oss.str().c_str(),def) );
  }
  // specialised for string
  void add(std::string *val, const char *text,const char *def) {
    std::ostringstream oss;
    oss << "Positional Option " << posOpt.size()+1 << ": " << text;
    posOpt.push_back( new opt<std::string>(val,oss.str().c_str(),def) );
  }

  void readcommandline(int argc, char *argv[]);
  void readfromfile(const char *filename,bool ExitOnWarning=false);
   /** print out the options and the values set                                    */
  std::ostream &print(std::ostream &o) const;
  std::ostream &printXML(const char *progname,std::ostream &o) const ;

  void printversion(std::ostream &o, const char* progname,const char *start="") const {
    o << start << progname  << " version " << vers << "  (compiled on "
      << __DATE__<<" at " << __TIME__ <<")" << std::endl;
  }
  std::string version() const {
    return vers;
  }
  std::ostream &printhelp(std::ostream &o);
  /** And now some functions that allow us to step through the options
   */
  itor begin() {
    return optmap.begin();
  }
  itor end() {
    return optmap.end();
  }
  size_t size() const {
    return optmap.size();
  }
private:
  std::map<std::string, optBase *> optmap;
  std::vector<optBase *> posOpt;   // positional options
  std::string description,vers;
};


std::ostream &operator<<(std::ostream &o, const options &opt);

#endif
