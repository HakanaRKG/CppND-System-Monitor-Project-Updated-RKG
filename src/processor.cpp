#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
    float cur_active = LinuxParser::ActiveJiffies(), cur_idle = LinuxParser::IdleJiffies();
    float d_total = (cur_active + cur_idle) - (prev_active + prev_idle), d_idle = cur_idle - prev_idle;
    prev_active = cur_active;
    prev_idle = cur_idle;
    return (d_total - d_idle) / d_total;     
}