#include <stdexcept>

#include "control_table_constants.h"
#include "dconstants.h"
#include "dmodeldetails.h"
#include "additionaltools.h"
#include "user_settings.h"

#include "dxsetupmotor.h"

#define MOTOR_NUMBER 0

/**
 * @brief DXSetupMotor::DXSetupMotor
 * Constructor for Motor Calibration Instance; Only 1 motor should be connected at a time;
 * Resets the motor to factory setting + specifications indicated on setting_constants.h (ID, baud rate, SRL and delay time)
 * @param newID Desired new ID (unique) for the motor
 * @param devicePort Designated port for interface (dev/ttyUSB*, where * is the port number)
 * @param dxlBaudRate Baud Rate to be set, according to Dynamixel's table for baud rate data
 */
DXSetupMotor::DXSetupMotor(int newID, int devicePort, int dxlBaudRate):DXSystem(devicePort, dxlBaudRate)
{
    if (isInitialized())
    {
        if (numMotors > 1)
            handleError("Please connect only 1 motor for setup", true);

    }
    if (numMotors == 1)
        hardReset(newID);
}

/**
 * @brief DXSetupMotor::~DXSetupMotor
 * Destructor
 */
DXSetupMotor::~DXSetupMotor()
{

}

/**
 * @brief DXSetupMotor::hardReset
 * Factory Reset motor and assigns new ID (and sets SRL, baudrate and return delay time to default in setting_constants.h)
 * @param newID Desired new ID (unique) for the motor
 */
void DXSetupMotor::hardReset(int newID)
{    
    int model = allModel[MOTOR_NUMBER];
    systemSRL = SRL_ALL;

    // Reset motor
    dclass.dxl_set_txpacket_id(allID[MOTOR_NUMBER]);
    dclass.dxl_set_txpacket_instruction(INST_RESET);
    dclass.dxl_set_txpacket_length(2);

    dclass.dxl_txrx_packet();

    // Reopen at default reset baudrate
    int resetBaudRate = modelDefaultBaudRate(model);
    reopenPort(resetBaudRate);

    // Reset delay time 10ms before sending more packets
    delay(10);
    dclass.dxl_write_byte(DEFAULT_RESET_ID, P_RETURN_DELAY_TIME, DEFAULT_RETURN_DELAY_TIME);
    dclass.dxl_write_byte(DEFAULT_RESET_ID, P_STATUS_RETURN_LEVEL, DEFAULT_SRL);
    systemSRL = DEFAULT_SRL;
    resetNewID(newID);

    // Reset and reopen at default baudrate (if necessary)
    delay(10);
    if (resetBaudRate != DEFAULT_BAUDRATE)
        setAllBaudRate(DEFAULT_BAUDRATE);

    // Delay after reopening port
    delay(10);

}

/**
 * @brief DXSetupMotor::resetNewID
 * Writes new ID to the EEPROM
 * @param newID Desired new ID
 */
void DXSetupMotor::resetNewID(int newID)
{
    dclass.dxl_write_byte(DEFAULT_RESET_ID, P_DXL_ID, newID);
    allID[MOTOR_NUMBER] = newID;
}
