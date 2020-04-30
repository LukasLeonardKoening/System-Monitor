#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int PID) {
  this->pid = PID;
  updateCpuUtilization();
}

// Return this process's ID
int Process::Pid() const { return this->pid; }

// Return this process's CPU utilization
float Process::getCpuUtilization() const { return cpu_utilization; }

void Process::updateCpuUtilization() {
  // Get raw data
  vector<string> cpu_usage = LinuxParser::ProcessCpuUtilization(this->pid);
  const long int utime = std::stol(cpu_usage[0]);
  const long int stime = std::stol(cpu_usage[1]);
  const long int cutime = std::stol(cpu_usage[2]);
  const long int cstime = std::stol(cpu_usage[3]);
  const long int starttime = std::stol(cpu_usage[4]);
  const long int systime = LinuxParser::UpTime();

  // Calculate cpu ustilization
  const float TOTAL_TIME =
      (utime + stime + cutime + cstime) / sysconf(_SC_CLK_TCK);
  const float ELAPSED_TIME = systime - (starttime / sysconf(_SC_CLK_TCK));
  if (ELAPSED_TIME == 0.0) {
    cpu_utilization = 0.0;
  } else {
    cpu_utilization = TOTAL_TIME / ELAPSED_TIME;
  }
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(this->pid); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(this->pid); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(this->pid); }

// Return the age of this process (in seconds)
long int Process::UpTime() {
  // std::cout << LinuxParser::UpTime(this->pid) << std::endl;
  return LinuxParser::UpTime(this->pid);
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const &a) const {
  return getCpuUtilization() < a.getCpuUtilization();
}