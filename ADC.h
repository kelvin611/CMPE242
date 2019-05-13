#ifndef ADC_H
#define ADC_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>        // read/write usleep
#include <stdlib.h>        // exit function
#include <inttypes.h>      // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <wiringPi.h>

int fd_ADC;
int16_t val;
uint8_t writeBuf[3];
uint8_t readBuf[2];

void init_ADC(void);
unsigned int read_ADC(int fd, uint8_t *readBuf, int reg);

#endif