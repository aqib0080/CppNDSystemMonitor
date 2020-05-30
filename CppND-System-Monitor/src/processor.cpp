#include <string>
#include <vector>
#include <unistd.h>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stof;
using std::stol;


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {  
 return (float) (LinuxParser::ActiveJiffies()/sysconf(_SC_CLK_TCK))/LinuxParser::UpTime();
 }