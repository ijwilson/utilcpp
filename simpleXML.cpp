#include "simpleXML.H"
#include "util.H"
#include <fstream>  // for ifstream
#include <iostream>  // for ostringstream

string readtag(istream &is) 
{ 
  string tag;
  char c;
  while (is.get(c)) {
    if (c=='<') {
      c=is.peek();
      if (c!='!') break;
      else std::getline(is,tag,'>');
    } else {
      if (!isspace(c))  error("expected a '<' in readtag");
    }
  }
  if (is.eof()) return "EOF";
  std::getline(is,tag,'>');
  return tag;
}
void skiptag(istream &is,const string &tag) 
{
  char c;
  int count=0;
  string xx;
  for (;;) {
    while(is.get(c)) {
      if (c=='<') break;
    }
    if (is.eof())
      error("reached the end of the file attempting to skip tag <",tag,">\n");
    is.get(c);
    if (c =='/') {
      getline(is,xx,'>');
      if (count==0&&tag==xx) {
	return;
      } else if (count==0) { 
	error("expected a closing tag </",tag,"> at this level, got <",xx,"> \n");
      } else count--;
    } else count++;
  }
}
void skiptotag(istream &is,const string &tag) 
{
  char c;
  string xx;
  for (;;) {
    while(is.get(c)) {
      if (c=='<') break;
    }
    if (is.eof())
      error("reached the end of the file attempting to skip tag <",tag,">\n");
    getline(is,xx,'>');
    if (tag==xx) {
      return;
    }
  }
}
/** read a set of tags from the tag that opens to the end of that tag
*/
bool simpleXML::read(istream &in)
{
  /* start by reading a tag until closing ">"  */
  string opening = readtag(in);
  if (opening=="EOF") return false;
  if (opening[0]=='/') {
#ifdef LOUD
    warning("expected an opening tag in simpleXML::read and got closing tag <",opening,">\n"); 
#endif
    return false;
  }
 
//   else if (opening[0]=='?') {  // declaration 
//     opening = readtag(in);
//   } else if (opening[0]=='!') {
//     // a comment try again
//     opening = readtag(in);
//   }

  while (!in.eof())  {
    string thisTAG=readtag(in);
    if (thisTAG[0]=='/') { 
      if (thisTAG.compare(1,thisTAG.length(),opening)!=0) 
        error("opening and closing braces do not match in simpleXML, have:"
              ,thisTAG,": closing tag :",opening,":\n");
      else return true;
    }
     
    itor i=XMLmap.find(thisTAG);
    if (i==XMLmap.end()) {
      skiptag(in,thisTAG);
    } else {
      i->second->read(in);
    
      string closing=readtag(in);
      if (closing[0]!='/') {
	error("need a closing tag, have ",closing,"\n");
      } else {
	if (closing.compare(1,closing.length(),thisTAG)!=0)
	  error("opening and closes  braces do not match in simpleXML, have :"
		,closing,": closing :",thisTAG,":\n");
      }
    }
  }    
  return true; 
}

ostream &simpleXML::write(ostream &o) 
{
   itor i=XMLmap.begin();
   while (i!=XMLmap.end()) {
     o << "<" << i->first <<">" << std::endl;
     i->second->print(o);
     o << std::endl <<   "</" << i->first <<">\n";
     i++; 
   }
   return o;
}
