#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {                
}
// TODO: Return this process's ID
int Process::Pid() { 
    return pid_; 
}


// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {  
  string key, line, token;
  vector<string> values;
  int utime = 0;
  int stime = 0;
  int cutime = 0;
  int cstime= 0;
  float starttime = 0.0;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream>>token) 
        {
          values.push_back(token);
        }    
  }
  utime = std::stoi(values[13]);
  stime = std::stoi(values[14]);
  cutime = std::stoi(values[15]);
  cstime = std::stoi(values[16]);
  starttime = std::stof(values[21]);
  float totaltime = float(utime + stime + cutime + cstime);
  float seconds = float (LinuxParser::UpTime() - (starttime/sysconf(_SC_CLK_TCK)));
  cpuutilization_ = (totaltime/sysconf(_SC_CLK_TCK))/seconds;

  return cpuutilization_;
}


// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return cpuutilization_ > a.cpuutilization_;
}