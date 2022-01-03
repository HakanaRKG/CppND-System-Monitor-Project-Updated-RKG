#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector<int> pid_info = LinuxParser::Pids();
    
    set<Process> temp_data;
    processes_= {};
    for (auto info : pid_info )
    {
        Process process_info(info);
        float cpu_util;
        if (LinuxParser::UpTime(info) > 0)
            cpu_util= (float)LinuxParser::ActiveJiffies(info) / (LinuxParser::UpTime(info));//
        else
            cpu_util = 0;
        process_info.UpdateData(LinuxParser::User(info),LinuxParser::Command(info),LinuxParser::Ram(info),cpu_util,LinuxParser::UpTime(info));
        temp_data.insert(process_info);
    }
    std::copy(temp_data.begin(), temp_data.end(), std::back_inserter(processes_));
    std::sort(processes_.begin(),processes_.end(), //CompareFValue);
      [](Process a, Process b){
          return (a.CpuUtilization() > b.CpuUtilization());
      }
    );
    
    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

// Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

// Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}