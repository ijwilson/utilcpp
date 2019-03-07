#include "fltkutils.H"
#include <cstdlib>
#include "FL/Fl.H"

//
void quit_cb(Fl_Window *w)
{
    int butt = fl_choice("Do you want to quit?","No","Yes","Cancel");
    if (butt==1) exit(EXIT_SUCCESS);
    return;  
}
