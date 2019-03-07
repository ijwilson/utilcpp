#ifndef READCSV_H__
#define  READCSV_H__

#include <string>
#include <vector>
#include <cstdlib>


std::vector<std::string>  ReadAndTokenizeLine(std::istream &is, 
                                              const std::string& delimiters=",");

/// error class throw when reading csv
class csv_exception: public std::exception {
public:
  csv_exception(const char *errtxt):message(errtxt) {
  };
  csv_exception(const std::string &errtxt):message(errtxt) {
  };
  ~csv_exception() throw() {
  }

  const char *what() const throw()  {
     return message.c_str();
  }
public:
  std::string message;
};

/// read cvs (comma separated value) files
class SimpleCSVReader {
public:
  SimpleCSVReader(const char *filename,bool header=true,bool quote=false);

  size_t ncol() const {
    return ncol_;
  }
  size_t nrow() const {
    return xx_.size();
  }
  const std::vector<std::string> &row(size_t jj) const {
    return xx_[jj];
  }
  const std::vector<std::string> &operator[](size_t jj) const {
    return xx_[jj];
  }

  std::vector<int> intcol(size_t ii) const {
    std::vector<int> ret(nrow());
    for (size_t jj=0;jj<nrow();jj++) ret[jj]=atoi(xx_[jj][ii].c_str());
    return ret;
  }
  
  std::vector<std::string> col(size_t ii) const {
    std::vector<std::string> ret(nrow());
    for (size_t jj=0;jj<nrow();jj++) ret[jj]=xx_[jj][ii];
    return ret;
  }
    
public:
  std::vector<std::string> colnames;
private:
  size_t ncol_;
  std::vector<std::vector<std::string> > xx_;

};

#endif
