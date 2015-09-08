#ifndef DXLHAL_H
#define DXLHAL_H

class DXLHAL
{
public:
    DXLHAL();
    ~DXLHAL();

    int	gSocket_fd;
    long	glStartTime;
    float	gfRcvWaitTime;
    float	gfByteTransTime;
    char	gDeviceName[20];

    int dxl_hal_open(int deviceIndex, float baudrate);
    void dxl_hal_close();
    int dxl_hal_set_baud( float baudrate );
    void dxl_hal_clear();
    int dxl_hal_tx( unsigned char *pPacket, int numPacket );
    int dxl_hal_rx( unsigned char *pPacket, int numPacket );
    void dxl_hal_set_timeout( int NumRcvByte );
    int dxl_hal_timeout();
};

#endif // DXLHAL_H
