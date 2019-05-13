#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

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

#define enable_pin 23 //enable pin for the motor controller,
#define MS1_pin 21    //MS1 pin, board# 16,
#define MS2_pin 22    //MS2 pin, board# 18
#define step_pin 4    //step pin is board#11
#define dir_pin 5     //step pin is board#11

void init_motor_driver(void);
int calculate_step_count (float voltage, float prev_voltage);

#endif