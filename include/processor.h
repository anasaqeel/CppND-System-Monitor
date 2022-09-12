#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization(); 

 private:
 long int prevTotal = 0;
 long int prevIdle = 0;
};

#endif
