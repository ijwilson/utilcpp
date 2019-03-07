/// @file
#ifndef ClockH__
#define ClockH__

#include <ctime>

/// timer class 
/// wrapper for  working with the clock in ctime 
class Clock
{
public:
  Clock():start_time(time(NULL))
  {};
  double elapsed() {
    return  difftime(time(NULL), start_time);
  }
private:
  time_t start_time;
};


//---------------------------------------------------------------------------
#endif
