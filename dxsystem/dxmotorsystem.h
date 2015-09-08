#ifndef DXMOTORSYSTEM_H
#define DXMOTORSYSTEM_H

#include "dxsystem.h"

class DXMotorSystem : public DXSystem
{
public:
    /** @defgroup dxmotorsystem DXMotorSystem
     * This class extends DXSystem and controls the motion settings of a system of dynamixel motors (position, speed and PID gain)
     * @{
     */
    DXMotorSystem(int devicePort, int dxlBaudRate);
    ~DXMotorSystem();

    /**
     * @defgroup motorLimits Motor Motion Limit Settings
     * For handling position and torque limits set on motors
     * @{
     */
    void setAllHomePosition(vector<int> homePositions);
    vector<int> getAllHomePosition();

    void setPositionLimits(unsigned int motorNo, int lowerBound, int upperBound);
    void setAllPositionLimits(const vector<int>& lowerBounds, const vector<int>& upperBounds);
    int getPositionLowerLimit(unsigned int motorNo);
    int getPositionUpperLimit(unsigned int motorNo);

    void setTorqueLimit(unsigned int motorNo, int upperBound);
    void setAllTorqueLimit(const vector<int>& upperBounds);
    int getTorqueLimit(unsigned int motorNo);

    void setTorqueMax(unsigned int motorNo, int upperBound);
    void setAllTorqueMax(const vector<int>& upperBounds);
    int getTorqueMax(unsigned int motorNo);

    void setTorqueEn(unsigned int motorNo, bool enableVal);
    void setAllTorqueEn(bool enableVal);
    bool getTorqueEn(unsigned int motorNo);
    /** @} */

    /**
     * @defgroup pssetter Setters and Getters for Position and Speed
     * For handling position and speed information (current/goal position and current/set speed)
     * @{
     */
    void queuePosition(unsigned int motorNo, int goalPosition);
    void setPosition(unsigned int motorNo, int goalPosition);
    void setAllPosition(const vector<int>& goalPositions);
    void movetoPosition(unsigned int motorNo, int goalPosition);
    int getPosition(unsigned int motorNo);
    vector<int> getAllPosition();

    void queueSpeed(unsigned int motorNo, int goalSpeed);
    void setSpeed(unsigned int motorNo, int goalSpeed);
    void setAllSpeed(const vector<int>& goalSpeeds);
    int getCurrentSpeed(unsigned int motorNo);
    int getSetSpeed(unsigned int motorNo);
    vector<int> getAllSetSpeed();
    vector<int> getAllCurrentSpeed();

    bool isMoving(unsigned int motorNo);
    void resetMovingSettings(unsigned int motorNo);
    void resetAllMovingSettings();
    /** @} */


    /**
     * @defgroup forcesetters Setters and Getters Compliance/PID Control
     * For handling Compliance/PID settings for fine control
     * Note: PID is only for MX28, MX64 and MX106 - where pidCapable(model) == true
     * Note: Compliance Slope/Margin is only for AX12 and RX28 - where complianceSettingCapable(mode) == true
     * @{
     */
    // Punch
    void setPunch(unsigned int motorNo, int punch);
    void setAllPunch(const vector<int>& punch);
    int getPunch(unsigned int motorNo);

    // PID
    // Only for MX
    void setGainP(unsigned int motorNo, int pVal);
    void setAllGainP(vector<int>& pVal);
    int getGainP(unsigned int motorNo);

    void setGainI(unsigned int motorNo, int iVal);
    void setAllGainI(vector<int>& iVal);
    int getGainI(unsigned int motorNo);

    void setGainD(unsigned int motorNo, int dVal);
    void setAllGainD(vector<int>& dVal);
    int getGainD(unsigned int motorNo);

    // Compliance Slope/Margin
    // Only for AX/RX
    void setComplianceSlope(unsigned int motorNo, int cwSlope, int ccwSlope);
    void setComplianceMargin(unsigned int motorNo, int cwMargin, int ccwMargin);
    void setAllComplianceSlope(const vector<int>& cwSlope, const vector<int>& ccwSlope);
    void setAllComplianceMargin(const vector<int>& cwMargin, const vector<int>& ccwMargin);
    int getComplianceSlopeCW(unsigned int motorNo);
    int getComplianceSlopeCCW(unsigned int motorNo);
    int getComplianceMarginCW(unsigned int motorNo);
    int getComplianceMarginCCW(unsigned int motorNo);
    /** @} */


private:
// Private function and information enclosed within DXMotorSystem Class
    /**
     * @brief These functions are called when limitInfo should be updated
     */
    void updateLimitsInfo(unsigned int motorNo);
    void updateAllLimitsInfo();
    /**
     * @brief The limitInfo struct holds information about the mode and max/min position and speed limits for each motor
     */
    struct limitInfo {
        int mode;
        int homePos;
        int midPos;
        int minPos;
        int maxPos;
        int minSpd;
        int maxSpd;
    };
    /**
     * @brief allLimits Internal reference to save modes and Position/Speed limits (minimize amount of communication necessary)
     */
    vector<limitInfo> allLimits;

    /** @} */
};

#endif // DXMOTORSYSTEM_H
