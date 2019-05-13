#include "LSM303.h"

const double PI = 3.1415926535897;
char config[2] = {0};

void init_LSM303(void)
{
     //int file;
     char *bus = "/dev/i2c-1";
     if ((fd_LSM = open(bus, O_RDWR)) < 0)
     {
          printf("Failed to open the bus. \n");
          exit(1);
     }
     // Get I2C device, LSM303DLHC MAGNETO I2C address is 0x1E(30)
     ioctl(fd_LSM, I2C_SLAVE, 0x1E);

     // Select MR register(0x02)
     // Continuous conversion(0x00)
     config[0] = 0x02;
     config[1] = 0x00;
     write(fd_LSM, config, 2);
     // Select CRA register(0x00)
     // Data output rate = 15Hz(0x10)
     config[0] = 0x00;
     config[1] = 0x10;
     write(fd_LSM, config, 2);
     // Select CRB register(0x01)
     // Set gain = +/- 1.3g(0x20)
     config[0] = 0x01;
     config[1] = 0x20;
     write(fd_LSM, config, 2);
     sleep(1);
}


float get_heading_reading(void)
{
     char reg[1] = {0};
     char data[1] = {0};
     // Read 6 bytes of data
     // msb first
     // Read xMag msb data from register(0x03)
     reg[0] = 0x03;
     write(fd_LSM, reg, 1);
     read(fd_LSM, data, 1);
     char data1_0 = data[0];

     // Read xMag lsb data from register(0x04)
     reg[0] = 0x04;
     write(fd_LSM, reg, 1);
     read(fd_LSM, data, 1);
     char data1_1 = data[0];

     // Read zMag msb data from register(0x05)
     reg[0] = 0x05;
     write(fd_LSM, reg, 1);
     read(fd_LSM, data, 1);
     char data1_2 = data[0];

     // Read zMag lsb data from register(0x06)
     reg[0] = 0x06;
     write(fd_LSM, reg, 1);
     read(fd_LSM, data, 1);
     char data1_3 = data[0];

     // Read yMag msb data from register(0x07)
     reg[0] = 0x07;
     write(fd_LSM, reg, 1);
     read(fd_LSM, data, 1);
     char data1_4 = data[0];

     // Read yMag lsb data from register(0x08)
     reg[0] = 0x08;
     write(fd_LSM, reg, 1);
     read(fd_LSM, data, 1);
     char data1_5 = data[0];

     // Convert the data
     int xMag = (data1_0 * 256 + data1_1);
     if (xMag > 32767)
     {
          xMag -= 65536;
     }

     int yMag = (data1_4 * 256 + data1_5);
     if (yMag > 32767)
     {
          yMag -= 65536;
     }

     int zMag = (data1_2 * 256 + data1_3);
     if (zMag > 32767)
     {
          zMag -= 65536;
     }

     // Output data to screen

     float heading = (atan2(yMag, xMag) * 180) / PI;
     if (heading < 0) {
          heading += 360;
     }
     return heading;
}