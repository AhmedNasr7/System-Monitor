#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::stol;
using std::stof;
using std::string;
using std::to_string;
using std::vector;


// An example of how to read data from the filesystem
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
  filestream.close();

  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream fstream(kProcDirectory + kVersionFilename);
  if (fstream.is_open()) {
    std::getline(fstream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  fstream.close();
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  	
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  long mem_total, mem_free;
  vector<long>mem; // 0: mem_total, 1: mem_free
  
  for (int i = 0; i < 2; i++) 
  {
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream sstream(line);
    sstream >> key >> value;
    mem.push_back(stol(value));
  }
  }
  
  filestream.close();
  mem_total = mem[0];
  mem_free = mem[1];
  
  	
  return (float) (mem_total - mem_free) / mem_total; 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  
  string line, uptime, idletime;
  long uptime_l;
  
  std::ifstream fstream(kProcDirectory + kUptimeFilename);
  if (fstream.is_open())
  {
    std::getline(fstream, line);
    std::istringstream sstream(line);
    sstream >> uptime >> idletime;
    uptime_l = stol(uptime);
    
  }
  
  fstream.close();
  return uptime_l; 
}



float LinuxParser::ActiveJiffies(int pid) { 
  string line, value;
  string utime, cutime, cstime, Stime;
  long int total;
  float sec, cpu_usage;  

  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream sstream(line);
       for(int i = 0; i < 17; i++)
       {
       sstream >> value;
       if(i == 13) utime = value;
       else if(i == 14) Stime = value;
       else if(i == 15) cutime = value;
       else if(i == 16) cstime = value;
     }
   }
        
    }
  filestream.close();
  
  total = stol(utime) + stol(Stime) + stol(cutime) + stol(cstime);
  sec = UpTime(pid);
  cpu_usage = ((total / sysconf(_SC_CLK_TCK)) / sec);
  
  return cpu_usage; 
   
}



// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string>values;
  string value, line;
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,guest_nice;
  
  std::ifstream fstream(kProcDirectory + kStatFilename);
  if (fstream.is_open())
  {
    std::getline(fstream, line);
    std::istringstream sstream(line);
    sstream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>softirq >> steal >> guest >> 		guest_nice;
   
	values.push_back(user);
    values.push_back(nice);
    values.push_back(system);
    values.push_back(idle);
    values.push_back(iowait);
    values.push_back(irq);
    values.push_back(softirq);
    values.push_back(steal);
    values.push_back(guest);
    values.push_back(guest_nice);    
    
    
  }

  fstream.close();

  return values; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream sstream(line);
      sstream >> key >> value;
      if (key == "processes") break;
        
    }
    
  }
  filestream.close();
  return stoi(value); 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream sstream(line);
      sstream >> key >> value;
      if (key == "procs_running") break;
        
    }
    
  }
  filestream.close();
  return stoi(value); 

}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  
  string line, cmd;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open())
  {
   	  getline(filestream, line);
      std::istringstream sstream(line);
      sstream >> cmd;       
  }

  filestream.close();

  return cmd; 
 }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid)
{ 
	string line, key, value;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream sstream(line);
      sstream >> key >> value;
      if (key == "VmSize:") break;
        
    }
    
  }

  filestream.close();
  return value; 

}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream sstream(line);
      sstream >> key >> value;
      if (key == "Uid:") {
         return value;  

      }
        
    }
    
  }

  filestream.close();

  return "-1";                             
  }

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  
  
  string key, unit, value, line, uid;
  uid = LinuxParser::Uid(pid);
  std::ifstream fstream(kPasswordPath);
  if(fstream.is_open()){
    while (std::getline(fstream, line)){
  		std::replace(line.begin(), line.end(), ':', ' ');
     	std::istringstream linestream(line);
    while (linestream >> key >> unit >> value) {
        if (value == uid)  return key;
      }
    }
   }
fstream.close();
return "-1";
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  
  string line, key, value;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open())
  {
    while(std::getline(filestream, line)) 
    {
     std::istringstream sstream(line);
     for(int i = 0; i < 22; i++)
      {
        sstream >> value;        
      }
    }
     
  }
  filestream.close();
  return UpTime() - (stol(value) / sysconf(_SC_CLK_TCK));
}