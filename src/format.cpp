#include <string>
#include <iostream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string hour_text, minute_text, seconds_text;
    double floating, fraction;

    // Calculating hour
    floating = (float)seconds/(60*60);
    hour_text= std::to_string((int)floating);
    fraction = floating - (int)floating;

    // Calculating minutes
    floating = fraction*60; // returning the fraction to seconds
    minute_text= std::to_string((int)floating);
    fraction = floating - (int)floating;

    // Calculating seconds
    floating = fraction*(60); // returning the fraction to second
    seconds_text= std::to_string((int)floating);

    return hour_text + ":" + minute_text + ":" + seconds_text;
}