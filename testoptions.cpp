#include "options.H"
#include <iostream>
#include <cstdlib> // for EXIT_FAILURE

using std::cerr;

int main(int argc, char *argv[]) 
{
  int intopt;
  std::string s;
  options o("test options","0.1");
  try {
    o.add(&intopt,"i","an integer option",10);
    o.add(&s,"str","A string option","");  // should be able to have spaces within quotes 
                                           // this is the test I am trying to make 
    o.readcommandline(argc,argv);
  }
  catch(std::exception& e) {
    cerr << "error: " << e.what() << "\n";
    return 1;
  }	
  catch (...) {
    cerr << "unknown exception";
    return EXIT_FAILURE;
  }

  cerr << "The command line looks like:\n======================\n";
  for (size_t i=0;i<argc;i++) {
    std::cerr << i << ": " << argv[i] << std::endl;
  }
  cerr << "======================\n";

  cerr << "The integer option is: " << intopt << std::endl;
  cerr << "The string option is: " << s << std::endl;

  return(1);
}
