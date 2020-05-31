#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >>kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float totalmemory{0.0};
  float buffer{0.0};
  float memoryfree{0.0};
  string line;
  string key;
  float value{0.0};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "MemTotal:") 
        {
          totalmemory =  value;
        } 
        else if(key == "MemFree:")
        {
          memoryfree = value;
        }
        else if(key == "Buffers:")
        {
          buffer = value;
        }
      }
  
    }
  }
  return 1.0 -(memoryfree/(totalmemory - buffer));
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string token;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> token)
      {
      return stol(token);
      }
  
    }
  }
  return stol(token);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::UpTime()*sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> utilization = LinuxParser::CpuUtilization();
  long activejiffies{0};
  activejiffies = stol(utilization[0]) + stol(utilization[1]) + stol(utilization[2]) + stol(utilization[5]) + stol(utilization[6])
  + stol(utilization[7]) + stol(utilization[8]) + stol(utilization[9]);
  return activejiffies;
}
// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    vector<string> utilization = LinuxParser::CpuUtilization();
  long idlejiffies{0};
  idlejiffies = stol(utilization[3]) + stol(utilization[4]);
  return idlejiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string key, line, token;
  vector<string> values;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> token) {
      if(token == "cpu"){
        while(linestream>>token) 
        {
          values.push_back(token);
        }
      return values;    
     }
   }
  }
  return values;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {  
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream stream(line);
      while (stream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {   
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream stream(line);
      while (stream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {  
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid)+ kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  
  return line;

}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {   
  string line;
  string key;
  string value;
  string ram;
  std::ifstream filestream(kProcDirectory + to_string(pid)+ kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line))
    {
    std::istringstream linestream(line);
    while (linestream >> key >> value) {
      if(key == "VmData:")
      {
        ram = value;
        return to_string(stoi(ram)/1024);
      }
   }
  }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> key >> value) {
      if(key == "Uid:")
      {
      return value;
      }
   }
  }
  return value; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string user;
  string line;
  std::ifstream stream(kPasswordPath);
  string uid = Uid(pid);
  if (stream.is_open()) {
    
    while (getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      string key;
      string temp;
      std::istringstream user_string(line);
      user_string >> user >> temp >> key;
      if (key == uid) {
        return user;
      }
    }
  }
  return user;
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid){
  string line, key;
  string token;
  int count = 0;
  std::ifstream stream(kProcDirectory + to_string(pid)+ kStatFilename);
  if (stream.is_open()) {
    while(stream>>token)
    {
      if(count == 21)
      {
        return UpTime() - stol(token)/sysconf(_SC_CLK_TCK);
      }

    count += 1;
    }
  }
  return UpTime() - stol(token)/sysconf(_SC_CLK_TCK);

} 