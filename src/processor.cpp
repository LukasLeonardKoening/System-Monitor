#include "processor.h"
#include "linux_parser.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

// Returns the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpu_usage = LinuxParser::CpuUtilization();
  const long int usertime = std::stol(cpu_usage[0]);
  const long int nicetime = std::stol(cpu_usage[1]);
  const long int systemtime = std::stol(cpu_usage[2]);
  const long int idletime = std::stol(cpu_usage[3]);
  const long int iowait = std::stol(cpu_usage[4]);
  const long int irq = std::stol(cpu_usage[5]);
  const long int softirq = std::stol(cpu_usage[6]);
  const long int steal = std::stol(cpu_usage[7]);

  // Calculate needed jiffy values (current state)
  const float IDLE = idletime + iowait;
  const float NON_IDLE =
      usertime + nicetime + systemtime + irq + softirq + steal;
  const float TOTAL = IDLE + NON_IDLE;

  if (prevuser == -1 || prevnice == -1 || prevsystem == -1 || previdle == -1 ||
      previowait == -1 || previrq == -1 || prevsoftirq == -1 ||
      prevsteal == -1) {
    // Save current CPU status for future calculations
    prevuser = usertime;
    prevnice = nicetime;
    prevsystem = systemtime;
    previdle = idletime;
    previowait = iowait;
    previrq = irq;
    prevsoftirq = softirq;
    prevsteal = steal;

    // calculate percentage of CPU usage
    if (TOTAL == 0.0) {
      return 0.0;
    }
    return (TOTAL - IDLE) / TOTAL;
  } else {
    // Calculate needed jiffy values (previous state)
    const long int PREV_IDLE = previdle + previowait;
    const long int PREV_NON_IDLE =
        prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    const long int PREV_TOTAL = PREV_IDLE + PREV_NON_IDLE;

    // Calculate differential values
    const float TOTAL_DIF = TOTAL - PREV_TOTAL;
    const float IDLE_DIF = IDLE - PREV_IDLE;

    // Save current CPU status for future calculations
    prevuser = usertime;
    prevnice = nicetime;
    prevsystem = systemtime;
    previdle = idletime;
    previowait = iowait;
    previrq = irq;
    prevsoftirq = softirq;
    prevsteal = steal;

    // calculate percentage of CPU usage
    if (TOTAL_DIF == 0.0) {
      return 0.0;
    }
    return (TOTAL_DIF - IDLE_DIF) / TOTAL_DIF;
  }
  return 0.0;
}