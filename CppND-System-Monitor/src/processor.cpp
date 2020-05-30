#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stof;
using std::stol;


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
 vector<string> util = LinuxParser::CpuUtilization();
 long user, nice, system, idle, iowait, irq, softirq, steal;
 user = stol(util[0]);
 nice = stol(util[1]);
 system = stol(util[2]);
 idle = stol(util[3]);
 iowait = stol(util[4]);
 irq = stol(util[5]);
 softirq = stol(util[6]);
 steal = stol(util [7]);
 
 long Idle{idle + iowait};
 long NonIdle{user + nice + system + irq + softirq + steal};
 long Total{Idle + NonIdle};
 long PrevTotal{previdle + prevnonidle};

 long totald{Total - PrevTotal};
 long idled{Idle - previdle};
 float utilization = static_cast<float>((totald - idled)/totald);
 
 return utilization;
 }