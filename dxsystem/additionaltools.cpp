#include <ctime>
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include "dmodeldetails.h"

#include "additionaltools.h"

using namespace std;
#define MAX_DATE 16

/**
 * @brief delay
 * Introduce delay for set amount of time (for current thread)
 * @param msec Delay time in milliseconds
 */
void delay(int msec)
{
    chrono::milliseconds duration(msec);
    this_thread::sleep_for(duration);
}

/**
 * @brief getTimeStr
 * Gives the current date and time, formatted as a string, to append to filenames
 * @return Formatted string with current date and time
 */
string getTimeStr()
{
    ostringstream timeStream;
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    char timestr[MAX_DATE];
    strftime(timestr, MAX_DATE, "%Y%m%d%H%M%S", localtime(&now));
    timeStream << timestr;
    return timeStream.str();
}

/**
 * @brief str2int Converts a string to an integer (parsing file)
 * @param s Input string (assumes positive integer input)
 * @return Integer equivalent of the input string (-1 if string is empty)
 */
int str2int(const string& s)
{
    if (s.compare("") == 0)
        return -1;
    return atoi(s.c_str());
}

/**
 * @brief getInt
 * Asks for an integer input; adapted from a similar function in simpio of Stanford Template Library
 * @return A valid integer
 */
int getInt()
{
    string s;
    int num = 0;
    while(true) {
        getline(cin, s);
        istringstream str(s);
        str >> num >> ws;
        if (!str.fail() && str.eof())
            break;
        cout << "Enter an integer, try again:";
    }
    return num;
}
/**

 * @brief getYorN
 * Asks for yes/no input; adapted from a similar function in simpio of Stanford Template Library
 * @return Boolean corresponding to yes (true) or no (false)
 */
bool getYorN()
{
    string s;
    while(true) {
        getline(cin, s);
        if (s.length() > 0)
        {
            char option = tolower(s[0]);
            if (option == 'y')
                return true;
            if (option == 'n')
                return false;
        }
        cout << "Please type 'y' or 'n':";
    }
}


#ifndef PI
#define PI 3.14159f
#endif
/**
 * @brief angle2Position
 * Converts angle input into position data (based on motor model provided)
 * @param angle Angle to be converted
 * @param model Motor model, as encoded on Dynamixel motor's control table
 * @param isRadians Boolean flag for unit used in expressing the angle, true if radians (radians by default)
 * @return Position value corresponding to the joint angle (mid-position is 0 degrees/rad)
 */
int angle2Position(double angle, int model, bool isRadians)
{
    double angleInDegree = isRadians ? angle*180/PI : angle;
    int position = (angleInDegree + modelAngleRange(model)/2) * (double)modelMaxPosition(model)/modelAngleRange(model) + 0.5;
    if (position > modelMaxPosition(model))
        position = modelMaxPosition(model);
    else if (position < 0)
        position = 0;
    return position;
}


/**
 * @brief throwPrintError
 * Throws or prints error message (depending on throwFlag/MSG_SETTING)
 * @param msg String message to be thrown/printed
 * @param throwFlag Boolean corresponding to mode of presenting error message (MSG_SETTING == THROW_ERROR being true)
 */
void handleError(const string& msg, bool throwFlag)
{
    if (throwFlag)
       throw runtime_error(msg);
    else
        cerr << msg << endl;
}

