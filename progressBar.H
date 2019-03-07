/// @file

#ifndef progressBar_H__
#define progressBar_H__


#include <iostream>

class progressBase {
public:
  virtual ~progressBase(){};
  virtual void update(int where)=0;
  virtual void finish()=0;
};

/// A class to produce a text progress bar
/** This provides a nice neat progress bar for use in text terminals.  It is only useful if length > 20.
 */
  class progressBar: public progressBase {
public:
  progressBar(std::ostream &o,const char *comm, int length)
    :oo(o),len(length),finished(false),curr(-1) {
    start(comm);
  };
  ~progressBar() {
    if (!finished) finish();
  }
  void start(const char *comm) {
    finished=false;
    oo << comm << " [" <<  std::flush;
  }
  /** restart the progress bar with a new length                    */
  void restart(const char *comm,int length) {
    if (!finished) finish();
    len = length;
    curr=-1;
    start(comm);
  }
  virtual void update(int where) {
    int pr=int(20.0*where/(double)len);
    if (pr>curr) {
      curr=pr;
      if (!(pr%4))
	oo << 5*pr << "%" << std::flush;
      else
	oo << "." << std::flush;
    }
  }
  /** Finish the progress bar                                        */
  void finish() {
    if (curr<20) oo << "100%";
    oo << "]" << std::endl;
    finished=true;
  }
private:
  std::ostream &oo;
  int len;
  bool finished;
  int curr;
};
/// A class to produce a text progress bar - most useful if length < 40 .
class simpleBar {
public:
  simpleBar(std::ostream &o,const char *comm):oo(o),finished(false){
    start(comm);
  };
  ~simpleBar() {
    if (!finished) finish();
  }
  void start(const char *comm) {
    oo << comm << " [" <<  std::flush;
  }
  void restart(const char *comm) {
    if (!finished) finish();
    start(comm);
  }
  void update() {
    oo << "." << std::flush;
  }
  void finish() {
    oo << "]" << std::endl;
    finished=true;
  }
private:
  std::ostream &oo;
  bool finished;
};

#endif
