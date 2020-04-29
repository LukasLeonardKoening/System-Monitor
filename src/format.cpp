#include <string>

#include "format.h"

using std::string;

// Converts long int of seconds into string of format hh:mm:ss
string Format::ElapsedTime(long seconds) {
  long int minutes = seconds / 60;
  long int hours = minutes / 60;
  return std::to_string(hours) + ":" + std::to_string(minutes % 60) + ":" +
         std::to_string(seconds % 60);
}