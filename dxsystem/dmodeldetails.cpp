#include "control_table_constants.h"
#include "dconstants.h"
#include "user_settings.h"

#include "dmodeldetails.h"

/**
 * @brief modelMiddlePosition
 * Returns the data value corresponding to the model's middle position
 * @param model Model number (found on motor's EEPROM)
 * @return Data value of the middle position
 */
int modelMiddlePosition(int model)
{
    switch(model)
    {
    case 0xC:
    case 0x12:
    case 0x12C:
    case 0xA:
    case 0x18:
    case 0x1C:
    case 0x40:
        return 512;
    case 0x168:
    case 0x1D:
    case 0x136:
    case 0x140:
        return 2048;
    default:
        return -1;
    }
}

/**
 * @brief modelMaxPosition
 * Returns the data value corresponding to the model's maximum position
 * @param model Model number (found on motor's EEPROM)
 * @return Data value of the maximum position
 */
int modelMaxPosition(int model)
{
    switch(model)
    {
    case 0xC:
    case 0x12:
    case 0x12C:
    case 0xA:
    case 0x18:
    case 0x1C:
    case 0x40:
        return 1023;
    case 0x168:
    case 0x1D:
    case 0x136:
    case 0x140:
        return 4095;
    default:
        return -1;
    }
}

/**
 * @brief modelMinPosition
 * Returns the data value corresponding to the model's minimum position
 * @param model Model number (found on motor's EEPROM)
 * @return Data value of the minimum position
 */
int modelMinPosition(int model)
{
    switch(model)
    {
    case 0xC:
    case 0x12:
    case 0x12C:
    case 0xA:
    case 0x18:
    case 0x1C:
    case 0x40:
    case 0x168:
    case 0x1D:
    case 0x136:
    case 0x140:
        return 0;
    default:
        return -1;
    }
}

/**
 * @brief modelMaxSpeed
 * Returns the data value corresponding to the model's maximum speed
 * @param model Model number (found on motor's EEPROM)
 * @param mode Mode - JOINT_MODE or WHEEL_MODE
 * @return Data value of the maximum speed
 */
int modelMaxSpeed(int model, int mode)
{
    switch(model)
    {
    case 0xC:
    case 0x12:
    case 0x12C:
    case 0xA:
    case 0x18:
    case 0x1C:
    case 0x40:
    case 0x168:
    case 0x1D:
    case 0x136:
    case 0x140:
        if (mode == JOINT_MODE)
            return 1023; // Joint Mode
        if (mode == WHEEL_MODE)
            return 2047; // Wheel Mode
    default:
        return -1;
    }
}


/**
 * @brief modelMinSpeed
 * Returns the data value corresponding to the model's minimum speed
 * @param model Model number (found on motor's EEPROM)
 * @param mode Mode - JOINT_MODE or WHEEL_MODE
 * @return Data value of the minimum speed
 */
int modelMinSpeed(int model, int mode)
{
    switch(model)
    {
    case 0xC:
    case 0x12:
    case 0x12C:
    case 0xA:
    case 0x18:
    case 0x1C:
    case 0x40:
    case 0x168:
    case 0x1D:
    case 0x136:
    case 0x140:
        if (mode == JOINT_MODE)
            return 1; // Joint Mode
        if (mode == WHEEL_MODE)
            return 0; // Wheel Mode
    default:
        return -1;
    }
}

/**
 * @brief modelAngleRange
 * Returns the angle range of the motor model (corresponds to the angle at maximum position)
 * @param model Model number (found on motor's EEPROM)
 * @return Angle range
 */
int modelAngleRange(int model)
{
    switch(model)
    {
    case 0xC:
    case 0x12:
    case 0x12C:
    case 0xA:
    case 0x18:
    case 0x1C:
    case 0x40:
        return 300;
    case 0x168:
    case 0x1D:
    case 0x136:
    case 0x140:
        return 360;
    default:
        return -1;
    }
}

/**
 * @brief modelMaxRPM
 * Returns the RPM (speed) of the motor model that corresponds to speed data value 1023
 * @param model Model number (found on motor's EEPROM)
 * @return "Maximum" RPM (the speed corresponding to 1023, based on documentation)
 */
int modelMaxRPM(int model)
{
    switch(model)
    {
    case 0xC:
    case 0x12:
    case 0x12C:
    case 0xA:
    case 0x18:
    case 0x1C:
    case 0x40:
        return 114;
    case 0x168:
    case 0x1D:
    case 0x136:
    case 0x140:
        return 117;
    default:
        return -1;
    }
}

/**
 * @brief modelDefaultBaudRate
 * Checks the default baud rate upon factory reset
 * @param model Model number (found on motor's EEPROM)
 * @return Data value corresponding to motor's default baud rate upon reset
 */
int modelDefaultBaudRate(int model)
{
    switch(model)
    {
    case 0xC:
    case 0x12:
    case 0x12C:
    case 0x168:
        return 1;
    case 0xA:
    case 0x18:
    case 0x1C:
    case 0x40:
    case 0x1D:
    case 0x136:
    case 0x140:
        return 34;
    default:
        return -1;
    }
}

/**
 * @brief pidCapable
 * Checks if motor model is capable of PID control
 * @param model Model number (found on motor's EEPROM)
 * @return True if PID capable, false otherwise
 */
bool pidCapable(int model)
{
    switch(model)
    {
    case 0x168:
    case 0x1D:
    case 0x136:
    case 0x140:
        return true;
    default:
        return false;
    }
}

/**
 * @brief complianceSettingCapable
 * Checks if motor model is capable of compliance settings (slope/margin)
 * @param model Model number (found on motor's EEPROM)
 * @return True if capable of compliance settings, false otherwise
 */
bool complianceSettingCapable(int model)
{
    switch(model)
    {
    case 0xC:
    case 0x12:
    case 0x12C:
    case 0xA:
    case 0x18:
    case 0x1C:
    case 0x40:
        return true;
    default:
        return false;
    }
}



/**
 * @brief bulkreadCapable
 * Checks if motor model is capable of BULK_READ instruction
 * @param model Model number (found on motor's EEPROM)
 * @return True if capable of bulk read (i.e. MX series), false otherwise
 */
bool bulkreadCapable(int model)
{
    switch(model)
    {
    case 0x168:
    case 0x1D:
    case 0x136:
    case 0x140:
        return true;
    default:
        return false;
    }
}
