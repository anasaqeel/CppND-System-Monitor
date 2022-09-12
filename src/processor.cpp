#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::stol;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpuValues;
  cpuValues = LinuxParser::CpuUtilization();
  //NonIdle = user + nice + system + irq + softirq + steal;
  unsigned long long int currNonIdle = stoll(cpuValues[LinuxParser::CPUStates::kUser_]) + stoll(cpuValues[LinuxParser::CPUStates::kNice_]) +
  stoll(cpuValues[LinuxParser::CPUStates::kSystem_]) + stoll(cpuValues[LinuxParser::CPUStates::kIRQ_]) + stoll(cpuValues[LinuxParser::CPUStates::kSoftIRQ_]) +
  stoll(cpuValues[LinuxParser::CPUStates::kSteal_]);
  unsigned long long int currIdle = stoll(cpuValues[LinuxParser::CPUStates::kIdle_]) + stoll(cpuValues[LinuxParser::CPUStates::kIOwait_]);
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
