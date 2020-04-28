#include "processor.h"
#include "linux_parser.h"


using std::stof;
using std::string;
using std::vector;


// Return the aggregate CPU utilization
float Processor::Utilization() { 
  
  float user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  float idle_elements, non_idle, total,percent;
  vector<string> cpu_util = LinuxParser::CpuUtilization();
  
  
  user = stof(cpu_util[0]);
  nice = stof(cpu_util[1]);
  system = stof(cpu_util[2]);
  idle = stof(cpu_util[3]);
  iowait = stof(cpu_util[4]);
  irq = stof(cpu_util[5]);
  softirq = stof(cpu_util[6]);
  steal = stof(cpu_util[7]);
  guest = stof(cpu_util[8]);
  guest_nice = stof(cpu_util[9]);
  
  idle_elements = idle + iowait;
  non_idle = user + nice + system + irq + softirq + steal + guest + guest_nice;
  total = idle_elements + non_idle;
  percent = (total - idle_elements) / total;
  
  return percent; 
}