#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
  float Utilization();

private:
  // Jiffies from previous state
  long int prevuser{-1};
  long int prevnice{-1};
  long int prevsystem{-1};
  long int previdle{-1};
  long int previowait{-1};
  long int previrq{-1};
  long int prevsoftirq{-1};
  long int prevsteal{-1};
};

#endif