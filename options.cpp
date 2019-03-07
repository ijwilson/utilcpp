#include "options.H"
#include "newio.H" // for stringwrap



// template specialization for string
template<> void opt<std::string>::read(const std::string &val) {
  *x = val;
}



/** strip away leading and trailing spaces (and other characters
  * from a string   */
// from http://www.thescripts.com/forum/thread61719.html
std::string trimmed( std::string const& str, char const* sepSet )
{
  std::string::size_type const first = str.find_first_not_of(sepSet);
  return ( first==std::string::npos )
    ? std::string()
    : str.substr(first, str.find_last_not_of(sepSet)-first+1);
}
/** Split a string into smaller strings separated by delinitors  */
void Tokenise(const std::string& str,
                      std::vector<std::string>& tokens,
                      const std::string& delimiters)
// another copy of my tokenise function
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
/** Read options from the command line    */
void options::readcommandline(int argc, char *argv[]) 
{
  for (int i=1;i<argc;i++) {
    std::string a(argv[i]+2);
    if (a=="help") {
      printhelp(std::cout);
      exit(EXIT_SUCCESS);
    }
    if (a=="version") {
      printversion(std::cout,argv[0]);
      exit(EXIT_SUCCESS);
    }
  }

  pos_itor pi=posOpt.begin();
  for (int i=1;i<argc;i++) {
    if (argv[i][0]=='-') {
      if (argv[i][1]!='-') {
        std::ostringstream oss;
        oss <<  "Only using long style options" << std::endl;
        oss << argv[i] << " not recognised, exiting";
        throw options_exception(oss.str());
      }
      std::string a(argv[i]+2);
      
      // now split around "="
      std::vector<std::string> aa;
      Tokenise(a,aa,"=");
      if (aa.size()>2) throw options_exception("expected only one '=' in options!\n");
      
      itor ii=optmap.find(aa[0].c_str());
      if (ii==optmap.end()) {
        std::ostringstream oss;
        oss <<  "option --" << aa[0] << " not found";
        throw options_exception(oss.str());
      }
      
      if (aa.size()==1) {
        ii->second->settrue();
      } else {
        ii->second->read(aa[1]);
      }
    } else {
      // try this as a positional option
      if (pi==posOpt.end()) {
        std::ostringstream oss;
        oss << "Too many positional options, The application supports "
            << posOpt.size() << ".\nOption " << argv[i] << " not recognised." <<  std::endl;
        throw options_exception(oss.str());
      }
      (*pi)->read(argv[i]);
      pi++;
    }
  }
}

/** read a set of options from a file
 * they are in the format that are written by writeoptions
 * that is in pairs
option1 = value1   #comment after line
option2 = value2
# this is a comment and everything on this line is ignored
               # spaces and a comment
*/
void options::readfromfile(const char *filename,bool ExitOnWarning)
{
  std::ifstream in(filename);
  if (!in) {
    std::ostringstream oss;
    oss  << "Input file " << filename << " not found.  ";
    throw options_exception(oss.str());
  }

  while (!in.eof())  {

    std::string line;
    getline(in,line);
    line=trimmed(line," \t");  // trim away leading and trailing spaces and tabs
    if (line[0]=='#') continue;  // is this line a comment

    std::vector<std::string> aa;
    Tokenise(line,aa,"#");        // use only before token
    if (aa.size()==0) continue;  // nothing here

    std::vector<std::string> bb;

    Tokenise(aa[0],bb,"=");
    if (bb.size()>2) throw options_exception("expected only one '=' per line!\n");

    // now throw away blank spaces and tabs at the ends of the strings
    bb[0]=trimmed(bb[0]," \t");
    if (bb.size()==2) bb[1]=trimmed(bb[1]," \t");

    itor i=optmap.find(bb[0].c_str());
    if (i==optmap.end()) {
      std::ostringstream oss;
      oss <<  "option " << bb[0] << " not found";
      if (ExitOnWarning) {
        throw options_exception(oss.str());
      } else {
        std::cerr << "Warning: " << oss.str() << std::endl;
      }
    } else {
        if (bb.size()==1) {
          // test to see if this is a switch (boolean) option
          // change to true if it is seen 
          // used to switch - but reapeated readings of files would swap
          i->second->settrue();
        } else {
          i->second->read(bb[1]);
        }
    }
  }
}
/** Print the help message */
std::ostream &options::printhelp(std::ostream &o)
{
  std::left(o);   // left justify the output stream 
  o << "Usage:\nfilename ";
  pos_itor pi=posOpt.begin();
  while (pi!=posOpt.end()) {
    o << "<positionOption" << std::distance(posOpt.begin(),pi)+1 << "> ";
    pi++;
  }
  o << "<Other Options>\n\n";
  o << stringwrap(description) << std::endl;

  pi=posOpt.begin();
  while (pi!=posOpt.end()) {
    std::ostringstream oss;
    oss << " " << "positionOption" << std::distance(posOpt.begin(),pi)+1 << " ";
    (*pi)->print(oss,"(",")");
    o << std::setw(26) << oss.str();
    o << " ";
    (*pi)->printdesc(o);
    o << std::endl;
    pi++;
  }

  o << std::setw(26) << "  --help" << " produce help message " << std::endl;
  o << std::setw(26) << "  --version" <<" version number" << std::endl;

  itor i=optmap.begin();
  while (i!=optmap.end()) {
    std::ostringstream oss,ossb;
    std::left(ossb);
    oss << "  --" << i->first << " ";
    i->second->print(oss,"(",")");
    ossb << std::setw(26) << oss.str();
    ossb << " ";
    i->second->printdesc(ossb);
    ossb << std::endl;
    o << stringwrap(ossb.str(),72,"                           ") << std::endl;
    i++;
  }
  return o;
}
/** print out the options and the values set                                    */
std::ostream &options::print(std::ostream &o) const {
    printversion(o,"","#");
    citor i=optmap.begin();
    while (i!=optmap.end()) {
      if (!i->second->isnull()) {
        o << i->first << " = ";
        i->second->print(o);
        o  << std::endl;
      }
      i++;
    }
    return o;
  }
/** print out the options and the values set as an XML file                               */
std::ostream &options::printXML(const char *progname,std::ostream &o) const {
    o << "<options name=\"" << progname <<"\" version=\"" << vers <<"\" compiled=\"" << __DATE__ << "\">\n";
    citor i=optmap.begin();
    while (i!=optmap.end()) {
      o << "<" << i->first << ">";
      i->second->print(o);
      o << "</" << i->first << ">";
      o  << std::endl;
      i++;
    }
    o << "</options>\n";
    return o;
  }
/** output the information              */
std::ostream &operator<<(std::ostream &o, const options &opt)
{
  opt.print(o);
  return o;
}
