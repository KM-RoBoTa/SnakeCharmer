// Constants associated with dynamixel motors

// SRL Modes
#define SRL_NONE 0
#define SRL_READ_ONLY 1
#define SRL_ALL 2

// Joint or Wheel mode identification
#define JOINT_MODE 1023
#define WHEEL_MODE 2047
#define MULTI_TURN_MODE 4095

// Port number (default: tty/devUSB0)
#define DEFAULT_PORT_NUM 0

// Baud Rate (default: 1 = 1Mbps)
#define DEFAULT_BAUDRATE 1
#define ALT_BAUDRATE 34

// ID is 1 upon reset
#define DEFAULT_RESET_ID 1

// Max Temperature limit (default: 80oC - should not be changed)
#define DEFAULT_TEMP_LIMIT 80

// Speed values, For joint mode
#define MAX_SPD 1023
#define MIN_SPD 0

// Speed Reference, For wheel mode
#define SPD_CW 0        // For CW rotation
#define SPD_CCW 1024    // For CCW rotation (10th bit high)

// Print or throw communication/error messages?
#define THROW_ERROR 1
#define PRINT_ERROR 0

// Torque enable settings
#define TORQUE_ENABLE 1
#define TORQUE_DISABLE 0

// Max number of motors
#define MAX_NUM_MOTORS 27
