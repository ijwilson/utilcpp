/** This is a class to implement a simple fltk console that acts to show
 * text written to an "output stream".  
 *
 * I would like to be able to also
 * 1) remove all the text and reuse
 * 2) save text from the window
 * 3) close the window without closing the application
 */

#include <iostream>
#include <sstream>
#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Text_Display.H"

class fltk_OSS: public std::ostringstream {
public:
  fltk_OSS(const char *name): std::ostringstream() {
    win = new Fl_Window(640,480);
    buff = new Fl_Text_Buffer();
    textDisplay = new Fl_Text_Display(20, 20, 640-40, 480-40, name);
    textDisplay->textfont(FL_COURIER);
    textDisplay->buffer(buff);
    win->resizable(*textDisplay);
    win->show();
  };
  ~fltk_OSS() {
    hide();
  };
  void hide() {
    win->hide();
  };
  void wipe() {
    str("");
    buff->text("");
    flushConsole();
  }


  template<typename __input> inline
  fltk_OSS& operator<<(
                       const __input & Val)
  {
    //Cast the current class to ostringstream to avoid a recursion
    *((std::ostringstream*)this)<<Val;
    flushConsole();
    return *this;
  }
  // for endl
  // only writes to the window on an endl;
  virtual fltk_OSS& operator<<(std::ostream& (*f)(std::ostream&)) {
    *((std::ostringstream*)this) << f;
    flushConsole();
    return *this;
  } 
 
  void flushConsole() {
    buff->text(str().c_str());
    Fl::flush();
  }

private:
  Fl_Window *win;
  Fl_Text_Buffer *buff;
  Fl_Text_Display *textDisplay;
};

// suggestion from the book Standard C++ Library
//Iostreams and Locales User's Guide
// template <class charT, class Traits=char_traits<charT> >
// class odatstream : public basic_ostream <charT,Traits>
// {
// public:
//  odatstream(basic_ostream<charT,Traits>& ostr,
//             const char* fmt = "%x")                            //1
//  : basic_ostream<charT,Traits>(ostr.rdbuf())
//  {
//    fmt_=new charT[strlen(fmt)];
//    use_facet<ctype<charT> >(ostr.getloc()).
//        widen(fmt, fmt+strlen(fmt), fmt_);                     //2
//  }
 
//  basic_ostream<charT,Traits>& fmt(const char* f)              //3
//  {
//    delete[] fmt_;
//    fmt_=new charT[strlen(f)];
//    use_facet<ctype<charT> >(os.getloc()).
//        widen(f, f+strlen(f), fmt_);
//    return *this;
//  }
 
//  charT const* fmt() const                                    //4
//  {
//   charT * p = new charT[Traits::length(fmt_)];
//   Traits::copy(p,fmt_,Traits::length(fmt_));
//   return p;
//  }

//  ~odatstream()                                                //5
//  { delete[] fmt_; }
 

// private:
//  charT* fmt_;                                                 //6
 
 
//  template <class charT, class Traits>                         //7
//  friend basic_ostream<charT, Traits> &
//  operator << (basic_ostream<charT, Traits >& os, const date& dat);
// };


//1	A date output stream borrows the stream buffer of an already existing output stream, so that the two streams will share the stream buffer.

//The constructor also takes an optional argument, the date format string. This is always a sequence of tiny characters.
//2	The format string is widened or translated into the stream's character type charT. This is because the format string will be provided to the time facet of the stream's locale, which expects an array of characters of type charT.
//3	This version of function fmt() allows you to set the format string.
//4	This version of function fmt() returns the current format string setting.
//5	The date stream class needs a destructor that deletes the format string.
//6	A pointer to the date format specification is stored as a private data member fmt_.
//7	The inserter for dates will have to access the date format specification. For this reason, we make it a friend of class odatstream. 
