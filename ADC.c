#include "ADC.h"

int asd_address = 0x48;

void init_ADC(void){
     // open device on /dev/i2c-1 the default on Raspberry Pi B
     if ((fd_ADC = open("/dev/i2c-1", O_RDWR)) < 0)
     {
          printf("Error: Couldn't open device! %d\n", fd_ADC);
          exit(1);
     }

     // connect to ADS1115 as i2c slave
     if (ioctl(fd_ADC, I2C_SLAVE, asd_address) < 0)
     {
          printf("Error: Couldn't find device on address!\n");
          exit(1);
     }

     // set config register and start conversion
     // AIN0 and GND, 4.096v, 860s/s
     // Refer to page 19 area of spec sheet
     writeBuf[0] = 1;          // config register is 1
     writeBuf[1] = 0b11000010; // 0xC2 single shot off
     // bit 15 flag bit for single shot not used here
     // Bits 14-12 input selection:
     // 100 ANC0; 101 ANC1; 110 ANC2; 111 ANC3
     // Bits 11-9 Amp gain. Default to 010 here 001 P19
     // Bit 8 Operational mode of the ADS1115.
     // 0 : Continuous conversion mode
     // 1 : Power-down single-shot mode (default)
     writeBuf[2] = 0b11100000;
     // Bits 7-5 data rate default to 111 for 860SPS
     // Bits 4-0  comparator functions see spec sheet.

     if (write(fd_ADC, writeBuf, 3) != 3)
     {
          perror("Write to register 1");
          exit(1);
     }
     sleep(1);

     readBuf[0] = 0;
     if (write(fd_ADC, readBuf, 1) != 1)
     {
          perror("Conversion register select");
          exit(-1);
     }
}

unsigned int read_ADC(int fd, uint8_t *readBuf, int reg)
{
     // read conversion register
     unsigned int ADC_val = 0;

     if (read(fd, readBuf, 2) != 2)
     {
          perror("Read conversion");
          exit(-1);
     }
     // could also multiply by 256 then add readBuf[1]
     ADC_val = readBuf[0] << 8 | readBuf[1];

     // with +- LSB sometimes generates very low neg number.
     if (ADC_val < 0)
          ADC_val = 0;

     return ADC_val;
}