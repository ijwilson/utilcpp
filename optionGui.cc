#include "optionGui.H"
#include <fstream>
#include <iostream>
#include "fltkutils.H"

/************************************************************************/
/** Define the set of callbacks                                         */
/************************************************************************/
void GuiOptioncb(Fl_Widget  *ob) {
  static_cast<GuiOption *>(ob)->copyToOpt();
  Fl::flush(); 
}

void Acceptbutton_cb(Fl_Widget *butt,void *og) { 
  static_cast<optionGui *>(og)->destroy();
}

void OpenButton_cb(Fl_Widget *butt,void *og) { 
  Fl_File_Chooser *chooser = new Fl_File_Chooser(".",			// directory
			    "*",			// filter
			    Fl_File_Chooser::SINGLE, 	// chooser type
			    "Open  Infile");	// title
  chooser->show();
  while (chooser->shown()) {
    Fl::wait();
  }
 
  if (chooser->count()==1) {
    static_cast<optionGui *>(og)->readfromfile(chooser->value());
  }
  Fl::flush(); 
}

void SaveButton_cb(Fl_Widget *butt,void *og) { 
  Fl_File_Chooser *chooser = new Fl_File_Chooser(".",			// directory
			    "*",			// filter
			    Fl_File_Chooser::CREATE, 	// chooser type
			    "Save Infile");	// title

  chooser->show();

  while (chooser->shown()) {
    Fl::wait();
  }
  
  if (chooser->count()==1) {
    std::ofstream out(chooser->value());
    static_cast<optionGui *>(og)->print(out);
  }
  Fl::flush(); 
}

/** Create an option Gui from a normal set of options (with values)   */
optionGui::optionGui(options &o):opt(o)
{
  Fl::get_system_colors();

  // get heights and widths for a reasonable display
    size_t n=o.size()+3;
    size_t columns=(n-1)/20+1;
    if (columns>1) n=20;
    size_t height = 25+35*(n+1);
    window = new Fl_Window(400*columns+50,height);

    int y = 10;
    int count=0;
    int column=0;
    options::itor ii=opt.begin();
    while (ii!=opt.end()) {
      GuiOption *otmp = new GuiOption(400*column+100,y,300,30,ii->first.c_str());
      otmp->setbase(ii->second);
      flopts.push_back(otmp);   
      y += 35; 
      if (++count==19) {
        count=0;column++;y=10;
      }
      ++ii;
    }
    window->callback((Fl_Callback *)quit_cb);
    b =  new Fl_Button(400*column+100,y+5,200,25,"Accept");
    b->callback(Acceptbutton_cb,(void *)this);
    b->tooltip("Accept these settings?");
    b =  new Fl_Button(400*column+100,y+35,200,25,"Save");
    b->callback(SaveButton_cb,(void *)this);
    b->tooltip("Save these settings?");
    b =  new Fl_Button(400*column+100,y+65,200,25,"Load");
    b->callback(OpenButton_cb,(void *)this);
    b->tooltip("Load New Settings?");
    window->end();
 }
/** read change the values to those from opt     */
void optionGui::setvalues() {
    std::vector<GuiOption *>::iterator ii=flopts.begin();
    while (ii!=flopts.end()) {
      (*ii)->copyFromOpt();
      ++ii;
    }
    Fl::flush(); 
  }

/** read change the values to those from opt     */
void optionGui::resetvalues() {
    std::vector<GuiOption *>::iterator ii=flopts.begin();
    while (ii!=flopts.end()) {
      (*ii)->reset();
      ++ii;
    }
    Fl::flush(); 
  }

/** Read the set of options from a file and set them on the Gui */
void optionGui::readfromfile(const char *filename) {
    // first need to reset everything to the defaults
  //  resetvalues();
    // then read in the new file
    opt.readfromfile(filename);
    setvalues();
    Fl::flush(); 
  }



