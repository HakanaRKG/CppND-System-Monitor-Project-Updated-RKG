#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template<typename T> 
T FindValueByKey(std::string const &key_filter, std::string const &file_name){
  std::string line, key;
  T value;
  std::ifstream stream(LinuxParser::kProcDirectory + file_name);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == key_filter) {
          return value;
        }
      }
    }
  }
  return value;
};

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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float mem_total = FindValueByKey<float>(filterMemTotalString, kMeminfoFilename);
  float mem_free = FindValueByKey<float>(filterMemFreeString, kMeminfoFilename);
  return (mem_total-mem_free)/mem_total; 
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() { 
  int uptime, idletime;
  string line;
  string ss;
  string value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> uptime >> idletime;
    }
  }
  return uptime;
}

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> cpu_util = LinuxParser::CpuUtilization();
  return stoi(cpu_util[kUser_]) + stoi(cpu_util[kNice_]) + stoi(cpu_util[kSystem_]) + stoi(cpu_util[kIdle_]) + stoi(cpu_util[kIOwait_]) + stoi(cpu_util[kIRQ_]) + stoi(cpu_util[kSoftIRQ_]) + stoi(cpu_util[kSteal_]) + stoi(cpu_util[kGuest_]) + stoi(cpu_util[kGuestNice_]); 
}

// Done: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  vector<string> pid_info = LinuxParser::ParsePIDStat(pid);
  long utime = stoi(pid_info[kStatUTime_]), stime = stoi(pid_info[kStatSTime_]), cutime = stoi(pid_info[kStatCUTime_]), cstime = stoi(pid_info[kStatCSTime_]);
  return (utime + stime + cutime + cstime)/sysconf(_SC_CLK_TCK); 
}

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> cpu_util = LinuxParser::CpuUtilization();
  return (stoi(cpu_util[kUser_]) + stoi(cpu_util[kGuest_])) + (stoi(cpu_util[kNice_]) - stoi(cpu_util[kGuestNice_])) + stoi(cpu_util[kSystem_]) + stoi(cpu_util[kIRQ_]) + stoi(cpu_util[kSoftIRQ_]) + stoi(cpu_util[kSteal_]); 
}

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> cpu_util = LinuxParser::CpuUtilization();
  return stoi(cpu_util[kIdle_]) + stoi(cpu_util[kIOwait_]); 
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_utilization;
  string line;
  string data;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream)
      {
        if (linestream >> data) {
          if (data != "cpu"){
            cpu_utilization.push_back(data);
          }
        } else {
          return cpu_utilization;
        }
      }
    }
  }
  return {}; 
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return FindValueByKey<int>(filterProcesses, kStatFilename);
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return FindValueByKey<int>(filterRunningProcesses,kStatFilename);
}

// Done: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      return line;
    }
  }
  return string(); 
}

// Done: Read and return the memory used by a process
/* The memory usage that is used here is the physical memory size as VmSize is the Virtual Memory Size could be bigger 
 * than the real physical memory and could cause confussion for the user
 */
string LinuxParser::Ram(int pid) {
  string pid_file_path = to_string(pid) + kStatusFilename;
  return to_string(FindValueByKey<int>(filterProcMem,pid_file_path)/1024); 
}

// Done: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string const pid_folder_path = to_string(pid) + kStatusFilename;
  return FindValueByKey<string>(filterUID, pid_folder_path); 
}

// Done: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string user_id = LinuxParser::Uid(pid);
  string user_name, skip_second_column, Uid;
  string line;
  string key;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user_name >> skip_second_column >> Uid) {
        if (Uid == user_id) {
          return user_name;
        }
      }
    }
  }
  return string(); 
}

// Done: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  vector<string>pid_data = LinuxParser::ParsePIDStat(pid);
  return LinuxParser::UpTime() - (stoi(pid_data[kStatStartTime_])/sysconf(_SC_CLK_TCK)); 
}


std::vector<std::string> LinuxParser::ParsePIDStat(int pid){
  std::vector<std::string> return_data={};
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value) {
        return_data.push_back(value);
      }
    }
  }
  return return_data;
}