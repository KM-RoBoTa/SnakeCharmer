#ifndef DYNAMIXELCLASS_H
#define DYNAMIXELCLASS_H

#define MAXNUM_TXPARAM		(150)
#define MAXNUM_RXPARAM		(60)

#define BROADCAST_ID		(254)

#define INST_PING			(1)
#define INST_READ			(2)
#define INST_WRITE			(3)
#define INST_REG_WRITE		(4)
#define INST_ACTION			(5)
#define INST_RESET			(6)
#define INST_SYNC_WRITE		(131)

#define INST_BULK_READ      (146) // for MX series

#define ERRBIT_VOLTAGE		(1)
#define ERRBIT_ANGLE		(2)
#define ERRBIT_OVERHEAT		(4)
#define ERRBIT_RANGE		(8)
#define ERRBIT_CHECKSUM		(16)
#define ERRBIT_OVERLOAD		(32)
#define ERRBIT_INSTRUCTION	(64)

#define	COMM_TXSUCCESS		(0)
#define COMM_RXSUCCESS		(1)
#define COMM_TXFAIL         (2)
#define COMM_RXFAIL         (3)
#define COMM_TXERROR		(4)
#define COMM_RXWAITING		(5)
#define COMM_RXTIMEOUT		(6)
#define COMM_RXCORRUPT		(7)

#include <vector>

#include "dxlhal.h"

using namespace std;

class dynamixelClass
{
public:
    dynamixelClass();
    ~dynamixelClass();

    DXLHAL dxlhal;

    unsigned char gbInstructionPacket[MAXNUM_TXPARAM+10];
    unsigned char gbStatusPacket[MAXNUM_RXPARAM+10];
    unsigned char gbRxPacketLength;
    unsigned char gbRxGetLength ;
    int gbCommStatus;
    int giBusUsing;

    ///////////// device control methods ////////////////////////
    int dxl_initialize(int deviceIndex, int baudnum );
    void dxl_terminate();


    ///////////// set/get packet methods //////////////////////////
    void dxl_set_txpacket_id(int id);


    void dxl_set_txpacket_instruction(int instruction);


    void dxl_set_txpacket_parameter(int index, int value);
    void dxl_set_txpacket_length(int length);

    int dxl_get_rxpacket_allerror();
    int dxl_get_rxpacket_error(int errbit);


    int dxl_get_rxpacket_length(void);
    int dxl_get_rxpacket_parameter(int index);


    // utility for value
    int dxl_makeword(int lowbyte, int highbyte);
    int dxl_get_lowbyte(int word);
    int dxl_get_highbyte(int word);


    ////////// packet communication methods ///////////////////////
    void dxl_tx_packet(void);
    void dxl_rx_packet(void);
    void dxl_txrx_packet(void);

    int dxl_get_result(void);


    //////////// high communication methods ///////////////////////
    void dxl_ping(int id);
    int dxl_read_byte(int id, int address);
    void dxl_write_byte(int id, int address, int value);
    int dxl_read_word(int id, int address);
    void dxl_write_word(int id, int address, int value);
    void dxl_write_2word(int id, int address, int value1, int value2);

    //////////// custom high communication methods ///////////////////////
    void dxl_queue_byte( int id, int address, int value );
    void dxl_queue_word( int id, int address, int value );
    void dxl_act_queue( int id );

    void dxl_sync_write_byte(int address, const vector<int>& allIDs, const vector<int>& values);
    void dxl_sync_write_word(int address, const vector<int>& allIDs, const vector<int>& values);
    void dxl_sync_write_2word(int address, const vector<int>& allIDs, const vector<int>& values1, const vector<int>& values2);
    vector<int> dxl_bulk_read(int address, const vector<int>& allIDs, int readLength, const vector<bool>& bulkreadFlag);
    void dxl_bulk_rx_packet(int id);

    vector<int> bulkCommStatus;
    vector<int> bulkError;
};

#endif // DYNAMIXELCLASS_H
