/**
 * Student Name: Chong Hang Cheong
 * Student ID: 007323537
 * Class : CMPE 242
 * Lab 2
 * Date : 03/12/2019
 * Prof. Hua Li
 */

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
#include "ADC_compensation.h"
#include "ADC.h"
#include "LSM303.h"
#include "motor_driver.h"
#include <softPwm.h>

// Connect ADDR to GRD.
// Setup to use ADC0 single ended
//int fd_ADC;
//int fd_LSM;
//int asd_address = 0x48;
// int16_t val;
// uint8_t writeBuf[3];
// uint8_t readBuf[2];
// char config[2] = {0};
const float volt_per_step = 4.096 / 32768.0; // volts per step
//const float volt_per_full_step = (1.8 / (180 / 1.65));
int step_count = 0;
float prev_voltage = 0;
int start_flag = 0;

//const double PI = 3.1415926535897;

/*
https://pinout.xyz/pinout
*/

#define start_pin 29 // button, wiringPi [29], physicalPin [40], GPIO21


int read_start_pin (void){
     return digitalRead(start_pin);
}

int main()
{    

     wiringPiSetup(); //setup wiringPi to use the wiringPi pin numbers
     init_motor_driver(); //
     init_ADC();
     init_LSM303();
     
     pinMode(start_pin, INPUT);
     pullUpDnControl(start_pin, PUD_UP);
     //softPwmCreate(step_pin, 0, 33);

     FILE *fptr_ADC_heading = fopen("ADC_compensation_heading.txt", "w");

     while (1)
     {
          
          if (start_flag == 0)
          {
               //printf("start: %d\n", !read_start_pin());
               if (!read_start_pin())
               {
                    start_flag = 1;
                    printf("=============================Start=============================\n");
                    sleep(1);
               }
          }

          while (start_flag)
          {
               
               float heading_value = 0;
               unsigned int ADC_result = read_ADC(fd_ADC, readBuf, 2);
               unsigned int compensated_value = ADC_compensation(ADC_result);
               //printf("ADC: %d  \t", compensated_value);
               double voltage_value = compensated_value * volt_per_step;
               double step_count = calculate_step_count(voltage_value, prev_voltage);

               //printf("voltage_value: %f\t", voltage_value);

               prev_voltage = voltage_value;
               /*
               int pwmValue = 95;
               if (step_count == 0){
                    softPwmWrite (step_pin, 0) ;
               }
               else {
                    */
               for (int i = 0; i < step_count; i++)
               {
                    /*
                    if (pwmValue > 100)
                    pwmValue =0;

                    softPwmWrite (step_pin, 1) ;
                    */
                    
                    digitalWrite(step_pin, HIGH);
                    // ADC_result = read_ADC(fd_ADC, readBuf, 2);
                    // compensated_value = ADC_compensation(ADC_result);
                    // voltage_value = compensated_value * volt_per_step;
                    // heading_value = get_heading_reading();
                    // printf("voltage_value: %f\t", voltage_value);
                    // printf("heading_value: %f\n", heading_value);
                    //usleep(500);
                    int high_time_us = (int)(compensated_value/26460)*1000;
                    usleep(high_time_us);
                    digitalWrite(step_pin, LOW);
                    // ADC_result = read_ADC(fd_ADC, readBuf, 2);
                    // compensated_value = ADC_compensation(ADC_result);
                    // voltage_value = compensated_value * volt_per_step;
                    // heading_value = get_heading_reading();
                    // printf("voltage_value: %f\t", voltage_value);
                    // printf("heading_value: %f\n", heading_value);
                    int low_time_us = 1000 - high_time_us;
                    usleep(low_time_us);
                    //usleep(500);
                    
                  //usleep(1000);

               }
               
               usleep(15000); //10000
               heading_value = get_heading_reading();
               printf("ADC_compensated_value: %d  \t", compensated_value);
               printf("voltage_value: %f\t", voltage_value);
               printf("heading_value: %f\n", heading_value);
               fprintf(fptr_ADC_heading,"ADC_compensated_value: %d  \t", compensated_value);
               fprintf(fptr_ADC_heading,"voltage_value: %f\t", voltage_value);
               fprintf(fptr_ADC_heading,"heading_value: %f\n", heading_value);
               //int button_reading = digitalRead(start_pin);
               if (!read_start_pin())
               {
                    start_flag = 0;
                    printf("=============================Pause=============================\n");
                    fclose(fptr_ADC_heading);
                    usleep(500000);
               }
               
               //heading_value = get_heading_reading();
               //printf("heading_value: %f\n", heading_value);
          }
          
     }
     
}
