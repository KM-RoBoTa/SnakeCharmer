#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>

#include "control_table_constants.h"
#include "dconstants.h"
#include "dmodeldetails.h"
#include "additionaltools.h"
#include "user_settings.h"

#include "dxsystem.h"

/**
 * @brief DXSystem::DXSystem
 * Constructor for DXSystem
 * @param devicePort Designated port for interface
 * (dev/ttyUSB*, where * is the port number)
 * @param dxlBaudRate Baud Rate to be set, according to Dynamixel's table for baud rate data
 */
DXSystem::DXSystem(int devicePort, int dxlBaudRate)
{
    initFlag = 0;
    baudRate = dxlBaudRate;
    devPort = devicePort;
    if( dclass.dxl_initialize(devicePort, baudRate)) {
        initFlag = 1;
        initMotors();
        int systemSRL = DEFAULT_SRL;
        setAllStatusReturnLevel(systemSRL);
    }
}
/**
 * @brief DXSystem::~DXSystem
 * Destructor for the DXSystem Class, Terminates DXL system
 */
DXSystem::~DXSystem()
{
    dclass.dxl_terminate();
}

/**
 * @brief DXSystem::initMotors
 * Initializes motors by searching all valid IDs (0 to 252) on the designated baud rate;
 * Saves all available dynamixel motors into the DXSystem class;
 * Translates the native ID of each motor into a corresponding "motor number";
 * Initialization assumes that all motors have a unique ID attached
 */
void DXSystem::initMotors()
{
    for (unsigned int ii = 0; ii < 252; ii++) {
        dclass.dxl_ping(ii);
        if (dclass.dxl_get_result() == COMM_RXSUCCESS) {
            int model = dclass.dxl_read_word(ii,P_MODEL_NO_L);
            allModel.push_back(model);
            allID.push_back(ii);
            bulkreadFlag.push_back(bulkreadCapable(model));
        }
    }
    numMotors = allID.size();
}

/**
 * @brief DXSystem::isInitialized
 * Used to check if system of dynamixel motors is properly initialized
 * @return Boolean "Initialized Flag", true if successfully initialized, false otherwise
 */
bool DXSystem::isInitialized()
{
    return initFlag;
}

/**
 * @brief DXSystem::isQueued
 * Used to check if instructions has been queued for the requested motor
 * @param motorNo Motor number (#)
 * @return Boolean "Registered Flag", true if registered (instruction has been queued), false otherwise
 */
bool DXSystem::isQueued(unsigned int motorNo)
{
    return readByte(motorNo, P_REGISTERED);
}

/**
  * @brief DXSystem::identifyMotors
  * Function to help in identifying motors and associating them with their corresponding number;
  * Console will print the motor number and light up the LED of the corresponding motor;
  * Aids in motor identification and troubleshooting (especially if there are non-unique ID conflicts)
  */
 void DXSystem::identifyMotors()
{
     cout << "There are " << numMotors << " motors." << endl;
     for (unsigned int ii = 0; ii < numMotors; ii++) {
         cout << "This is Motor #" << ii << "." << endl;
         setByte(ii,P_LED,1);
         cout << "Press Enter to Continue:";
         string s;
         getline(cin, s);
         setByte(ii,P_LED,0);
     }
     cout << "--End--" << endl;
 }

/**
 * @brief DXSystem::reopenPort
 * Reopens port - used when baud rate of motors have been changed.
 * @param dxlBaudRate New baud rate set; changes port to communicate at new baud rate
 */
void DXSystem::reopenPort(int dxlBaudRate)
{
    baudRate = dxlBaudRate;
    dclass.dxl_terminate();
    if (!dclass.dxl_initialize(devPort, baudRate))
        handleError("Error in reopening port", MSG_SETTING == THROW_ERROR);
}


/**
 * @brief DXSystem::lockEEPROM
 * Locks the settings within the EEPROM
 */
void DXSystem::lockEEPROM()
{
    vector<int> allLock(numMotors, 1);
    setAllByte(P_LOCK,allLock);
}

// Motor Status Getters //

/**
 * @brief DXSystem::getLoad
 * Reads current load on the motor
 * @param motorNo Motor number (#)
 * @return Current load (data value) of the motor
 */
int DXSystem::getLoad(unsigned int motorNo)
{
    return readWord(motorNo, P_PRESENT_LOAD_L);
}

vector<int> DXSystem::getAllLoad()
{
    vector<int> allLoad = readAllWord(P_PRESENT_LOAD_L);
    for (unsigned int ii = 0; ii < numMotors; ii++)
    {
        allLoad[ii] = allLoad[ii]%1024; // Only get the "absolute" value 0-1023
    }
    return allLoad;
}

/**
 * @brief DXSystem::getTemperature
 * Reads current temperature on the motor
 * @param motorNo Motor number (#)
 * @return Current temperature of the motor
 */
int DXSystem::getTemperature(unsigned int motorNo)
{
    return readByte(motorNo, P_PRESENT_TEMPERATURE);
}

/**
 * @brief DXSystem::getVoltage
 * Reads current voltage on the motor
 * @param motorNo Motor number (#)
 * @return Current voltage (data value) of the motor
 */
int DXSystem::getVoltage(unsigned int motorNo)
{
    return readByte(motorNo, P_PRESENT_VOLTAGE);
}


// Motor Configuration Settings //

/**
 * @brief DXSystem::setMaxTemperature
 * Sets the maximum operating temperature for the motor
 * (The max temperature should be set at 80oC. Change at your own risk.)
 * @param motorNo Motor number (#)
 * @param upperBound Desired maximum temperature limit for the motor
 */
void DXSystem::setMaxTemperature(unsigned int motorNo, int upperBound)
{
    cout << "Recommended: 70 - 80, depending on motor model (You should not change the internal limit temperature)" << endl;
    if (upperBound != DEFAULT_TEMP_LIMIT) {
        cout << "Input 'Y' to continue:";
        if (!getYorN())
            return;
    }
    setByte(motorNo, P_TEMP_HIGH_LIMIT, upperBound);
}

/**
 * @brief DXSystem::setAllMaxTemperature
 * Sets the maxmum operating temperature for all motors
 * (The max temperature should be set at 80oC. Change at your own risk.)
 * @param upperBound Desired maximum temperature limit for the motor  (in an int vector corresponding to motor number (#))
 */
void DXSystem::setAllMaxTemperature(const vector<int>& upperBounds)
{
    cout << "Recommended: 70 - 80, depending on motor model (You should not change the internal limit temperature)" << endl;
    for (unsigned int ii = 0; ii < numMotors; ii++) {
        if (upperBounds[ii] != DEFAULT_TEMP_LIMIT) {
            cout << "Input 'Y' to continue:";
            if (!getYorN())
                return;
            else
                break;
        }
    }
    setAllByte(P_TEMP_HIGH_LIMIT, upperBounds);
}

/**
 * @brief DXSystem::getMaxTemperature
 * Gets the maximum operating temperature of the motor
 * @param motorNo Motor number (#)
 * @return Max temperature limit of the motor
 */
int DXSystem::getMaxTemperature(unsigned int motorNo)
{
    return readByte(motorNo, P_TEMP_HIGH_LIMIT);
}

/**
 * @brief DXSystem::setVoltageLimits
 * Sets the margins of operating voltage for the motor
 * @param motorNo Motor number (#)
 * @param lowerBound Desired lower voltage limit for the motor
 * @param upperBound Desired upper voltage limit for the motor
 */
void DXSystem::setVoltageLimits(unsigned int motorNo, int lowerBound, int upperBound)
{
    setWord(motorNo, P_VOLT_LOW_LIMIT, dclass.dxl_makeword(lowerBound, upperBound));
}

/**
 * @brief DXSystem::setAllVoltageLimits
 * Sets the margins of operating voltage for all motors
 * @param lowerBound Desired lower voltage limits for the motor (in an int vector corresponding to motor number (#))
 * @param upperBound Desired upper voltage limits for the motor (in an int vector corresponding to motor number (#))
 */
void DXSystem::setAllVoltageLimits(const vector<int>& lowerBounds, const vector<int>& upperBounds)
{
    vector<int> bounds;
    for (unsigned int ii = 0; ii < numMotors; ii++) {
        bounds.push_back(dclass.dxl_makeword(lowerBounds[ii],upperBounds[ii]));
    }
    setAllWord(P_VOLT_LOW_LIMIT,bounds);
}

/**
 * @brief DXSystem::getMaxVoltage
 * Gets the maximum operating voltage of the motor
 * @param motorNo Motor number (#)
 * @return Max voltage limit of the motor
 */
int DXSystem::getMaxVoltage(unsigned int motorNo)
{
    return readByte(motorNo, P_VOLT_HIGH_LIMIT);
}

/**
 * @brief DXSystem::getMinVoltage
 * Gets the minimum operating voltage of the motor
 * @param motorNo Motor number (#)
 * @return Min voltage limit of the motor
 */
int DXSystem::getMinVoltage(unsigned int motorNo)
{
    return readByte(motorNo, P_VOLT_LOW_LIMIT);
}

/**
 * @brief DXSystem::setAllDelay
 * Sets the delay time between instruction and status packet for all motors
 * @param newDelayTime Desired delay time for all motors (all motors will share the same delay time)
 */
void DXSystem::setAllDelay(int newDelayTime)
{
    vector<int> allVal(numMotors, newDelayTime);
    setAllByte(P_RETURN_DELAY_TIME,allVal);
}

/**
 * @brief DXSystem::getDelay
 * Gets the current delay time setting between instruction and status packet of the motor
 * @param motorNo Motor number (#)
 * @return Current delay time setting of the motor
 */
int DXSystem::getDelay(unsigned int motorNo)
{
    return readByte(motorNo, P_RETURN_DELAY_TIME);
}

/**
 * @brief DXSystem::setAllStatusReturnLevel
 * Sets the Status Return Level for all motors
 * (SRL_ALL to return all (except when writing with BROADCAST_ID),
 * SRL_READ_ONLY to return only when READ commands are called
 * and SRL_NONE to never return (except with PING command))
 * @param newLevel Desired Status Return Level for all motors (all motors will share the same level)
 */
void DXSystem::setAllStatusReturnLevel(int newLevel)
{
    if (newLevel == 0)
    {
        cout << "Setting SRL to 0 will result in the inability to read the registers from the motors. Continue? ";
        if (!getYorN())
            return;
    }
    vector<int> allVal(numMotors, newLevel);
    setAllByte(P_STATUS_RETURN_LEVEL,allVal);
    systemSRL = newLevel;
}

/**
 * @brief DXSystem::getStatusReturnLevel
 * Gets the current Status Return Level setting of the motor
 * @param motorNo Motor number (#)
 * @return Current Status Return Level Setting of the motor
 */
int DXSystem::getStatusReturnLevel(unsigned int motorNo)
{
    return readByte(motorNo, P_STATUS_RETURN_LEVEL);
}

/**
 * @brief DXSystem::setAllBaudRate
 * Sets the baud rate for all motors
 * (Should be avoided while system is in use)
 * @param newBaudRate Desired baud rate setting for all motors (all motors will share the same baud rate setting)
 */
void DXSystem::setAllBaudRate(int newBaudRate)
{
    if (newBaudRate == baudRate)
        return;
    vector<int> allVal(numMotors, newBaudRate);
    setAllByte(P_BAUD_RATE,allVal);
    delay(10);
    reopenPort(newBaudRate);
}

/**
 * @brief DXSystem::getBaudRate
 * Gets the current baud rate setting of the motor
 * @param motorNo Motor number (#)
 * @return Current baud rate setting of the motor
 */
int DXSystem::getBaudRate(unsigned int motorNo)
{
    return readByte(motorNo, P_BAUD_RATE);
}

// Motor Reference Info //

/**
 * @brief DXSystem::id2MotorNum
 * Maps native ID (within EEPROM) to motor number (in DXSystem object)
 * @param dxlID Native motor ID
 * @return Corresponding motor number (#)
 */
int DXSystem::id2MotorNum(int dxlID)
{
    for (unsigned int ii = 0; ii < allID.size(); ii++) {
        if (allID[ii] == dxlID)
            return ii;
    }
    return -1;
}

/**
 * @brief DXSystem::motorNum2id
 * Maps motor number (in DXSystem object) to native ID (within EEPROM)
 * @param motorNo Motor number (#)
 * @return Native ID of the motor
 */
int DXSystem::motorNum2id(int motorNo)
{
    return allID[motorNo];
}

/**
 * @brief DXSystem::getModel
 * Returns the model of the motor (as in the EEPROM)
 * @param motorNo Motor number (#)
 * @return Model of the motor
 */
int DXSystem::getModel(int motorNo)
{
    return allModel[motorNo];
}

/**
 * @brief DXSystem::getNumMotors
 * Returns the number of motors initiated within the DXSystem object
 * @return Total number of motors established in the system
 * (Can be used to identify valid motor number (#) - 0 to NumMotors-1)
 */
unsigned int DXSystem::getNumMotors()
{
    return numMotors;
}


// Manual Settings//
// Does not have strict upper/lower bound checks //

/**
 * @brief DXSystem::setByte
 * Manually set a byte in EEPROM address, check status packet if applicable
 * @param motorNo Motor number (#)
 * @param addr Address on the EEPROM
 * @param value Value to be written (single byte)
 */
void DXSystem::setByte(unsigned int motorNo, int addr, int value)
{
    checkinRange(motorNo);
    int internalID = allID[motorNo];
    dclass.dxl_write_byte(internalID, addr, value);
    if (systemSRL == SRL_ALL)
    {
        checkCommError();
    }
}

/**
 * @brief DXSystem::readByte
 * Manually read a byte in EEPROM address, check status packet if applicable
 * @param motorNo Motor number (#)
 * @param addr Address on the EEPROM
 * @return Value written on the EEPROM (single byte)
 */
int DXSystem::readByte(unsigned int motorNo, int addr)
{
    checkinRange(motorNo);
    int internalID = allID[motorNo];
    int value = dclass.dxl_read_byte(internalID, addr);
    if (systemSRL != SRL_NONE && !checkCommError())
        return value;
    return -1;
}

/**
 * @brief DXSystem::setWord
 * Manually set a word (two consecutive bytes) in EEPROM address
 * @param motorNo Motor number (#)
 * @param addr Address on the EEPROM
 * @param value Value to be written (single word, i.e. two bytes)
 */
void DXSystem::setWord(unsigned int motorNo, int addr, int value)
{
    checkinRange(motorNo);
    int internalID = allID[motorNo];
    dclass.dxl_write_word(internalID, addr, value);
    if (systemSRL == SRL_ALL)
        checkCommError();
}

/**
 * @brief DXSystem::readWord
 * Manually read a word (two consecutive bytes) in EEPROM address, check status packet if applicable
 * @param motorNo Motor number (#)
 * @param addr Address on the EEPROM
 * @return Value written on the EEPROM (single word, i.e. two bytes)
 */
int DXSystem::readWord(unsigned int motorNo, int addr)
{
    checkinRange(motorNo);
    int internalID = allID[motorNo];
    int value = dclass.dxl_read_word(internalID, addr);
    if (systemSRL != SRL_NONE && !checkCommError())
        return value;
    return -1;
}

/**
 * @brief DXSystem::set2Word
 * Manually set 2 words (4 consecutive bytes) in EEPROM address --
 * Used for writing into two consecutive settings on the EEPROM, each composed of two bytes
 * (e.g. Position & Speed)
 * @param motorNo Motor number (#)
 * @param addr Address on the EEPROM
 * @param value1 First value to be written (single word, i.e. two bytes)
 * @param value2 Second value to be written (single word, i.e. two bytes)
 */
void DXSystem::set2Word(unsigned int motorNo, int addr, int value1, int value2)
{
    checkinRange(motorNo);
    int internalID = allID[motorNo];
    dclass.dxl_write_2word(internalID, addr, value1, value2);
    if (systemSRL == SRL_ALL)
        checkCommError();
}

/**
 * @brief DXSystem::queueByte
 * Manually queue a byte in EEPROM address, check status packet if applicable
 * (Requires executeQueue to execute writing)
 * @param motorNo Motor number (#)
 * @param addr Address on the EEPROM
 * @param value Value to be written (single byte)
 */
void DXSystem::queueByte(unsigned int motorNo, int addr, int value)
{
    checkinRange(motorNo);
    int internalID = allID[motorNo];
    dclass.dxl_queue_byte(internalID, addr, value);
    if (systemSRL == SRL_ALL)
        checkCommError();
}

/**
 * @brief DXSystem::queueWord
 * Manually queue a word (two consecutive bytes) in EEPROM address, check status packet if applicable
 * (Requires DXSystem::executeQueue to execute writing)
 * @param motorNo Motor number (#)
 * @param addr Address on the EEPROM
 * @param value Value to be written (single word, i.e. two bytes)
 */
void DXSystem::queueWord(unsigned int motorNo, int addr, int value)
{
    checkinRange(motorNo);
    int internalID = allID[motorNo];
    dclass.dxl_queue_word(internalID, addr, value);
    if (systemSRL == SRL_ALL)
        checkCommError();
}

/**
 * @brief DXSystem::executeQueue
 * Executes queue, as registered by DXSystem::queueByte or DXSystem::queueWord
 */
void DXSystem::executeQueue()
{
    dclass.dxl_act_queue(BROADCAST_ID);
}

/**
 * @brief DXSystem::setAllByte
 * Manually set a byte in EEPROM address of all motors
 * (Note: setAllByte sends the write instruction to all motors within a single packet)
 * @param addr Address on the EEPROM
 * @param value Values (single byte) to be written (in an int vector corresponding to motor number (#))
 */
void DXSystem::setAllByte(int addr, const vector<int>& value)
{
    if (numMotors > MAX_NUM_MOTORS)
        handleError("Too many motors", MSG_SETTING == THROW_ERROR);
    if (value.size() < numMotors)
        handleError("Input value size mismatch", MSG_SETTING == THROW_ERROR);

    dclass.dxl_sync_write_byte(addr, allID, value);
}

/**
 * @brief DXSystem::setAllWord
 * Manually set a word in EEPROM address of all motors
 * (Note: setAllWord sends the write instruction to all motors within a single packet)
 * @param addr Address on the EEPROM
 * @param value Values (single word, i.e. two bytes) to be written (in an int vector corresponding to motor number (#))
 */
void DXSystem::setAllWord(int addr, const vector<int>& value)
{
    if (numMotors > MAX_NUM_MOTORS)
        handleError("Too many motors", MSG_SETTING == THROW_ERROR);
    if (value.size() < numMotors)
        handleError("Input value size mismatch", MSG_SETTING == THROW_ERROR);

    dclass.dxl_sync_write_word(addr, allID, value);
}

/**
 * @brief DXSystem::setAll2Word
 * Manually set 2 word in EEPROM address of all motors
 * (Note: setAll2Word sends the write instruction to all motors within a single packet)
 * (Used for writing into two consecutive settings on the EEPROM, each composed of two bytes)
 * @param addr Address on the EEPROM
 * @param value1 First value to be written (single word, i.e. two bytes) (in an int vector corresponding to motor number (#))
 * @param value2 Second value to be written (single word, i.e. two bytes) (in an int vector corresponding to motor number (#))
 */
void DXSystem::setAll2Word(int addr, const vector<int>& value1, const vector<int>& value2)
{
    if (numMotors > MAX_NUM_MOTORS)
        handleError("Too many motors", MSG_SETTING == THROW_ERROR);
    if (value1.size() < numMotors || value2.size() < numMotors)
        handleError("Input value size mismatch", MSG_SETTING == THROW_ERROR);

    dclass.dxl_sync_write_2word(addr, allID, value1, value2);
}

/**
 * @brief DXSystem::readAllByte
 * Manually read a byte in EEPROM address across all motors, check status packet if applicable
 * @param addr Address on the EEPROM
 * @return Values (single bytes) read (in an int vector corresponding to motor number (#))
 */
vector<int> DXSystem::readAllByte(int addr)
{
    vector<int> data = dclass.dxl_bulk_read(addr, allID, 1, bulkreadFlag);
    checkBulkCommError();
//    vector<int> data(numMotors,-1);
//    for (unsigned int ii = 0; ii < numMotors; ii++)
//    {
//        data[ii] = readByte(ii,addr);
//    }
    return data;
}

/**
 * @brief DXSystem::readAllWord
 * Manually read a word (i.e. two bytes) in EEPROM address across all motors, check status packet if applicable
 * @param addr Address on the EEPROM
 * @return Values (single word, i.e. two bytes) read (in an int vector corresponding to motor number (#))
 */
vector<int> DXSystem::readAllWord(int addr)
{
    vector<int> data = dclass.dxl_bulk_read(addr, allID, 2, bulkreadFlag);
    checkBulkCommError();
//    vector<int> data(numMotors,-1);
//    for (unsigned int ii = 0; ii < numMotors; ii++)
//    {
//        data[ii] = readWord(ii,addr);
//    }
    return data;
}

// Checking Mechanisms//

/**
 * @brief DXSystem::checkinRange
 * Check if motor # is within set number of motors; throws exception if out of range
 * @param motorNo Motor number (#)
 * @return Boolean; true if motor # is valid, false otherwise
 */
bool DXSystem::checkinRange(unsigned int motorNo)
{
    bool inRange = (motorNo < numMotors);
    if (!inRange)
        handleError("Motor Number Out of Range", MSG_SETTING == THROW_ERROR);
    return inRange;
}

//
/**
 * @brief DXSystem::checkCommError
 * Checks Communication Status and Error. Flag set when issue detected.
 * Also throws/prints out the error or communication messages to console
 * (Behavior controlled by MSG_SETTING in setting_constants.h)
 * @return Boolean "Issue Flag"; true if error/communication status problem occurs, false otherwise
 */
bool DXSystem::checkCommError() {
    mStatus commStatus = checkCommStatus();
    if (!commStatus.flag) {
        mStatus errStatus = checkError();
        if (errStatus.flag) {
            handleError(errStatus.msg, MSG_SETTING == THROW_ERROR);
            return 1;
        }
    }
    else {
        handleError(commStatus.msg, MSG_SETTING == THROW_ERROR);
        return 1;
    }
    return 0;
}

/**
 * @brief DXSystem::checkError
 * Checks error bits in status packet
 * @return mStatus struct containing the error message, information and boolean error flag
 */
DXSystem::mStatus DXSystem::checkError()
{
    mStatus s;
    s.info = dclass.dxl_get_rxpacket_allerror();
    s.flag = 0;
    s.msg = "";

    if(dclass.dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1) {
        s.msg = "Input voltage error!";
        s.flag = 1;
        return s;
    }


    if(dclass.dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1) {
        s.msg = "Angle limit error!";
        s.flag = 1;
        return s;
    }

    if(dclass.dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1) {
        s.msg = "Overheat error!";
        s.flag = 1;
        return s;
    }

    if(dclass.dxl_get_rxpacket_error(ERRBIT_RANGE) == 1) {
        s.msg = "Out of range error!";
        s.flag = 1;
        return s;
    }

    if(dclass.dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1) {
        s.msg = "Checksum error!" ;
        s.flag = 1;
        return s;
    }

    if(dclass.dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1) {
        s.msg = "Overload error!";
        s.flag = 1;
        return s;
    }

    if(dclass.dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1) {
        s.msg = "Instruction code error";
        s.flag = 1;
        return s;
    }

    return s;
}

/**
 * @brief DXSystem::checkCommStatus
 * Checks communication status in status packet
 * @return mStatus struct containing the communication status message, information and boolean status error flag
 */
DXSystem::mStatus DXSystem::checkCommStatus()
{
    int commStatus = dclass.dxl_get_result();
    mStatus s;
    s.info = commStatus;
    s.flag = 0;
    s.msg = "";

    switch(commStatus)
    {
    case COMM_TXFAIL:
        s.msg = "COMM_TXFAIL: Failed transmit instruction packet!";
        s.flag = 1;
        return s;

    case COMM_TXERROR:
        s.msg = "COMM_TXERROR: Incorrect instruction packet!";
        s.flag = 1;
        return s;

    case COMM_RXFAIL:
        s.msg = "COMM_RXFAIL: Failed get status packet from device!";
        s.flag = 1;
        return s;

    case COMM_RXWAITING:
        s.msg = "COMM_RXWAITING: Now recieving status packet";
        s.flag = 1;
        return s;

    case COMM_RXTIMEOUT:
        s.msg = "COMM_RXTIMEOUT: There is no status packet!";
        s.flag = 1;
        return s;

    case COMM_RXCORRUPT:
        s.msg = "COMM_RXCORRUPT: Incorrect status packet!";
        s.flag = 1;
        return s;

    case COMM_RXSUCCESS:
    case COMM_TXSUCCESS:
        return s;

    default:
        s.msg = "COMM?: This is unknown error code!";
        s.flag = 1;
    }
    return s;
}

/**
 * @brief DXSystem::checkBulkCommError
 * Checks communication status and error bit of all packets upon using bulk read instructions
 * @return Boolean status error flag, returns 0 if no error was detected
 */
bool DXSystem::checkBulkCommError()
{
    vector<int> allComm = dclass.bulkCommStatus;
    vector<int> allErr = dclass.bulkError;
    int commStatus, error;
    if (allComm.size() != allErr.size())
        throw(runtime_error("Comm Status and Error information from Bulk Read is incomplete (size mismatch)"));
    for (unsigned int ii = 0; ii < allComm.size(); ii++)
    {
        bool flag = 0;
        commStatus = allComm[ii];
        error = allErr[ii];
        stringstream ss;
        ss << "Serial Port" << devPort << ", Motor #" << ii << ": ";
        string commMsg = ss.str();
        string errMsg = ss.str();

        switch(commStatus)
        {
        case COMM_RXSUCCESS:
        case COMM_TXSUCCESS:
            break;

        case COMM_TXFAIL:
            commMsg += "COMM_TXFAIL: Failed transmit instruction packet!";
            flag = 1;
            break;

        case COMM_TXERROR:
            commMsg += "COMM_TXERROR: Incorrect instruction packet!";
            flag = 1;
            break;

        case COMM_RXFAIL:
            commMsg += "COMM_RXFAIL: Failed get status packet from device!";
            flag = 1;
            break;

        case COMM_RXWAITING:
            commMsg += "COMM_RXWAITING: Now recieving status packet";
            flag = 1;
            break;

        case COMM_RXTIMEOUT:
            commMsg += "COMM_RXTIMEOUT: There is no status packet!";
            flag = 1;
            break;

        case COMM_RXCORRUPT:
            commMsg += "COMM_RXCORRUPT: Incorrect status packet!";
            flag = 1;
            break;

        default:
            commMsg += "COMM?: This is unknown error code!";
            flag = 1;
            break;
        }

        if (flag)
        {
            handleError(commMsg, MSG_SETTING == THROW_ERROR);
            return 1;
        }
    
        
        if (error & ERRBIT_VOLTAGE)
        {
            errMsg += "Input voltage error!";
            flag = 1;
        }
        if (error & ERRBIT_ANGLE)
        {
            errMsg += "Angle limit error!";
            flag = 1;
        }
        if (error & ERRBIT_OVERHEAT)
        {
            errMsg += "Overheat error!";
            flag = 1;
        }
        if (error & ERRBIT_RANGE)
        {
            errMsg += "Out of range error!";
            flag = 1;
        }
        if (error & ERRBIT_CHECKSUM)
        {
            errMsg += "Checksum error!";
            flag = 1;
        }
        if (error & ERRBIT_OVERLOAD)
        {
            errMsg += "Overload error!";
            flag = 1;
        }
        if (error & ERRBIT_INSTRUCTION)
        {
            errMsg += "Instruction code error!";
            flag = 1;
        }
        
        if (flag)
        {
            handleError(errMsg, MSG_SETTING == THROW_ERROR);
            return 1;
        }
    }
    return 0;
}
