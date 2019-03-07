// options Gui,  follow tha patterns of the general options file but
// use the information in the options to create an automatic 
// and presumably very ugly form
#include <string>
#include <iosfwd>
#include <typeinfo>
#include "options.H"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>  // for file chooser

void GuiOptioncb(Fl_Widget  *ob);

/** The Gui option widget - only takes a string so depends on translating the
 * input back to the correct format.   It would be nice to have a polymorphic type 
 * but this appears to be more difficult to do                                    */

/** another thought:  What would we like to do with these options:
    1) write the default to be the original option set.
    2) reset the option to be the original
    3) when changed - change the option to the same value as optionGui

    These can all be done by
    a) setting the default
    b) and doing changes both ways when required
*/


class GuiOption: public Fl_Input {
public:
  GuiOption(int x, int y, int w, int h, const char *label = 0):
    Fl_Input(x,y,w,h,label) {
  }
  void setDefault(const char *defa) {
    Default = std::string(defa);
  }
  /** Reset to the default value          */
  void reset() {
    value(Default.c_str());
    if (Default=="") {
      if (typeid(*BaseOption)!=typeid(opt<std::string>)) {
        throw options_exception("error with reset option");
      }
      static_cast<opt<std::string> *>(BaseOption)->setvalue("");
    } else {
      BaseOption->read(Default);
    }
  } 
  /** copy from the external option to the value  */
  void copyFromOpt() {
    std::string s(BaseOption->getstr());   // no idea why I have to do this?
    value(s.c_str());
  }
  /** copy from the value to the option               */
  void copyToOpt() {
    BaseOption->read(std::string(value()));
  }
  void setbase(optBase *o) {
    // set the pointer to the correct option
    BaseOption=o;
    // set the tootip to the option description
    tooltip(BaseOption->desc().c_str());
    // when do we need the option to change
    when(FL_WHEN_CHANGED);
    callback(GuiOptioncb);
    // set default to original value
    setDefault(BaseOption->getstr());
    value(Default.c_str());
  }

private:
  optBase *BaseOption;
  std::string Default;
};


// I want to be able to extract the description, the option name and the address of
// the required value
//
class optionGui {
public:
  /** constructor with the default options         */
  optionGui(options &o);

  /** display the values                           */
  int display() {
    window->show();
    return Fl::run();
  }
  void destroy() {
    window->hide();
    Fl::flush(); 
  }
  ~optionGui() {
  }
  /** print the options to an output stream         */
  void print(std::ostream &o) {
    opt.print(o);
  }
  /** read change the values to those from opt     */
  void setvalues();
 /** read change the values to those from opt     */
  void resetvalues();
  /** Read the set of options from a file and set them on the Gui */
  void readfromfile(const char *filename);
private:
  Fl_Window *window;
  Fl_Button *b;
  std::vector<GuiOption *> flopts;
  options &opt;
};

 
