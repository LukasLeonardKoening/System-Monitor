#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
  Process(int PID);
  int Pid() const;
  std::string User();
  std::string Command();
  void updateCpuUtilization();
  float getCpuUtilization() const;
  std::string Ram();
  long int UpTime();
  bool operator<(Process const &a) const;

private:
  int pid;
  float cpu_utilization{0.0};
};

#endif