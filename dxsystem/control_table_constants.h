// Control table address
// Encodes EEPROM and RAM control table for dynamixel motors

// Read
// EEPROM
#define P_MODEL_NO_L                0
#define P_MODEL_NO_H                1
#define P_FIRMWARE_VER              2
// RAM
#define P_PRESENT_POSITION_L        36
#define P_PRESENT_POSITION_H        37
#define P_PRESENT_SPEED_L           38
#define P_PRESENT_SPEED_H           39
#define P_PRESENT_LOAD_L            40
#define P_PRESENT_LOAD_H            41
#define P_PRESENT_VOLTAGE           42
#define P_PRESENT_TEMPERATURE       43
#define P_REGISTERED				44
#define P_MOVING					46

// Read Write
// EEPROM
#define P_DXL_ID					3
#define P_BAUD_RATE                 4
#define P_RETURN_DELAY_TIME         5
#define P_CW_ANGLE_LIMIT_L          6
#define P_CW_ANGLE_LIMIT_H          7
#define P_CCW_ANGLE_LIMIT_L         8
#define P_CCW_ANGLE_LIMIT_H         9
#define P_TEMP_HIGH_LIMIT           11
#define P_VOLT_LOW_LIMIT			12
#define P_VOLT_HIGH_LIMIT			13
#define P_MAX_TORQUE_L              14
#define P_MAX_TORQUE_H              15
#define P_STATUS_RETURN_LEVEL       16
#define P_ALARM_LED                 17
#define P_ALARM_SHUTDOWN            18
// RAM
#define P_TORQUE_ENABLE             24
#define P_LED						25
#define P_GOAL_POSITION_L           30
#define P_GOAL_POSITION_H           31
#define P_MOVING_SPEED_L            32
#define P_MOVING_SPEED_H            33
#define P_TORQUE_LIMIT_L			34
#define P_TORQUE_LIMIT_H			35
#define P_LOCK						47
#define P_PUNCH_L					48
#define P_PUNCH_H					49


#define P_CW_COMPLIANCE_MARGIN      26 // Only for AX/RX
#define P_CCW_COMPLIANCE_MARGIN     27 // Only for AX/RX
#define P_CW_COMPLIANCE_SLOPE       28 // Only for AX/RX
#define P_CCW_COMPLIANCE_SLOPE      29 // Only for AX/RX


#define P_MULTI_TURN_OFFSET_L       20 // Only for MX
#define P_MULTI_TURN_OFFSET_H       21 // Only for MX
#define P_RESOLUTION_DIVIDER        22 // Only for MX
#define P_D_GAIN					26 // Only for MX
#define P_I_GAIN					27 // Only for MX
#define P_P_GAIN					28 // Only for MX
#define P_GOAL_ACCELERATION         73 // Only for MX


#define P_DRIVE_MODE                10 // Only for MX106
#define P_CURRENT_L                 68 // Only for MX106 and MX64
#define P_CURRENT_H                 69 // Only for MX106 and MX64
#define P_TORQUE_CONTROL_MODE       70 // Only for MX106 and MX64
#define P_GOAL_TORQUE_L             71 // Only for MX106 and MX64
#define P_GOAL_TORQUE_H             72 // Only for MX106 and MX64
