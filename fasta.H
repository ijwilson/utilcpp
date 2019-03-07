#ifndef FASTA_H__
#define FASTA_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "newio.H"
 
/** A class to hold fasta type files                                 
 * This one is mainly for fasta quality files                        */
template <typename T> class fasta {
public:
  /** Standard constructor from an input stream                      */
    fasta(std::istream &in) { 
        read(in);
    }
    /** Read in a sequence from an sff file   */
    std::istream &read(std::istream &in) {
    	seq_.clear();
    	std::vector<std::string> tmp = readLinesUntil(in,'>',true);
    if (tmp[0][0]!='>') 
      throw ioerror("expected a > in fasta<T>::read(), got",tmp[0][0]);
    	tmp[0].erase(0,1);  // get rid of the >
    
    	name_=tmp[0];
    	for (size_t ii=1;ii<tmp.size();ii++) {
			std::istringstream iss(tmp[ii]);
      		T xx;
      		while (iss) {
        		iss >> xx;
        		seq_.push_back(xx);
      		}
    	}
    	return in;
	}
	/** convert the fasta template to a string stripping trailing spaces                        */
	std::string tostring() const { 
	  std::ostringstream oss;
	  std::copy(seq_.begin(),seq_.find_first_of(' '),std::ostream_iterator<T>(oss));
	  return oss.str();
  	}
	std::ostream &print(std::ostream &out,const char *space=" ") const {
    	out << name_ << std::endl;
    	return printvector(seq_,out);
  	}
	/** reverse the sequence                                          */
	void rev() {
  		std::reverse(seq_.begin(),seq_.end());
  	}
	/** helper function to return the name                            */
	const std::string &name() const {
    	return name_;
	}
	/** what is the length of the sequence                            */ 
	size_t length() const {
    	return seq_.size();
  	}
private:
  std::string name_;
  std::vector<T> seq_;
};
/** A specialisation of the fasta class to deal with character based files*/
template <>
class fasta<char> {
 public:
  /** constructor  from an input stream                                */
  fasta(std::istream &in) {
     read(in);
  }
  /** return the name                         */
  const std::string name() const {
     return name_;
  }
   /** what is the length of a sequence           */
  size_t length() const {
    return seq_.size();
  }
  /** Are any bases missing ? sing bases                               */
  bool anyMissing() const {
    if (std::find(seq_.begin(),seq_.end(),'N')==seq_.end()) return false;
    return true;
  }
  /** Read from a file                                                 */
  std::istream &read(std::istream &in) {
    seq_.clear();
    std::vector<std::string> tmp = readLinesUntil(in,'>',true);
    if (tmp[0][0]!='>') 
      throw ioerror("expected a > in fasta<char>::read(), got",tmp[0][0]);
    
    tmp[0].erase(0,1);  // get rid of the >
    name_ = tmp[0];
    for (size_t ii=1;ii<tmp.size();ii++) {
      std::copy(tmp[ii].begin(),std::find(tmp[ii].begin(),tmp[ii].end(),' '),back_inserter(seq_));
    }
    return in;
  }   
  /** Turn it into a string                                           */
  std::string tostring() const {
    return std::string(seq_.begin(),std::find(seq_.begin(),seq_.end(),' '));
  }
  /** print out the sequence with a name                              */
  std::ostream &print(std::ostream &out,const char *space=" ") const {
    out << name_ << std::endl;
    return printvector(out,seq_);
  }
  /** reverse the sequence                                          */
  void rev() {
    std::reverse(seq_.begin(),seq_.end());
  }
private:
  std::string name_;
  std::vector<char> seq_;
};

template<typename T>
std::istream operator>>(std::istream &in, fasta<T> &a)
{
  return a.read(in);
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const fasta<T> &a)
{
  return a.print(out);
}
#endif
