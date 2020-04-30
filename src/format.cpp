#include <string>

#include "format.h"

using std::string;

// Converts long int of seconds into string of format hh:mm:ss
string Format::ElapsedTime(long seconds) {
  long int minutes = seconds / 60;
  long int hours = minutes / 60;

  string hours_s, minutes_s, seconds_s;

  if (hours < 10) {
    hours_s = "0" + std::to_string(hours);
  } else {
    hours_s = std::to_string(hours);
  }
  if ((minutes % 60) < 10) {
    minutes_s = "0" + std::to_string(minutes % 60);
  } else {
    minutes_s = std::to_string(minutes % 60);
  }
  if ((seconds % 60) < 10) {
    seconds_s = "0" + std::to_string(seconds % 60);
  } else {
    seconds_s = std::to_string(seconds % 60);
  }

  return hours_s + ":" + minutes_s + ":" + seconds_s;
}