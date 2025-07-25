#include "touch.h"
#include "lpc24xx.h"

#define CS_PIN            0x00100000        //P0.20

static unsigned char touch_read(unsigned char command);

void touch_init(void)
{
	//Implement this as you see fit
	//Remember to setup CS_TP as a GPIO output
}

void touch_read_xy(char *x, char* y)
{
	//Read X co-ordinate from the touch screen controller
	*x = touch_read(0x<INSERT COMMAND BYTE>);
	//Read Y co-ordinate from the touch screen controller
	*y = touch_read(0x<INSERT COMMAND BYTE>);
}

static unsigned char touch_read(unsigned char command)
{
	unsigned short result;

	//Set CS_TP pin low to begin SPI transmission
	
	
	//Transmit command byte on MOSI, ignore MISO (full read write cycle)
	
	
	//Transmit 0x00 on MOSI, read in requested result on MISO (another full read write cycle)
	
	
	//Transmission complete, set CS_TP pin back to high
	
	
	//Return 8 bit result.
	return (unsigned char) result;
}	

