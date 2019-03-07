#include "read454.H"
#include "SffInfo.H"
#include "newio.H"
#include <sstream>
#include <cstdlib>  // for system
#include <cstdio>  // tmpnam   




Read454::Read454(const char *file, bool trim)
{
  std::string filename = RunSffinfo(file,flow,not trim);
  std::ifstream in(filename.c_str());
 
  while (!in.eof()) {
    std::vector<std::string> tmp = readLinesUntil(in,'>',true);
    fg_.push_back(flowGram(tmp));
  }
  in.close();

  // delete the temporary file
  std::string delcommand = "rm "+filename;
  int warning = system(delcommand.c_str());
  if (warning!=0)
    std::cerr << "warning command:\n" << delcommand <<"\nmay not have run correctly\n";
}



