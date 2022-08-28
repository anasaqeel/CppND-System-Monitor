#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    string line, value;
    int tokenCounter = 0;
    long int utime, stime, cutime, cstime, startTime;
    float cpuUsage = 0;
    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
    if(stream.is_open())
    {
        std::getline(stream, line);
        std::istringstream lineStream(line);
        while (tokenCounter++ < 22)
        {
            lineStream>>value;
            if(tokenCounter = 13)
                utime = std::stoll(value);
            else if(tokenCounter == 14)
                stime = std::stoll(value);
            else if(tokenCounter == 15)
                cutime = std::stoll(value);
            else if (tokenCounter == 16)
                cstime = std::stoll(value);
            else if (tokenCounter == 21)
                startTime = std::stoll(value);
        }
        long long int totalTime = utime + stime + cutime + cstime;
        long long int hertz = sysconf(_SC_CLK_TCK);
        float seconds = LinuxParser::UpTime() - (startTime / hertz);
        cpuUsage = 100 * ((totalTime / hertz) / seconds);
    }
    return cpuUsage;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return LinuxParser::UpTime(this->pid_) < LinuxParser::UpTime(a.pid_); }
