#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{
    std::stringstream ss;
    string time;
    long hour = 0;
    long min = 0;
    long sec = 0;

    min = sec/60;
    hour = min/60;

    sec = seconds - (min*60);
    min = min - (hour*60);

    ss << std::setw(2) << std::setfill('0') << hour << ":" << std::setw(2) << std::setfill('0') << min << ":" << std::setw(2) << std::setfill('0') << sec;
    time = ss.str();
    return time;
    
}
