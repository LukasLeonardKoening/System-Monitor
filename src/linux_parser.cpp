#include <dirent.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Splits string by space to vector of strings
vector<string> splitString(string s) {
  std::stringstream stringstream(s);
  std::string split;
  vector<string> result;

  while (stringstream >> split) {
    result.push_back(split);
  }
  return result;
}

// Reads in data about the OS
string LinuxParser::OperatingSystem() {
  // Init variables
  string line, key, value;
  std::ifstream filestream(kOSPath);

  // If filestream is valid
  if (filestream.is_open()) {
    // While end of file not reached
    while (std::getline(filestream, line)) {
      // Replace charaters to use stringstream
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      // Read in key value pairs
      while (linestream >> key >> value) {
        // If key value pair of interest is reached, return it.
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Reads in data about the Kernel
string LinuxParser::Kernel() {
  string os, version, kernel, line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Reads in all pids of running tasks from filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Reads and returns the system memory utilization
float LinuxParser::MemoryUtilization() {
  float total_mem, available_mem;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          total_mem = std::stof(value);
        } else if (key == "MemAvailable:") {
          available_mem = std::stof(value);
        }
      }
    }
  }
  if (total_mem == 0.0) {
    return 0.0;
  }
  return (total_mem - available_mem) / total_mem;
}

// Reads and returns the system uptime
long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return std::stol(uptime);
}

// Reads and returns CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string usertime, nicetime, systemtime, idletime, iowait, irq, softirq, steal;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> usertime >> nicetime >> systemtime >>
             idletime >> iowait >> irq >> softirq >> steal) {
        if (key == "cpu") {
          return {usertime, nicetime, systemtime, idletime,
                  iowait,   irq,      softirq,    steal};
        }
      }
    }
  }
  return {};
}

// Reads and returns the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// Reads and returns the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// Reads and returns the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
    return command;
  }
  return "None";
}

// Reads and returns the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          // Return memory usage in MB
          return std::to_string(int(std::stod(value) / 1000));
        }
      }
    }
  }
  return string();
}

// Reads and returns the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return string();
}

// Reads and returns the user associated with a process
string LinuxParser::User(int pid) {
  // Get UID for given PID
  string uid = Uid(pid);

  string line;
  string name;
  string value;
  string x;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      // If UID of line equals UID from PID return name
      while (linestream >> name >> x >> value) {
        if (value == uid) {
          return name;
        }
      }
    }
  }
  return string();
}

// Reads and returns the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    vector<string> splitted = splitString(line);
    return UpTime() - (std::stol(splitted[21]) / sysconf(_SC_CLK_TCK));
  }
  return 0;
}

// Reads and returns CPU utilization of given PID
vector<string> LinuxParser::ProcessCpuUtilization(int pid) {
  string line;
  string key;
  string utime, stime, cutime, cstime, starttime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    vector<string> splitted = splitString(line);
    // utime, stime, cutime, cstime, starttime
    return {splitted[13], splitted[14], splitted[15], splitted[16],
            splitted[21]};
  }
  return {};
}