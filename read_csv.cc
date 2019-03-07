#include "read_csv.H"
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::string>  ReadAndTokenizeLine(std::istream &is, 
                                          const std::string& delimiters) {
  std::vector<std::string> tokens;
  std::string str;
  getline(is,str);
  // std::cerr << str << std::endl;
    // Skip delimiters at beginning.
  std::string::size_type lastPos = 0;//str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
  std::string::size_type pos;

  while (true) {
    pos = str.find_first_of(delimiters, lastPos);
    tokens.push_back(str.substr(lastPos, pos - lastPos));          
    if (pos==std::string::npos) { // no more tokens, found end
      break;
    }
    lastPos = pos+1;
  }
  return tokens;
}
SimpleCSVReader::SimpleCSVReader(const char *filename,bool header,bool quote) {
    std::ifstream inf(filename);
    // std::cerr << filename << std::endl;
    if (!inf) {
      std::ostringstream oss;
      oss << "Error, file " << filename << " cannot be opened for reading\n";
      throw csv_exception(oss.str());
    }
    std::vector<std::string> tmp;
    if (header) {
      colnames=ReadAndTokenizeLine(inf);
      ncol_=colnames.size();
      tmp = ReadAndTokenizeLine(inf);
    } else {  // read the first line then rewind
      tmp = ReadAndTokenizeLine(inf);
      ncol_=tmp.size();
    }
    int count=0;
    
    while (inf) {
      if (tmp.size()!=ncol_) 
        std::cerr << "warning, line " << count << " has " << tmp.size() << " elements not " << ncol() << std::endl; 
      
      if (tmp.size()>0) {
	xx_.push_back(tmp);
        count++;
      }
      tmp = ReadAndTokenizeLine(inf);
    }
    inf.close();
  
  }
