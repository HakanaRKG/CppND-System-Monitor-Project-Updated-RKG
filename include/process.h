#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid){};
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  
  void UpdateData(std::string user, std::string command, std::string ram, float cpu_utilization, long int uptime);
 private:
    int pid_;
    std::string user_, command_, ram_;
    float cpu_utilization_;
    long int up_time_;
    
};

#endif