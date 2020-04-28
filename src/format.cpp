#include <string>
#include "format.h"
#include <iomanip>
#include <sstream>
#include <iostream>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  
  long hours, mins, secs;

    hours = seconds / 3600;
    mins = (seconds % 3600) / 60;
    if(seconds < 60){
      secs = seconds;
    }
  else {
  
    secs = seconds - (hours*3600) - (mins*60);
  }
    
  std::stringstream formatted_stream;
  formatted_stream << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0') << mins << ":" << std::setw(2) << std::setfill('0') << secs;
  
 string time = formatted_stream.str();
  
  return time;
}