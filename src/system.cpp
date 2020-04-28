#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
   
  vector<int> pids;
  pids = LinuxParser::Pids();
  processes.clear();
  for(auto pid : pids)
  {
   processes.push_back(Process(pid));
  }
  std::sort(processes.begin(), processes.end(),[](Process& p1, Process& p2){return (p1<p2) ;});
  
  return processes; 

}

//  Return the system's kernel identifier (string)
std::string System::Kernel() 
{ 
  return string(LinuxParser::Kernel()); 
}

// Return the system's memory utilization
float System::MemoryUtilization() 
{ 
  return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() 
{ 
  return string(LinuxParser::OperatingSystem()); 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() 
{ 
  return LinuxParser::RunningProcesses(); 
}


// Return the total number of processes on the system
int System::TotalProcesses() 
{ 
  return LinuxParser::TotalProcesses(); 
}

// Return the number of seconds since the system started running
long int System::UpTime() 
{ 
  return LinuxParser::UpTime(); 
}