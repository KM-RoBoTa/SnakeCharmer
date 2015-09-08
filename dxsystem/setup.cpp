#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "additionaltools.h"
#include "dconstants.h"
#include "dmodeldetails.h"
#include "user_settings.h"
#include "userinputfn.h"

#include "setup.h"

using namespace std;

DXSetupMotor* setupMotor(const int newID);
DXSetupMotor* searchAllBaudRate(const int newID);

/**
 * @brief dxlUserDefaultConfig
 * Sets all motors to user default configuration (baudrate, SRL and delay time)
 * @param d Reference to the motor system
 */
void dxsystemUserDefaultConfig(DXMotorSystem& d)
{
    d.setAllStatusReturnLevel(DEFAULT_SRL);
    d.setAllDelay(DEFAULT_RETURN_DELAY_TIME);
}

/**
 * @brief dxlSetup
 * Helps in setting up and resetting motors (one at a time) - used to restore user default settings on the motor and avoid conflicts in motor IDs
 * @return Status flag - 0 upon exit without issues
 */
int dxlSetupSequence() {
    cout << "Please ensure that the port in use is dev/ttyUSB0." << endl;
    cout << "How many motors are you setting up?: ";
    int numMotors = getInt();
    if (numMotors > 252)
        handleError("Too many motors, insufficient unique IDs", MSG_SETTING == THROW_ERROR);
    vector<int> models;
    cout << "Please connect ONE motor to the port at a time, and enter 'Y' to continue." << endl;
    for (int ii = 0; ii < numMotors; ii++){
        cout << "Connect a new motor ('Y' to continue):";
        while(true)
        {
            DSETUP:
            if(!getYorN())
            {
                cout << "Setup Sequence terminated." << endl;
                return 1;
            }
            try{
                DXSetupMotor* p_dc = setupMotor(ii);
                if (p_dc != NULL)
                {
                    cout << "ID#" << p_dc->motorNum2id(0) << endl;
                    cout << "Baud Rate: " << p_dc->getBaudRate(0) << endl;
                    cout << "Model: " << p_dc->getModel(0) << endl;
                    cout << "SRL:" << p_dc->getStatusReturnLevel(0) << endl;
                    cout << "Delay:" << p_dc->getDelay(0) << endl;
                    models.push_back(p_dc->getModel(0));
                    delete(p_dc);
                    break;
                }
            }catch(runtime_error e){
                cout << e.what() << endl;
                cout << "Try again?:";
                goto DSETUP;
            }
            cout << "Check the connection and try again?:";
        }
    }
    cout << "End of Setup Sequence." << endl;
    return 0;
}

// Internal helper to dxlSetup
DXSetupMotor* setupMotor(const int newID)
{
    DXSetupMotor* p_dc = new DXSetupMotor(newID, DEFAULT_PORT_NUM, DEFAULT_BAUDRATE);
    cout << "Detected " << p_dc->getNumMotors() << " motors" << endl;
    if (!p_dc->getNumMotors())
    {
        delete(p_dc);
        delay(500);
        p_dc = new DXSetupMotor(newID, DEFAULT_PORT_NUM, ALT_BAUDRATE);

        cout << "Detected " << p_dc->getNumMotors() << " motors on baud rate 57.6kbps" << endl;
        if (!p_dc->getNumMotors())
        {
            cout << "No motors on 1Mbps and 57.6 kbps -- Please ensure motors are connected" << endl;
            cout << "Would you like to check all baudrate values?:";
            if(getYorN())
            {
                delete(p_dc);
                delay(500);
                p_dc = searchAllBaudRate(newID);
            } else {
                return NULL;
            }
        }
    }
    return p_dc;
}

// Iterates across all baud rate values to find a motor
DXSetupMotor* searchAllBaudRate(const int newID)
{
    DXSetupMotor* p_dc;
    for (int ii = 0; ii < 256; ii++)
    {
        p_dc = new DXSetupMotor(newID, DEFAULT_PORT_NUM, ii);
        if (p_dc->getNumMotors() > 0)
        {
            return p_dc;
        }
        delete(p_dc);
        delay(500);
    }
    p_dc = new DXSetupMotor(newID, DEFAULT_PORT_NUM, DEFAULT_BAUDRATE);
    return NULL;
}




//----------//

/**
 * @brief dxsystemSetup
 * Sets up a motor system and locks EEPROM of motors
 * @param deviceIndex Port number of the USB/serial device
 * @return Pointer to the DXMotorSystem
 */
DXMotorSystem* dxsystemSetup(const int deviceIndex)
{
    DXMotorSystem* d1 = new DXMotorSystem(deviceIndex,BAUDRATE);
    stringstream sstr;
    sstr << "Port " << deviceIndex << ": ";
    // Open USB2Dynamixel //
    if (!d1->isInitialized())
    {
        sstr << "Failure to open USB2Dynamixel";
        throw runtime_error(sstr.str());
    }
    sstr <<  "Succeed to open USB2Dynamixel!";
    cout << sstr.str() << endl;
    if (d1->getNumMotors() < 1)
    {
        throw runtime_error("There are no motors connected.");
    }

    // Set hard limits on motors (EEPROM) before moving
    dxsystemUserDefaultConfig(*d1);
    dxlSetupEEPROM(*d1);
    d1->lockEEPROM();
    return d1;
}

/**
 * @brief multidxsystemSetup
 * Sets up multiple motor systems and locks EEPROM of motors
 * @param numPorts Number of DXMotorSystem to set up (device ports 0 to numPorts-1 will be used)
 * @return Vector of pointers to DXMotorSystem objects
 */
vector<DXMotorSystem*> multidxsystemSetup(const int numPorts)
{
    vector<DXMotorSystem*> allSys;
    for (int ii = 0; ii < numPorts; ii++)
    {
        DXMotorSystem *dptr = dxsystemSetup(ii);
        allSys.push_back(dptr);
    }
    return allSys;
}

vector<DXMotorSystem*> multidxsystemSetup(vector<int> portNum)
{
    vector<DXMotorSystem*> allSys;
    for (unsigned int ii = 0; ii < portNum.size(); ii++)
    {
        DXMotorSystem *dptr = dxsystemSetup(portNum[ii]);
        allSys.push_back(dptr);
    }
    return allSys;
}
