#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// read data from the filesystem
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

// read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

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
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal, memFree, memUtilization;
  string key, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "MemTotal:")
        linestream >> memTotal;
      if(key == "MemFree:")
      {
        linestream >> memFree;
        break;
      }
    }
    memUtilization = memTotal - memFree;
  }
  memUtilization /= memTotal;
  return memUtilization;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  unsigned long int sysUpTime, idleUpTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> sysUpTime >> idleUpTime;
  }
  return sysUpTime - idleUpTime;
 }

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::UpTime() * sysconf(_SC_CLK_TCK);
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  int tokenCounter = 0;
  string token;
  unsigned long tokenToInt = 0;
  unsigned long sum = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while(tokenCounter < 13)
    {
      stream >> token;
      tokenCounter++;
    }
    while(tokenCounter <= 16)
    {
      stream >> token;
      std::istringstream tokenStream(token);
      tokenStream >> tokenToInt;
      sum += tokenToInt;
      tokenCounter++;
    }
  }
  return sum;
 }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  unsigned long tokenToInt = 0;
  unsigned long sum = 0;
  std::string line;
  std::string cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> cpu;
    while(lineStream >> tokenToInt)
    {
      sum+= tokenToInt;
    }
  }
  return sum;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  int tokenCounter = 0;
  unsigned long idleJiffies = 0;
  string token;
  int idle, iowait;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(tokenCounter < 3)
    {
      stream >> token;
      tokenCounter++;
    }
    stream >> idle >> iowait;
    idleJiffies = idle + iowait;
  }
  return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string key, cpuValue, line;
  vector<string> cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> key;
    if(key == "cpu")
    {
      while(lineStream >> cpuValue)
        cpu.push_back(cpuValue);
    } 
  }
  return cpu;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key;
  unsigned long int value = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if(key == "processes")
          return value;
      }
    }
 }
 return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key;
  unsigned long int value =0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if(key == "procs_running")
          return value;
      }
    }
 }
 return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open())
    getline(stream, command);
  return command;
   
   }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string key;
  string line;
  int value =0;
  string str = kProcDirectory + to_string(pid) + kStatusFilename;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    while(getline(stream, line))
    {
      std::istringstream lineStream(line);
      lineStream >> key;
      if(key == "VmSize:")
      {
        lineStream >> value;
        break;
      }
    }
  }
  value = value / 1000;
  return to_string(value);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string key, value, line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    while(getline(stream, line))
    {
      std::istringstream lineStream(line);
      lineStream >> key;
      if(key == "Uid:")
      {
        lineStream >> value;
        break;
      }
    }
  }
  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string userName{}, currUid, tmp;
  std::ifstream stream(kPasswordPath);
  string uid = Uid(pid);
  if(stream.is_open())
  {
      while(getline(stream, line))
      {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream lineStream(line);
        lineStream>>userName>>tmp>>currUid;
        if(currUid == uid)
        {
          return userName;
          break;
        }
      }
  }
  return userName;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string value;
  unsigned long startTime, upTime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open())
  {
    int i = 0;
    while(i++ < 22)
      stream>>value;
    std::istringstream valueStream(value);
    valueStream >> startTime;
    upTime = startTime / sysconf(_SC_CLK_TCK);
  }
  return upTime;
}
