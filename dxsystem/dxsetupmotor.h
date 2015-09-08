#ifndef DSETUPMOTOR_H
#define DSETUPMOTOR_H

#include "dxsystem.h"

class DXSetupMotor : public DXSystem
{
    /** @defgroup dsetupsystem DXSetupMotor
     *  This class extends DXSystem and aids in motor reset.
     *  This class can only handle one motor at a time to prevent conflict in motor IDs.
     *  @{
     */
public:
    DXSetupMotor(int newID, int devicePort, int dxlBaudRate);
    ~DXSetupMotor();

private:

    void hardReset(int newID);
    void resetNewID(int newID);
    /** @} */
};

#endif // DSETUPMOTOR_H
