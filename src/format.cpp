#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) 
{
    std::stringstream ss;
    string time;
    long hour = 0;
    long min = 0;
    long sec = 0;

    min = seconds/60;
    hour = min/60;

    sec = seconds - (min*60);
    min = min - (hour*60);

    ss << std::setw(2) << std::setfill('0') << hour << ":" << std::setw(2) << std::setfill('0') << min << ":" << std::setw(2) << std::setfill('0') << sec;
    time = ss.str();
    return time;
    
}
