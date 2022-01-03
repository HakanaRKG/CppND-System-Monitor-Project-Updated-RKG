#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();
  
 private:
  float prev_active, prev_idle;
};

#endif