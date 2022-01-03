#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Done: Return this process's ID
int Process::Pid() { 
    return pid_;
}

// Done: Return this process's CPU utilization
float Process::CpuUtilization() { 
    return cpu_utilization_; 
}

// Done: Return the command that generated this process
string Process::Command() { 
    if (command_.length() > 50){
        command_.erase (50,command_.length()-50);
        command_ = command_ + "...";
    }
    return command_;
}

// Done: Return this process's memory utilization
string Process::Ram() { 
    return ram_; 
}

// Done: Return the user (name) that generated this process
string Process::User() { 
    return user_; 
}

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return up_time_; 
}

// Done: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return cpu_utilization_ < a.cpu_utilization_; 
}
// Function to update the data of the process
void Process::UpdateData(std::string user, std::string command, std::string ram, float cpu_utilization, long int uptime){
    user_ = user;
    command_= command;
    ram_ = ram;
    cpu_utilization_ = cpu_utilization;
    up_time_ = uptime;
}