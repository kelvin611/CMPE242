#ifndef LSM303_H
#define LSM303_H

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

int fd_LSM;

void init_LSM303(void);
float get_heading_reading(void);

#endif