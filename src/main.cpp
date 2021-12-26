#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"

#include "format.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <set>

int main() {
  System system;
  NCursesDisplay::Display(system);
  /*while(1){
    std::vector<int> pid_info = LinuxParser::Pids();
    
    std::set<Process> temp_data;  
    for (auto info : pid_info )
    {
        Process process_info(info);
        float cpu_util;
        if (LinuxParser::UpTime(info) > 0)
            cpu_util= (float)LinuxParser::ActiveJiffies(info) / (LinuxParser::UpTime() - LinuxParser::UpTime(info));//
        else
            cpu_util = 0;
        process_info.UpdateData(LinuxParser::User(info),LinuxParser::Command(info),LinuxParser::Ram(info),cpu_util,LinuxParser::UpTime(info));
        temp_data.insert(process_info);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }*/

  //LinuxParser::MemoryUtilization();
  //LinuxParser::TotalProcesses();
  //LinuxParser::UpTime();
  //Format::ElapsedTime(LinuxParser::UpTime());
  //std::cout << LinuxParser::UpTime(1918) << std::endl;
  //LinuxParser::ParsePIDStat(1918);
  //system.Processes();
}