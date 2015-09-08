#ifndef DXSYSTEM_H
#define DXSYSTEM_H

#include <string>
#include <vector>

#include "dxllibclass/dynamixelclass.h"

using namespace std;

class DXSystem
{
public:
    /** @defgroup dxsystem DXSystem
     * This class controls the settings of a system of dynamixel motors.
     * DXSystem is the superclass of DXMotorSystem (control motor motion) and DXSetupMotor (reset motor settings).
     * @{
     */
    DXSystem(int devicePort, int dxlBaudRate); // Constructor
    ~DXSystem(); // Destructor

    bool isInitialized(); // Check if initialized
    bool isQueued(unsigned int motorNo); // Check if queued/registered
    void identifyMotors();
    void reopenPort(int dxlBaudRate);
    void lockEEPROM();

    /**
     * @defgroup statusGetter Motor Status Getters
     * For reading current load, temperature and voltage status.
     * @{
     */
    int getLoad(unsigned int motorNo);
    vector<int> getAllLoad();
    int getTemperature(unsigned int motorNo);
    int getVoltage(unsigned int motorNo);
    /** @} */

    /**
     * @defgroup configSettings Motor Configuration Settings
     * For handling motor settings (max temperature, voltage limits, delay time and status return level)
     * @{
     */
    void setMaxTemperature(unsigned int motorNo, int upperBound);
    void setAllMaxTemperature(const vector<int>& upperBounds);
    int getMaxTemperature(unsigned int motorNo);

    void setVoltageLimits(unsigned int motorNo, int lowerBound, int upperBound);
    void setAllVoltageLimits(const vector<int>& lowerBounds, const vector<int>& upperBounds);
    int getMaxVoltage(unsigned int motorNo);
    int getMinVoltage(unsigned int motorNo);

    void setAllDelay(int newDelayTime);
    int getDelay(unsigned int motorNo);

    void setAllStatusReturnLevel(int newLevel);
    int getStatusReturnLevel(unsigned int motorNo);

    void setAllBaudRate(int newBaudRate);
    int getBaudRate(unsigned int motorNo);
    /** @} */

    /**
     * @defgroup refInfo Motor Referencing Information
     * For obtaining information relating to ID/motor #/model/number of motors within a single DXSystem object
     * @{
     */
    int id2MotorNum(int dxlID);
    int motorNum2id(int motorNo);
    int getModel(int motorNo);
    unsigned int getNumMotors();
    /** @} */

    /**
     * @defgroup manual Manual Adjustments
     * For manual adjustment of byte/word on EEPROM (requires knowledge of EEPROM addresses and corresponding limits/valid inputs)
     * @{
     */
    void setByte(unsigned int motorNo, int addr, int value);
    int readByte(unsigned int motorNo, int addr);
    void setWord(unsigned int motorNo, int addr, int value);
    int readWord(unsigned int motorNo, int addr);
    void set2Word(unsigned int motorNo, int addr, int value1, int value2);
    void queueByte(unsigned int motorNo, int addr, int value);
    void queueWord(unsigned int motorNo, int addr, int value);
    void executeQueue();

    void setAllByte(int addr, const vector<int>& value);
    void setAllWord(int addr, const vector<int>& value);
    void setAll2Word(int addr, const vector<int>& value1, const vector<int>& value2);

    vector<int> readAllByte(int addr);
    vector<int> readAllWord(int addr);
    /** @} */

protected:
    /**
     * @defgroup protected Protected
     * Variables and functions that are shared across DXSystem class and subclasses.
     * @{
     */
    /** @brief Number of motors in the system (internal tracker) */
    unsigned int numMotors;
    /** @brief allID All native IDs of the motors, mapped to motor # (internal tracker) */
    vector<int> allID; // Tracks all internal ID
    vector<bool> bulkreadFlag;
    /** @brief allModel All model of the motors (internal tracker) */
    vector<int> allModel; // Tracks all model given

    bool checkCommError(); // Check error + communication status, prints msg
    /** @brief systemSRL Current SRL of the system */
    int systemSRL; // Status Return Level of the system
    /** @brief Object instance of the dynamixel API, for multiple USB devices manipulation */
    dynamixelClass dclass;
    /** @} */

private:
// Private function and information enclosed within DXSystem Class
    int devPort; // Device Port
    int baudRate; // Baud Rate Setting - default to 1, i.e. 1Mbps
    bool initFlag; // Initialize flag: 1 if initialized, 0 otherwise
    void initMotors(); // Initialize all motors (Search all available IDs)

    bool checkinRange(unsigned int motorNo); // Check if motor # supplied is valid


    /**
     * @brief The mStatus struct houses the error information, message and boolean flag.
     * Mainly used for internal checking of communication and error status.
     */
    struct mStatus {
        bool flag;
        string msg;
        int info;
    };
    mStatus checkError(); // Check error bits
    mStatus checkCommStatus(); // Check communication status
    bool checkBulkCommError();

    /** @} */
};

#endif // DXSYSTEM_H
