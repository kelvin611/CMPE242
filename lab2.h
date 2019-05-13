#ifndef LAB2_H
#define LAB2_H

unsigned int read_ADC(int fd, uint8_t readBuf, int reg)
void init_motor_driver (void);
void init_LSM303(void);
void init_ADC(void);
float get_heading_reading(void);


#endif