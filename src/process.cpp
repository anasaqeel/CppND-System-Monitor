#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

//Return this process's ID
int Process::Pid() { return pid_;}

// Return this process's CPU utilization
float Process::CpuUtilization() {
    string line, value;
    int tokenCounter = 0;
    long int utime, stime, cutime, cstime, startTime;
    float cpuUsage = 0;
    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
    if(stream.is_open())
    {
        string s;
        //std::getline(stream, line);
        //std::istringstream lineStream;//(line);
        while (tokenCounter < 22)
        {
            stream>>s;
            std::istringstream lineStream(s);
            //lineStream>>value;
            long numValue = 0;
            
            if(tokenCounter >= 13 && tokenCounter <=21)
            {
                if (!(lineStream >> numValue).fail())
                {    
                    long int myLong = std::stoll(s);
                    if(tokenCounter == 13)
                        utime = myLong;
                    else if(tokenCounter == 14)
                        stime = myLong;
                    else if(tokenCounter == 15)
                        cutime = myLong;
                    else if (tokenCounter == 16)
                        cstime = myLong;
                    else if (tokenCounter == 21)
                        startTime = myLong;
                }
                else {
                    std::cerr << "There was a problem converting the string to an integer!" << std::endl;
                }
            }
            tokenCounter++;
        }
        long long int totalTime = utime + stime + cutime + cstime;
        long long int hertz = sysconf(_SC_CLK_TCK);
        float seconds = LinuxParser::UpTime() - (startTime / (float)hertz);
        cpuUsage = 100 * ((totalTime / (float)hertz) / seconds);
    }
    return cpuUsage;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return LinuxParser::UpTime(this->pid_) < LinuxParser::UpTime(a.pid_); }
