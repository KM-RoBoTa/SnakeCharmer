#include <iostream>
#include <stdexcept>

#include "control_table_constants.h"
#include "dconstants.h"
#include "dmodeldetails.h"
#include "additionaltools.h"
#include "user_settings.h"

#include "dxmotorsystem.h"

using namespace std;

/**
 * @brief DXMotorSystem::DXMotorSystem
 * Constructor for DXMotorSystem
 * @param devicePort Designated port for interface
 * (dev/ttyUSB*, where * is the port number for <10, dev/ttySAC*, where * is the port number corresponding to devicePort%10)
 * @param dxlBaudRate Baud Rate to be set, according to Dynamixel's table for baud rate data
 */
DXMotorSystem::DXMotorSystem(int devicePort, int dxlBaudRate):DXSystem(devicePort, dxlBaudRate)
{
    if (isInitialized())
    {
        updateAllLimitsInfo();
        setAllHomePosition(getAllPosition());
        setAllStatusReturnLevel(DEFAULT_SRL);
        setAllDelay(DEFAULT_RETURN_DELAY_TIME);
    }
}

/**
 * @brief DXMotorSystem::~DXMotorSystem
 * Destructor for the DXMotorSystem Class
 */
DXMotorSystem::~DXMotorSystem()
{

}


// Limit Settings //

/**
 * @brief DXMotorSystem::updateLimitsInfo
 * Update internal limitInfo struct with mode and position/speed limits
 * @param motorNo Motor number (#)
 */
void DXMotorSystem::updateLimitsInfo(unsigned int motorNo)
{
    limitInfo p_info;
    p_info.maxPos = getPositionUpperLimit(motorNo);
    p_info.minPos = getPositionLowerLimit(motorNo);

    p_info.midPos = (p_info.maxPos + p_info.minPos + 1)/2;
    p_info.mode = (p_info.maxPos == 0 && p_info.minPos == 0) ? WHEEL_MODE : JOINT_MODE;

    p_info.maxSpd = modelMaxSpeed(allModel[motorNo], p_info.mode);
    p_info.minSpd = modelMinSpeed(allModel[motorNo], p_info.mode);


    if (motorNo < allLimits.size())
    {
        p_info.homePos = allLimits[motorNo].homePos;
        allLimits[motorNo] = p_info;
    }
    else if (motorNo == allLimits.size())
    {
        p_info.homePos = p_info.midPos;
        allLimits.push_back(p_info);
    }
    else
        handleError("Motor Number Out of Bounds in Updating Limits", MSG_SETTING == THROW_ERROR);
}

/**
 * @brief DXMotorSystem::updateAllLimitsInfo
 * Update all information of mode and limits to internal struct
 */
void DXMotorSystem::updateAllLimitsInfo()
{
    for (unsigned int ii = 0; ii < numMotors; ii++) {
        updateLimitsInfo(ii);
    }
}

/**
 * @brief DXMotorSystem::setAllHomePosition
 * Set the home position of all motors
 * @param homePositions Home positions for the motors (represented in an int vector corresponding to motor #)
 */
void DXMotorSystem::setAllHomePosition(vector<int> homePositions)
{
    for (unsigned int ii = 0; ii < allLimits.size(); ii++)
    {
        allLimits[ii].homePos = homePositions[ii];
    }
}

/**
 * @brief DXMotorSystem::getAllHomePosition
 * @return Home positions of the motors, represented in an int vector corresponding to motor #
 */
vector<int> DXMotorSystem::getAllHomePosition()
{
    vector<int> homePositions(numMotors,0);
    for (unsigned int ii = 0; ii < numMotors; ii++)
    {
        homePositions[ii] = allLimits[ii].homePos;
    }
    return homePositions;
}

/**
 * @brief DXMotorSystem::setPositionLimits
 * Set new position limits (CW and CCW) on the motor
 * @param motorNo Motor number (#)
 * @param lowerBound Desired CW angle limit (rightmost position limit)
 * @param upperBound Desired CCW angle limit (leftmost position limit)
 */
void DXMotorSystem::setPositionLimits(unsigned int motorNo, int lowerBound, int upperBound)
{
    set2Word(motorNo, P_CW_ANGLE_LIMIT_L, lowerBound, upperBound);
    delay(500); // update all motors before reading/changing internal records of limits
    updateLimitsInfo(motorNo);
}

/**
 * @brief DXMotorSystem::setAllPositionLimits
 * Set new position limits on all motors
 * @param lowerBound Desired CW angle limit (rightmost position limit) (represented in an int vector corresponding to motor #)
 * @param upperBound Desired CCW angle limit (leftmost position limit) (represented in an int vector corresponding to motor #)
 */
void DXMotorSystem::setAllPositionLimits(const vector<int>& lowerBounds, const vector<int>& upperBounds)
{
    setAll2Word(P_CW_ANGLE_LIMIT_L, lowerBounds, upperBounds);
    delay(500);  // update all motors before reading/changing internal records of limits
    updateAllLimitsInfo();

}

/**
 * @brief DXMotorSystem::getPositionLowerLimit
 * Get clockwise/minimum position limit on the motor
 * @param motorNo Motor number (#)
 * @return CW angle limit/minimum position limit of the motor
 */
int DXMotorSystem::getPositionLowerLimit(unsigned int motorNo)
{
    return readWord(motorNo, P_CW_ANGLE_LIMIT_L);
}

/**
 * @brief DXMotorSystem::getPositionUpperLimit
 * Get counterclockwise/maximum position limit on the motor
 * @param motorNo Motor number (#)
 * @return CCW angle limit/maximum position limit of the motor
 */
int DXMotorSystem::getPositionUpperLimit(unsigned int motorNo)
{
    return readWord(motorNo, P_CCW_ANGLE_LIMIT_L);
}

/**
 * @brief DXMotorSystem::setTorqueMax
 * Set maximum torque limit on the motor (absolute max, not to be confused with torque limit)
 * @param motorNo Motor number (#)
 * @param upperBound Desired maximum torque value for the motor
 */
void DXMotorSystem::setTorqueMax(unsigned int motorNo, int upperBound)
{
    setWord(motorNo, P_MAX_TORQUE_L, upperBound);
}

/**
 * @brief DXMotorSystem::setAllTorqueMax
 * Set torque limit on all motors (absolute max, not to be confused with torque limit)
 * @param upperBounds Desired maximum torque value for all motors (in an int vector corresponding to motor #)
 */
void DXMotorSystem::setAllTorqueMax(const vector<int>& upperBounds)
{
    setAllWord(P_MAX_TORQUE_L, upperBounds);
}

/**
 * @brief DXMotorSystem::getTorqueMax
 * Get maximum torque limit setting on the motor (absolute max, not to be confused with torque limit)
 * @param motorNo Motor number (#)
 * @return Maximum torque limit set on the motor
 */
int DXMotorSystem::getTorqueMax(unsigned int motorNo)
{
    return readWord(motorNo, P_MAX_TORQUE_L);
}

/**
 * @brief DXMotorSystem::setTorqueLimit
 * Set torque limit on the motor
 * @param motorNo Motor number (#)
 * @param upperBound Desired (goal) torque value for the motor
 */
void DXMotorSystem::setTorqueLimit(unsigned int motorNo, int upperBound)
{
    setWord(motorNo, P_TORQUE_LIMIT_L, upperBound);
}

/**
 * @brief DXMotorSystem::setAllTorqueLimit
 * Set torque limit on all motors
 * @param upperBounds Desired (goal) torque value for all motors (in an int vector corresponding to motor #)
 */
void DXMotorSystem::setAllTorqueLimit(const vector<int>& upperBounds)
{
    setAllWord(P_TORQUE_LIMIT_L, upperBounds);
}

/**
 * @brief DXMotorSystem::getTorqueLimit
 * Get current torque limit setting on the motor
 * @param motorNo Motor number (#)
 * @return Torque limit set on the motor
 */
int DXMotorSystem::getTorqueLimit(unsigned int motorNo)
{
    return readWord(motorNo, P_TORQUE_LIMIT_L);
}

/**
 * @brief DXMotorSystem::setTorqueEn
 * Set torque enable on the motor
 * @param motorNo Motor number (#)
 * @param enableVal TORQUE_ENABLE/TORQUE_DISABLE boolean
 */
void DXMotorSystem::setTorqueEn(unsigned int motorNo, bool enableVal)
{
    setByte(motorNo, P_TORQUE_ENABLE, enableVal);
}

/**
 * @brief DXMotorSystem::setAllTorqueEn
 * Set torque enable on all motors
 * @param enableVal TORQUE_ENABLE/TORQUE_DISABLE boolean (all motors will share the same torque enable value)
 */
void DXMotorSystem::setAllTorqueEn(bool enableVal)
{
    vector<int> allVal(numMotors, enableVal);
    setAllByte(P_TORQUE_ENABLE, allVal);
}

/**
 * @brief DXMotorSystem::getTorqueEn
 * Get torque enable setting of the motor
 * @param motorNo Motor number (#)
 * @return TORQUE_ENABLE/TORQUE_DISABLE boolean
 */
bool DXMotorSystem::getTorqueEn(unsigned int motorNo)
{
    return readByte(motorNo, P_TORQUE_ENABLE);
}

// Position and Speed Setter/Mover //

/**
 * @brief DXMotorSystem::movetoPosition
 * Move motor to goal position; Function waits until motor finishes moving
 * @param motorNo Motor number (#)
 * @param goalPosition Desired final position of the motor
 * @return Boolean denoting if position is successfully set
 */
void DXMotorSystem::movetoPosition(unsigned int motorNo, int goalPosition)
{
    setPosition(motorNo, goalPosition);
    while(isMoving(motorNo));
}

/**
 * @brief DXMotorSystem::setPosition
 * Set motor to goal position;
 * Error if motor is in wheel mode
 * @param motorNo Motor number (#)
 * @param goalPosition Desired final position of the motor
 */
void DXMotorSystem::setPosition(unsigned int motorNo, int goalPosition)
{
    if (allLimits[motorNo].mode == WHEEL_MODE)
        handleError("Motor is at Wheel Mode.", MSG_SETTING == THROW_ERROR);
    setWord(motorNo, P_GOAL_POSITION_L, goalPosition);
}

/**
 * @brief DXMotorSystem::setSpeed
 * Set motor to desired set speed
 * @param motorNo Motor number (#)
 * @param goalSpeed Desired set speed of the motor
 */
void DXMotorSystem::setSpeed(unsigned int motorNo, int goalSpeed)
{
    if (allLimits[motorNo].mode == WHEEL_MODE && goalSpeed < 0)
        goalSpeed = -goalSpeed + 1024;
    setWord(motorNo, P_MOVING_SPEED_L, goalSpeed);
}

/**
 * @brief DXMotorSystem::setAllPosition
 * Set all motors to goal positions
 * (Note: Function does not check for error; Motors possess their own checking mechanism when writing into RAM)
 * @param goalPositions Desired positions of the motors (given in an int vector corresponding to the motor #)
 */
void DXMotorSystem::setAllPosition(const vector<int>& goalPositions)
{
    setAllWord(P_GOAL_POSITION_L,goalPositions);
}

/**
 * @brief DXMotorSystem::setAllSpeed
 * Set all motors to desired set speeds
 * (Note: Function does not check for error; Motors possess their own checking mechanism when writing into RAM)
 * @param goalSpeeds Desired set speeds of the motors (given in an int vectors corresponding to the motor #)
 */
void DXMotorSystem::setAllSpeed(const vector<int>& goalSpeeds)
{
    setAllWord(P_MOVING_SPEED_L,goalSpeeds);
}


// Getters //

/**
 * @brief DXMotorSystem::getCurrentPosition
 * Get current position of the motor
 * @param motorNo Motor number (#)
 * @return Current position of the motor
 */
int DXMotorSystem::getPosition(unsigned int motorNo)
{
    return readWord(motorNo, P_PRESENT_POSITION_L);
}

/**
 * @brief DXMotorSystem::getCurrentSpeed
 * Get current moving speed of the motor
 * @param motorNo Motor number (#)
 * @return Current moving speed of the motor (at the instance of reading)
 */
int DXMotorSystem::getCurrentSpeed(unsigned int motorNo)
{
    return readWord(motorNo, P_PRESENT_SPEED_L);
}

/**
 * @brief DXMotorSystem::getSetSpeed
 * Get current speed setting of the motor
 * @param motorNo Motor number (#)
 * @return Current setting for speed of the motor (i.e. speed of the motor when moving)
 */
int DXMotorSystem::getSetSpeed(unsigned int motorNo)
{
    return readWord(motorNo, P_MOVING_SPEED_L);
}

/**
 * @brief DXMotorSystem::getAllPosition
 * Get positions of all motors
 * (Note: This function may not be robust, as the "current speeds" are read in serial, i.e. each motor's reading is off by a finite time difference)
 * @return All positions of the motors, represented in an int vector corresponding to motor #
 */
vector<int> DXMotorSystem::getAllPosition()
{
    return readAllWord(P_PRESENT_POSITION_L);
}

/**
 * @brief DXMotorSystem::getAllSetSpeed
 * Get speed setting of all motors
 * (Note: This function may not be robust, as the "current speeds" are read in serial, i.e. each motor's reading is off by a finite time difference)
 * @return All set speed of the motors (i.e. speed of motor when moving), represented in an int vector corresponding to motor #
 */
vector<int> DXMotorSystem::getAllSetSpeed()
{
    return readAllWord(P_MOVING_SPEED_L);
}

/**
 * @brief DXMotorSystem::getAllCurrentSpeed
 * Get current moving speed of all motors
 * (Note: This function may not be robust, as the "current speeds" are read in serial, i.e. each motor's reading is off by a finite time difference)
 * @return All current speed of the motors (i.e. speed at the "instance" of reading), represented in an int vector corresponding to motor #
 */
vector<int> DXMotorSystem::getAllCurrentSpeed()
{
    return readAllWord(P_PRESENT_SPEED_L);
}


// reset speed/pos //

/**
 * @brief DXMotorSystem::resetMovingSettings
 * Resets motor to speed 0 and home position
 * @param motorNo Motor number (#)
 */
void DXMotorSystem::resetMovingSettings(unsigned int motorNo)
{
    setSpeed(motorNo, 0);
    setPosition(motorNo, allLimits[motorNo].homePos);
}

/**
 * @brief DXMotorSystem::resetAllMovingSettings
 * Resets all motors to speed 0 and home position
 */
void DXMotorSystem::resetAllMovingSettings()
{
    for(unsigned int ii = 0; ii < numMotors; ii++)
        resetMovingSettings(ii);
}


// Queuers & Execute //

/**
 * @brief DXMotorSystem::queuePosition
 * Queue goal position for a motor;
 * This function only queues the value; DXSystem::executeQueue() is required to execute write
 * @param motorNo Motor number (#)
 * @param goalPosition Desired position of the motor
 */
void DXMotorSystem::queuePosition(unsigned int motorNo, int goalPosition)
{
    queueWord(motorNo, P_GOAL_POSITION_L, goalPosition);
}

/**
 * @brief DXMotorSystem::queueSpeed
 * Queue desired speed for a motor;
 * This function only queues the value; DXSystem::executeQueue() is required to execute write
 * @param motorNo Motor number (#)
 * @param goalSpeed Desired set speed of the motor
 */
void DXMotorSystem::queueSpeed(unsigned int motorNo, int goalSpeed)
{
    queueWord(motorNo, P_MOVING_SPEED_L, goalSpeed);
}


// Checking //

/**
 * @brief DXMotorSystem::isMoving
 * Check if motor is moving (based on EEPROM table)
 * @param motorNo Motor number (#)
 * @return Boolean denoting if the motor is moving at the instance of reading (true if moving, false otherwise)
 */
bool DXMotorSystem::isMoving(unsigned int motorNo)
{
    int moving = readByte(motorNo, P_MOVING);
    return moving ? true : false;
}

//----------//
// Punch //
 /**
 * @brief DXMotorSystem::setPunch
 * Set punch value to the motor
 * @param motorNo Motor number (#)
 * @param punch Desired punch for the motor
 */
void DXMotorSystem::setPunch(unsigned int motorNo, int punch)
{
    setWord(motorNo, P_PUNCH_L, punch);
}

/**
 * @brief DXMotorSystem::setAllPunch
 * Set punch values to all motors
 * @param punch Desired punch values for all motors (in an int vector corresponding to motor #)
 */
void DXMotorSystem::setAllPunch(const vector<int>& punch)
{
    setAllWord(P_PUNCH_L, punch);
}

/**
 * @brief DXMotorSystem::getPunch
 * Get punch value setting of the motor
 * @param motorNo Motor number (#)
 * @return Current setting for punch value
 */
int DXMotorSystem::getPunch(unsigned int motorNo)
{
    return readWord(motorNo, P_PUNCH_L);
}


/**
 * @brief DXMotorSystem::setGainP
 * Set P gain setting of the motor
 * @param motorNo Motor number (#)
 * @param pVal Desired P Gain for the motor
 */
void DXMotorSystem::setGainP(unsigned int motorNo, int pVal)
{
    if (pidCapable(allModel[motorNo]))
    {
        setByte(motorNo, P_P_GAIN, pVal);
    }
}

/**
 * @brief DXMotorSystem::setAllGainP
 * Set P gain settings of all motors;
 * Function does not check for error in int vector input
 * @param pVal Desired P Gains for the motors (in an int vector corresponding to motor #)
 */
void DXMotorSystem::setAllGainP(vector<int>& pVal)
{
    for (unsigned int ii = 0; ii < numMotors; ii++)
    {
        if (!pidCapable(allModel[ii]))
            pVal[ii] = readByte(ii, P_P_GAIN);
    }
    setAllByte(P_P_GAIN, pVal);
}

/**
 * @brief DXMotorSystem::getGainP
 * Get P gain setting of the motor
 * @param motorNo Motor number (#)
 * @return P gain value of the motor
 */
int DXMotorSystem::getGainP(unsigned int motorNo)
{
    if (pidCapable(allModel[motorNo]))
    {
        return readByte(motorNo, P_P_GAIN);
    }
    return -1;
}

/**
 * @brief DXMotorSystem::setGainI
 * Set I gain setting of the motor
 * @param motorNo Motor number (#)
 * @param iVal Desired I Gain for the motor
 */
void DXMotorSystem::setGainI(unsigned int motorNo, int iVal)
{
    if (pidCapable(allModel[motorNo]))
    {
        setByte(motorNo, P_I_GAIN, iVal);
    }
}

/**
 * @brief DXMotorSystem::setAllGainI
 * Set I gain settings of all motors;
 * Function does not check for error in int vector input
 * @param iVal Desired I Gains for the motors (in an int vector corresponding to motor #)
 */
void DXMotorSystem::setAllGainI(vector<int>& iVal)
{
    for (unsigned int ii = 0; ii < numMotors; ii++)
    {
        if (!pidCapable(allModel[ii]))
            iVal[ii] = readByte(ii, P_I_GAIN);
    }
    setAllByte(P_I_GAIN, iVal);
}

/**
 * @brief DXMotorSystem::getGainI
 * Get I gain setting of the motor
 * @param motorNo Motor number (#)
 * @return I gain value of the motor
 */
int DXMotorSystem::getGainI(unsigned int motorNo)
{
    if (pidCapable(allModel[motorNo]))
    {
        return readByte(motorNo, P_I_GAIN);
    }
    return -1;
}

/**
 * @brief DXMotorSystem::setGainD
 * Set D gain setting of the motor
 * @param motorNo Motor number (#)
 * @param dVal Desired D Gain for the motor
 */
void DXMotorSystem::setGainD(unsigned int motorNo, int dVal)
{
    if (pidCapable(allModel[motorNo]))
    {
        setByte(motorNo, P_D_GAIN, dVal);
    }
}

/**
 * @brief DXMotorSystem::setAllGainD
 * Set D gain settings of all motors;
 * Function does not check for error in int vector input
 * @param dVal Desired D Gains for the motors (in an int vector corresponding to motor #)
 */
void DXMotorSystem::setAllGainD(vector<int>& dVal)
{
    for (unsigned int ii = 0; ii < numMotors; ii++)
    {
        if (!pidCapable(allModel[ii]))
            dVal[ii] = readByte(ii, P_D_GAIN);
    }
    setAllByte(P_D_GAIN, dVal);
}

/**
 * @brief DXMotorSystem::getGainD
 * Get D gain setting of the motor
 * @param motorNo Motor number (#)
 * @return D gain value of the motor
 */
int DXMotorSystem::getGainD(unsigned int motorNo)
{
    if (pidCapable(allModel[motorNo]))
    {
        return readByte(motorNo, P_D_GAIN);
    }
    return -1;
}

/**
 * @brief DXMotorSystem::setComplianceMargin
 * Set Compliance Margin for the motor
 * @param motorNo Motor number (#)
 * @param cwMargin CW Margin
 * @param ccwMargin CCW Margin
 */
void DXMotorSystem::setComplianceMargin(unsigned int motorNo, int cwMargin, int ccwMargin)
{
    if (!complianceSettingCapable(allModel[motorNo]))
        return;
    setWord(motorNo,P_CW_COMPLIANCE_MARGIN, dclass.dxl_makeword(cwMargin, ccwMargin));
}

/**
 * @brief DXMotorSystem::setComplianceSlope
 * Set Compliance Slope for the motor
 * @param motorNo Motor number (#)
 * @param cwSlope CW Slope
 * @param ccwSlope CCW Slope
 */
void DXMotorSystem::setComplianceSlope(unsigned int motorNo, int cwSlope, int ccwSlope)
{
    if (!complianceSettingCapable(allModel[motorNo]))
        return;
    setWord(motorNo,P_CW_COMPLIANCE_SLOPE, dclass.dxl_makeword(cwSlope, ccwSlope));
}

/**
 * @brief DXMotorSystem::setAllComplianceMargin
 * Set Compliance Margins for all motors
 * @param cwMargin CW Margins (in an int vector corresponding to motor #)
 * @param ccwMargin CCW Margins (in an int vector corresponding to motor #)
 */
void DXMotorSystem::setAllComplianceMargin(const vector<int>& cwMargin, const vector<int>& ccwMargin)
{
    vector<int> marginVal (numMotors, 0);
    for (unsigned int ii = 0; ii < numMotors; ii++)
    {
        if (!complianceSettingCapable(allModel[ii]))
            marginVal[ii] = readWord(ii, P_CW_COMPLIANCE_MARGIN);
        marginVal[ii] = dclass.dxl_makeword(cwMargin[ii], ccwMargin[ii]);
    }
    setAllWord(P_CW_COMPLIANCE_MARGIN, marginVal);
}

/**
 * @brief DXMotorSystem::setAllComplianceSlope
 * Set Compliance Slopes for all motors
 * @param cwSlope CW Slopes (in an int vector corresponding to motor #)
 * @param ccwSlope CCW Slopes (in an int vector corresponding to motor #)
 */
void DXMotorSystem::setAllComplianceSlope(const vector<int>& cwSlope, const vector<int>& ccwSlope)
{
    vector<int> marginVal (numMotors, 0);
    for (unsigned int ii = 0; ii < numMotors; ii++)
    {
        if (!complianceSettingCapable(allModel[ii]))
            marginVal[ii] = readWord(ii, P_CW_COMPLIANCE_SLOPE);
        marginVal[ii] = dclass.dxl_makeword(cwSlope[ii], ccwSlope[ii]);
    }
    setAllWord(P_CW_COMPLIANCE_SLOPE, marginVal);
}

/**
 * @brief DXMotorSystem::getComplianceMarginCW
 * Get CW compliance margin
 * @param motorNo Motor (#)
 * @return CW Compliance Margin value
 */
int DXMotorSystem::getComplianceMarginCW(unsigned int motorNo)
{
    if (complianceSettingCapable(allModel[motorNo]))
        return readByte(motorNo, P_CW_COMPLIANCE_MARGIN);
    return -1;
}

/**
 * @brief DXMotorSystem::getComplianceMarginCCW
 * Get CCW compliance margin
 * @param motorNo Motor (#)
 * @return CCW Compliance Margin value
 */
int DXMotorSystem::getComplianceMarginCCW(unsigned int motorNo)
{
    if (complianceSettingCapable(allModel[motorNo]))
        return readByte(motorNo, P_CCW_COMPLIANCE_MARGIN);
    return -1;
}

/**
 * @brief DXMotorSystem::getComplianceSlopeCW
 * Get CW compliance slope
 * @param motorNo Motor (#)
 * @return CW Compliance Slope value
 */
int DXMotorSystem::getComplianceSlopeCW(unsigned int motorNo)
{
    if (complianceSettingCapable(allModel[motorNo]))
        return readByte(motorNo, P_CW_COMPLIANCE_SLOPE);
    return -1;
}

/**
 * @brief DXMotorSystem::getComplianceSlopeCCW
 * Get CCW compliance slope
 * @param motorNo Motor (#)
 * @return CCW Compliance Slope value
 */
int DXMotorSystem::getComplianceSlopeCCW(unsigned int motorNo)
{
    if (complianceSettingCapable(allModel[motorNo]))
        return readByte(motorNo, P_CCW_COMPLIANCE_SLOPE);
    return -1;
}
