#include "dynamixelclass.h"
#include <queue>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#define ID					(2)
#define LENGTH				(3)
#define INSTRUCTION			(4)
#define ERRBIT				(4)
#define PARAMETER			(5)
#define DEFAULT_BAUDNUMBER	(1)

dynamixelClass::dynamixelClass()
{
    memset(&gbInstructionPacket[0], 0, sizeof(gbInstructionPacket));
    memset(&gbStatusPacket[0], 0, sizeof(gbStatusPacket));
    gbRxPacketLength = 0;
    gbRxGetLength = 0;
    gbCommStatus = COMM_RXSUCCESS;
    giBusUsing = 0;
}

dynamixelClass::~dynamixelClass()
{

}

int dynamixelClass::dxl_initialize(int deviceIndex, int baudnum )
{
    float baudrate;
    baudrate = 2000000.0f / (float)(baudnum + 1);
    if (baudnum == 250)
        baudrate = 2250000;
    else if (baudnum == 251)
        baudrate = 2500000;
    else if (baudnum == 252)
        baudrate = 3000000;

    if( dxlhal.dxl_hal_open(deviceIndex, baudrate) == 0 )
        return 0;

    gbCommStatus = COMM_RXSUCCESS;
    giBusUsing = 0;
    return 1;
}

void dynamixelClass::dxl_terminate(void)
{
    dxlhal.dxl_hal_close();
}

void dynamixelClass::dxl_tx_packet(void)
{
    unsigned char i;
    unsigned char TxNumByte, RealTxNumByte;
    unsigned char checksum = 0;

    if( giBusUsing == 1 )
        return;

    giBusUsing = 1;

    if( gbInstructionPacket[LENGTH] > (MAXNUM_TXPARAM+2) )
    {
        gbCommStatus = COMM_TXERROR;
        giBusUsing = 0;
        return;
    }

    if( gbInstructionPacket[INSTRUCTION] != INST_PING
        && gbInstructionPacket[INSTRUCTION] != INST_READ
        && gbInstructionPacket[INSTRUCTION] != INST_WRITE
        && gbInstructionPacket[INSTRUCTION] != INST_REG_WRITE
        && gbInstructionPacket[INSTRUCTION] != INST_ACTION
        && gbInstructionPacket[INSTRUCTION] != INST_RESET
        && gbInstructionPacket[INSTRUCTION] != INST_SYNC_WRITE
        && gbInstructionPacket[INSTRUCTION] != INST_BULK_READ)

    {
        gbCommStatus = COMM_TXERROR;
        giBusUsing = 0;
        return;
    }

    gbInstructionPacket[0] = 0xff;
    gbInstructionPacket[1] = 0xff;
    for( i=0; i<(gbInstructionPacket[LENGTH]+1); i++ )
        checksum += gbInstructionPacket[i+2];
    gbInstructionPacket[gbInstructionPacket[LENGTH]+3] = ~checksum;

    if( gbCommStatus == COMM_RXTIMEOUT || gbCommStatus == COMM_RXCORRUPT )
        dxlhal.dxl_hal_clear();

    TxNumByte = gbInstructionPacket[LENGTH] + 4;
    RealTxNumByte = dxlhal.dxl_hal_tx( (unsigned char*)gbInstructionPacket, TxNumByte );

    if( TxNumByte != RealTxNumByte )
    {
        gbCommStatus = COMM_TXFAIL;
        giBusUsing = 0;
        return;
    }

    if( gbInstructionPacket[INSTRUCTION] == INST_READ)
        dxlhal.dxl_hal_set_timeout( gbInstructionPacket[PARAMETER+1] + 6 );
    else
        dxlhal.dxl_hal_set_timeout( 6 );

    gbCommStatus = COMM_TXSUCCESS;
}

void dynamixelClass::dxl_rx_packet(void)
{
    unsigned char i, j, nRead;
    unsigned char checksum = 0;

    if( giBusUsing == 0 )
        return;

    if( gbInstructionPacket[ID] == BROADCAST_ID )
    {
        gbCommStatus = COMM_RXSUCCESS;
        giBusUsing = 0;
        return;
    }

    if( gbCommStatus == COMM_TXSUCCESS )
    {
        gbRxGetLength = 0;
        gbRxPacketLength = 6;
    }

    nRead = dxlhal.dxl_hal_rx( (unsigned char*)&gbStatusPacket[gbRxGetLength], gbRxPacketLength - gbRxGetLength );
    gbRxGetLength += nRead;
    if( gbRxGetLength < gbRxPacketLength )
    {
        if( dxlhal.dxl_hal_timeout() == 1 )
        {
            if(gbRxGetLength == 0)
                gbCommStatus = COMM_RXTIMEOUT;
            else
                gbCommStatus = COMM_RXCORRUPT;
            giBusUsing = 0;
            return;
        }
    }

    // Find packet header
    for( i=0; i<(gbRxGetLength-1); i++ )
    {
        if( gbStatusPacket[i] == 0xff && gbStatusPacket[i+1] == 0xff )
        {
            break;
        }
        else if( i == gbRxGetLength-2 && gbStatusPacket[gbRxGetLength-1] == 0xff )
        {
            break;
        }
    }
    if( i > 0 )
    {
        for( j=0; j<(gbRxGetLength-i); j++ )
            gbStatusPacket[j] = gbStatusPacket[j + i];

        gbRxGetLength -= i;
    }

    if( gbRxGetLength < gbRxPacketLength )
    {
        gbCommStatus = COMM_RXWAITING;
        return;
    }

    // Check id pairing
    if( gbInstructionPacket[ID] != gbStatusPacket[ID])
    {
        gbCommStatus = COMM_RXCORRUPT;
        giBusUsing = 0;
        return;
    }

    gbRxPacketLength = gbStatusPacket[LENGTH] + 4;
    if( gbRxGetLength < gbRxPacketLength )
    {
        nRead = dxlhal.dxl_hal_rx( (unsigned char*)&gbStatusPacket[gbRxGetLength], gbRxPacketLength - gbRxGetLength );
        gbRxGetLength += nRead;
        if( gbRxGetLength < gbRxPacketLength )
        {
            gbCommStatus = COMM_RXWAITING;
            return;
        }
    }

    // Check checksum
    for( i=0; i<(gbStatusPacket[LENGTH]+1); i++ )
        checksum += gbStatusPacket[i+2];
    checksum = ~checksum;

    if( gbStatusPacket[gbStatusPacket[LENGTH]+3] != checksum )
    {
        gbCommStatus = COMM_RXCORRUPT;
        giBusUsing = 0;
        return;
    }

    gbCommStatus = COMM_RXSUCCESS;
    giBusUsing = 0;
}

void dynamixelClass::dxl_txrx_packet(void)
{
    dxl_tx_packet();

    if( gbCommStatus != COMM_TXSUCCESS )
        return;

    do{
        dxl_rx_packet();
    }while( gbCommStatus == COMM_RXWAITING );
}

int dynamixelClass::dxl_get_result(void)
{
    return gbCommStatus;
}

void dynamixelClass::dxl_set_txpacket_id( int id )
{
    gbInstructionPacket[ID] = (unsigned char)id;
}

void dynamixelClass::dxl_set_txpacket_instruction( int instruction )
{
    gbInstructionPacket[INSTRUCTION] = (unsigned char)instruction;
}

void dynamixelClass::dxl_set_txpacket_parameter( int index, int value )
{
    gbInstructionPacket[PARAMETER+index] = (unsigned char)value;
}

void dynamixelClass::dxl_set_txpacket_length( int length )
{
    gbInstructionPacket[LENGTH] = (unsigned char)length;
}

int dynamixelClass::dxl_get_rxpacket_error( int errbit )
{
    if( gbStatusPacket[ERRBIT] & (unsigned char)errbit )
        return 1;

    return 0;
}

int dynamixelClass::dxl_get_rxpacket_allerror(void)
{
    return gbStatusPacket[ERRBIT];
}

int dynamixelClass::dxl_get_rxpacket_length(void)
{
    return (int)gbStatusPacket[LENGTH];
}

int dynamixelClass::dxl_get_rxpacket_parameter( int index )
{
    return (int)gbStatusPacket[PARAMETER+index];
}

int dynamixelClass::dxl_makeword( int lowbyte, int highbyte )
{
    unsigned short word;

    word = highbyte;
    word = word << 8;
    word = word + lowbyte;
    return (int)word;
}

int dynamixelClass::dxl_get_lowbyte( int word )
{
    unsigned short temp;

    temp = word & 0xff;
    return (int)temp;
}

int dynamixelClass::dxl_get_highbyte( int word )
{
    unsigned short temp;

    temp = word & 0xff00;
    temp = temp >> 8;
    return (int)temp;
}

void dynamixelClass::dxl_ping( int id )
{
    while(giBusUsing);

    gbInstructionPacket[ID] = (unsigned char)id;
    gbInstructionPacket[INSTRUCTION] = INST_PING;
    gbInstructionPacket[LENGTH] = 2;

    dxl_txrx_packet();
}

int dynamixelClass::dxl_read_byte( int id, int address )
{
    while(giBusUsing);

    gbInstructionPacket[ID] = (unsigned char)id;
    gbInstructionPacket[INSTRUCTION] = INST_READ;
    gbInstructionPacket[PARAMETER] = (unsigned char)address;
    gbInstructionPacket[PARAMETER+1] = 1;
    gbInstructionPacket[LENGTH] = 4;

    dxl_txrx_packet();

    return (int)gbStatusPacket[PARAMETER];
}

void dynamixelClass::dxl_write_byte( int id, int address, int value )
{
    while(giBusUsing);

//    gbInstructionPacket[ID] = (unsigned char)id;
//    gbInstructionPacket[INSTRUCTION] = INST_WRITE;
//    gbInstructionPacket[PARAMETER] = (unsigned char)address;
//    gbInstructionPacket[PARAMETER+1] = (unsigned char)value;
//    gbInstructionPacket[LENGTH] = 4;

// Modified write byte to use SYNC_WRITE to optimize execution time
    gbInstructionPacket[ID] = BROADCAST_ID;
    gbInstructionPacket[INSTRUCTION] = INST_SYNC_WRITE;
    gbInstructionPacket[PARAMETER] = (unsigned char)address;
    gbInstructionPacket[PARAMETER+1] = 1;
    gbInstructionPacket[PARAMETER+2] = id;
    gbInstructionPacket[PARAMETER+3] = value;
    gbInstructionPacket[LENGTH] = 6;


    dxl_txrx_packet();
}

int dynamixelClass::dxl_read_word( int id, int address )
{
    while(giBusUsing);

    gbInstructionPacket[ID] = (unsigned char)id;
    gbInstructionPacket[INSTRUCTION] = INST_READ;
    gbInstructionPacket[PARAMETER] = (unsigned char)address;
    gbInstructionPacket[PARAMETER+1] = 2;
    gbInstructionPacket[LENGTH] = 4;

    dxl_txrx_packet();

    return dxl_makeword((int)gbStatusPacket[PARAMETER], (int)gbStatusPacket[PARAMETER+1]);
}

void dynamixelClass::dxl_write_word( int id, int address, int value )
{
    while(giBusUsing);

//    gbInstructionPacket[ID] = (unsigned char)id;
//    gbInstructionPacket[INSTRUCTION] = INST_WRITE;
//    gbInstructionPacket[PARAMETER] = (unsigned char)address;
//    gbInstructionPacket[PARAMETER+1] = (unsigned char)dxl_get_lowbyte(value);
//    gbInstructionPacket[PARAMETER+2] = (unsigned char)dxl_get_highbyte(value);
//    gbInstructionPacket[LENGTH] = 5;

// Modified write word to use SYNC_WRITE to optimize execution time
    gbInstructionPacket[ID] = BROADCAST_ID;
    gbInstructionPacket[INSTRUCTION] = INST_SYNC_WRITE;
    gbInstructionPacket[PARAMETER] = (unsigned char)address;
    gbInstructionPacket[PARAMETER+1] = 2;
    gbInstructionPacket[PARAMETER+2] = id;
    gbInstructionPacket[PARAMETER+3] = (unsigned char)dxl_get_lowbyte(value);
    gbInstructionPacket[PARAMETER+4] = (unsigned char)dxl_get_highbyte(value);
    gbInstructionPacket[LENGTH] = 7;

    dxl_txrx_packet();
}


void dynamixelClass::dxl_write_2word( int id, int address, int value1, int value2 )
{
    while(giBusUsing);

//    gbInstructionPacket[ID] = (unsigned char)id;
//    gbInstructionPacket[INSTRUCTION] = INST_WRITE;
//    gbInstructionPacket[PARAMETER] = (unsigned char)address;
//    gbInstructionPacket[PARAMETER+1] = (unsigned char)dxl_get_lowbyte(value1);
//    gbInstructionPacket[PARAMETER+2] = (unsigned char)dxl_get_highbyte(value1);
//    gbInstructionPacket[PARAMETER+3] = (unsigned char)dxl_get_lowbyte(value2);
//    gbInstructionPacket[PARAMETER+4] = (unsigned char)dxl_get_highbyte(value2);
//    gbInstructionPacket[LENGTH] = 7;

// Modified write 2word to use SYNC_WRITE to optimize execution time
    gbInstructionPacket[ID] = BROADCAST_ID;
    gbInstructionPacket[INSTRUCTION] = INST_SYNC_WRITE;
    gbInstructionPacket[PARAMETER] = (unsigned char)address;
    gbInstructionPacket[PARAMETER+1] = 4;
    gbInstructionPacket[PARAMETER+2] = id;
    gbInstructionPacket[PARAMETER+3] = (unsigned char)dxl_get_lowbyte(value1);
    gbInstructionPacket[PARAMETER+4] = (unsigned char)dxl_get_highbyte(value1);
    gbInstructionPacket[PARAMETER+5] = (unsigned char)dxl_get_lowbyte(value2);
    gbInstructionPacket[PARAMETER+6] = (unsigned char)dxl_get_highbyte(value2);
    gbInstructionPacket[LENGTH] = 9;

    dxl_txrx_packet();
}

void dynamixelClass::dxl_queue_byte( int id, int address, int value )
{
    while(giBusUsing);

    gbInstructionPacket[ID] = (unsigned char)id;
    gbInstructionPacket[INSTRUCTION] = INST_REG_WRITE;
    gbInstructionPacket[PARAMETER] = (unsigned char)address;
    gbInstructionPacket[PARAMETER+1] = (unsigned char)value;
    gbInstructionPacket[LENGTH] = 4;

    dxl_txrx_packet();
}

void dynamixelClass::dxl_queue_word( int id, int address, int value )
{
    while(giBusUsing);

    gbInstructionPacket[ID] = (unsigned char)id;
    gbInstructionPacket[INSTRUCTION] = INST_REG_WRITE;
    gbInstructionPacket[PARAMETER] = (unsigned char)address;
    gbInstructionPacket[PARAMETER+1] = (unsigned char)dxl_get_lowbyte(value);
    gbInstructionPacket[PARAMETER+2] = (unsigned char)dxl_get_highbyte(value);
    gbInstructionPacket[LENGTH] = 5;

    dxl_txrx_packet();
}

void dynamixelClass::dxl_act_queue( int id )
{
    while(giBusUsing);

    gbInstructionPacket[ID] = (unsigned char)id;
    gbInstructionPacket[INSTRUCTION] = INST_ACTION;
    gbInstructionPacket[LENGTH] = 2;

    dxl_txrx_packet();
}

void dynamixelClass::dxl_sync_write_byte(int address, const vector<int>& allIDs, const vector<int>& values)
{
    while(giBusUsing);

    gbInstructionPacket[ID] = BROADCAST_ID;
    gbInstructionPacket[INSTRUCTION] = INST_SYNC_WRITE;
    gbInstructionPacket[PARAMETER] = (unsigned char)address;
    gbInstructionPacket[PARAMETER+1] = 1;
    int size = allIDs.size();
    for (int ii = 0; ii < size; ii++)
    {
        gbInstructionPacket[PARAMETER+2+2*ii] = allIDs[ii];
        gbInstructionPacket[PARAMETER+2+2*ii+1] = values[ii];
    }
    gbInstructionPacket[LENGTH] = 2*size+4;

    dxl_txrx_packet();
}

void dynamixelClass::dxl_sync_write_word(int address, const vector<int>& allIDs, const vector<int>& values)
{
    while(giBusUsing);

    gbInstructionPacket[ID] = BROADCAST_ID;
    gbInstructionPacket[INSTRUCTION] = INST_SYNC_WRITE;
    gbInstructionPacket[PARAMETER] = (unsigned char)address;
    gbInstructionPacket[PARAMETER+1] = 2;
    int size = allIDs.size();
    for (int ii = 0; ii < size; ii++)
    {
        gbInstructionPacket[PARAMETER+2+3*ii] = allIDs[ii];
        gbInstructionPacket[PARAMETER+2+3*ii+1] = dxl_get_lowbyte(values[ii]);
        gbInstructionPacket[PARAMETER+2+3*ii+2] = dxl_get_highbyte(values[ii]);
    }
    gbInstructionPacket[LENGTH] = 3*size+4;

    dxl_txrx_packet();
}

void dynamixelClass::dxl_sync_write_2word(int address, const vector<int>& allIDs, const vector<int>& values1, const vector<int>& values2)
{
    while(giBusUsing);

    gbInstructionPacket[ID] = BROADCAST_ID;
    gbInstructionPacket[INSTRUCTION] = INST_SYNC_WRITE;
    gbInstructionPacket[PARAMETER] = (unsigned char)address;
    gbInstructionPacket[PARAMETER+1] = 4;
    int size = allIDs.size();
    for (int ii = 0; ii < size; ii++)
    {
        gbInstructionPacket[PARAMETER+2+5*ii] = allIDs[ii];
        gbInstructionPacket[PARAMETER+2+5*ii+1] = dxl_get_lowbyte(values1[ii]);
        gbInstructionPacket[PARAMETER+2+5*ii+2] = dxl_get_highbyte(values1[ii]);
        gbInstructionPacket[PARAMETER+2+5*ii+3] = dxl_get_lowbyte(values2[ii]);
        gbInstructionPacket[PARAMETER+2+5*ii+4] = dxl_get_highbyte(values2[ii]);
    }
    gbInstructionPacket[LENGTH] = 5*size+4;

    dxl_txrx_packet();
}

void dynamixelClass::dxl_bulk_rx_packet(int id)
{
    unsigned char i, j, nRead;
    unsigned char checksum = 0;

    if( giBusUsing == 0 )
        return;

    if (gbCommStatus != COMM_RXWAITING)
    {
        gbRxGetLength = 0;
        gbRxPacketLength = 6;
    }

    nRead = dxlhal.dxl_hal_rx( (unsigned char*)&gbStatusPacket[gbRxGetLength], gbRxPacketLength - gbRxGetLength );
    gbRxGetLength += nRead;
    if( gbRxGetLength < gbRxPacketLength )
    {
        if( dxlhal.dxl_hal_timeout() == 1 )
        {
            if(gbRxGetLength == 0)
                gbCommStatus = COMM_RXTIMEOUT;
            else
                gbCommStatus = COMM_RXCORRUPT;
            giBusUsing = 0;
            return;
        }
    }

    // Find packet header
    for( i=0; i<(gbRxGetLength-1); i++ )
    {
        if( gbStatusPacket[i] == 0xff && gbStatusPacket[i+1] == 0xff )
        {
            break;
        }
        else if( i == gbRxGetLength-2 && gbStatusPacket[gbRxGetLength-1] == 0xff )
        {
            break;
        }
    }
    if( i > 0 )
    {
        for( j=0; j<(gbRxGetLength-i); j++ )
            gbStatusPacket[j] = gbStatusPacket[j + i];

        gbRxGetLength -= i;
    }

    if( gbRxGetLength < gbRxPacketLength )
    {
        gbCommStatus = COMM_RXWAITING;
        return;
    }

    // Check id pairing
    if( id != gbStatusPacket[ID])
    {
        gbCommStatus = COMM_RXCORRUPT;
        giBusUsing = 0;
        return;
    }

    gbRxPacketLength = gbStatusPacket[LENGTH] + 4;
    if( gbRxGetLength < gbRxPacketLength )
    {
        nRead = dxlhal.dxl_hal_rx( (unsigned char*)&gbStatusPacket[gbRxGetLength], gbRxPacketLength - gbRxGetLength );
        gbRxGetLength += nRead;
        if( gbRxGetLength < gbRxPacketLength )
        {
            gbCommStatus = COMM_RXWAITING;
            return;
        }
    }

    // Check checksum
    for( i=0; i<(gbStatusPacket[LENGTH]+1); i++ )
        checksum += gbStatusPacket[i+2];
    checksum = ~checksum;

    if( gbStatusPacket[gbStatusPacket[LENGTH]+3] != checksum )
    {
        gbCommStatus = COMM_RXCORRUPT;
        return;
    }
    gbCommStatus = COMM_RXSUCCESS;
}

// Bulk read, adopts BULK_READ for MX series motors and performs normal READ for non-MX series motors (behavior based on bulkreadFlag)
vector<int> dynamixelClass::dxl_bulk_read(int address, const vector<int>& allIDs, int readLength, const vector<bool>& bulkreadFlag)
{
    while(giBusUsing);

    gbInstructionPacket[ID] = BROADCAST_ID;
    gbInstructionPacket[INSTRUCTION] = INST_BULK_READ;
    gbInstructionPacket[PARAMETER] = 0;
    int size = allIDs.size();
    queue<int> bulkreadID, normreadID;
    int idx = 0;
    for (int ii = 0; ii < size; ii++)
    {
        if (bulkreadFlag[ii])
        {
            gbInstructionPacket[PARAMETER+1+3*idx] = readLength;
            gbInstructionPacket[PARAMETER+1+3*idx+1] = allIDs[ii];
            gbInstructionPacket[PARAMETER+1+3*idx+2] = (unsigned char)address;
            bulkreadID.push(ii);
            idx++;
        }
        else
            normreadID.push(ii);
    }
    gbInstructionPacket[LENGTH] = 3*bulkreadID.size()+3;

    dxl_tx_packet();
    vector<int> allData(size,-1);
    if (readLength == 4)
        allData.resize(2*readLength,-1);

    bulkCommStatus.resize(size, COMM_RXSUCCESS);
    bulkError.resize(size,0);

    if( gbCommStatus == COMM_TXSUCCESS )
    {
        while(!bulkreadID.empty())
        {
            int ind = bulkreadID.front();
            bulkreadID.pop();

            do{
                dxl_bulk_rx_packet(allIDs[ind]);
            }while( gbCommStatus == COMM_RXWAITING );

            switch(readLength)
            {
            case 1:
                allData[ind] = (int)gbStatusPacket[PARAMETER];
                break;
            case 2:
                allData[ind] = dxl_makeword((int)gbStatusPacket[PARAMETER],(int)gbStatusPacket[PARAMETER + 1]);
                break;
            case 4:
                allData[ind] = dxl_makeword((int)gbStatusPacket[PARAMETER],(int)gbStatusPacket[PARAMETER + 1]);
                allData[size + ind] = dxl_makeword((int)gbStatusPacket[PARAMETER + 2],(int)gbStatusPacket[PARAMETER + 3]);
                break;
            default:
                throw(runtime_error("Unable to handle read length."));
                break;
            }
            bulkCommStatus[ind] = gbCommStatus;
            bulkError[ind] = gbStatusPacket[ERRBIT];
        }
    }
    giBusUsing = 0;

    while(!normreadID.empty())
    {
        int ind = normreadID.front();
        normreadID.pop();
        switch(readLength)
        {
        case 1:
            allData[ind] = dxl_read_byte(allIDs[ind],address);
            break;
        case 2:
            allData[ind] = dxl_read_word(allIDs[ind],address);
            break;
        case 4:
            allData[ind] = dxl_read_word(allIDs[ind],address);
            allData[size + ind] = dxl_read_word(allIDs[ind],address+2);
            break;
        default:
            throw(runtime_error("Unable to handle read length."));
            break;
        }
        bulkCommStatus[ind] = gbCommStatus;
        bulkError[ind] = gbStatusPacket[ERRBIT];
    }
    return allData;

}
