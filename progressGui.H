// a class to produce a progressBar within an FLTK window - will be removed
// once it is finished

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Progress.H>

#include "fltkutils.H"
#include "progressBar.H"

/// A gui for a progress bar
class progressGui: public progressBase {
public:
 progressGui(const char *comm, int length)
   :len(length) {
   w = new Fl_Window(420,80);
   progress =  new Fl_Progress(10,10,400,60,comm);
   progress->minimum(0);               // set progress bar attribs..
   progress->maximum(1);
   progress->value(0.0);
   progress->selection_color(FL_BLUE);
   w->callback((Fl_Callback *)quit_cb);
   w->end();  
   w->show();
  };
  ~progressGui() {
    w->remove(progress);                // remove progress bar from window
    delete(progress);  
    delete(w);
  }
  virtual void update(int where) {
    progress->value(static_cast<double>(where)/static_cast<double>(len));
    Fl::check();    
  }

  virtual void finish() {
    // nothing - a dummy for the other progress classes
  }


private:
  Fl_Progress *progress; 
  Fl_Window *w;
  int len;
  bool finished;
};
