#include "dconstants.h"


// Return Delay Time (default: 250 = 0.5 ms)
#define DEFAULT_RETURN_DELAY_TIME 0 // 2 us/data value

#define BAUDRATE 1 // 1Mbps

// Status Return Level (default: SRL_ALL (2) - Return All)
#define DEFAULT_SRL SRL_READ_ONLY

// Print or throw communication/error messages?
#define MSG_SETTING PRINT_ERROR //PRINT_ERROR/THROW_ERROR

// Torque enable settings
#define DEFAULT_TORQUE_EN TORQUE_DISABLE
