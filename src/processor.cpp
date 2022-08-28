#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::stol;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpuValues;
  cpuValues = LinuxParser::CpuUtilization();
  //NonIdle = user + nice + system + irq + softirq + steal;
  unsigned long long int currNonIdle = stol(cpuValues[LinuxParser::CPUStates::kUser_] + cpuValues[LinuxParser::CPUStates::kNice_] +
  cpuValues[LinuxParser::CPUStates::kSystem_] + cpuValues[LinuxParser::CPUStates::kIRQ_] + cpuValues[LinuxParser::CPUStates::kSoftIRQ_] +
  cpuValues[LinuxParser::CPUStates::kSteal_]);
  unsigned long long int currIdle = stol(cpuValues[LinuxParser::CPUStates::kIdle_] + cpuValues[LinuxParser::CPUStates::kIOwait_]);
  unsigned long long total, totald, idled;
  total = currNonIdle + currIdle;

  //calculating the deltas (current - previous)
  totald = total - prevTotal;
  idled = currIdle - prevIdle;
  float cpuPercentage = 0.0;
  if(totald > 0)
    cpuPercentage = (float)(totald - idled)/totald;

  prevTotal = total;
  prevIdle = currIdle;

  return cpuPercentage;
}
