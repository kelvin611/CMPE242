#include "motor_driver.h"
#include <softPwm.h>

#define enable_pin 23 //enable pin for motor controller, wiringPi [23], physicalPin [33] , GPIO13
#define MS1_pin 21    //MS1 pin, wiringPi [21], physicalPin [29], GPIO5
#define MS2_pin 22    //MS2 pin, wiringPi [22], physicalPin [31], GPIO6
#define step_pin 4    //step pin, wiringPi [4], physicalPin [16], GPIO23
#define dir_pin 5     //dir pin, wiringPi [5], physicalPin [18], GPIO24

const float volt_per_full_step = (1.8 / (180 / 1.65));

void init_motor_driver(void)
{
     pinMode(step_pin, OUTPUT);
     pinMode(enable_pin, OUTPUT);
     pinMode(MS1_pin, OUTPUT);
     pinMode(MS2_pin, OUTPUT);
     pinMode(dir_pin, OUTPUT);

     digitalWrite(enable_pin, LOW); //enable motor control
     digitalWrite(MS1_pin, HIGH); //eighth Step - when both MS1 and MS2 are High. 
     digitalWrite(MS2_pin, HIGH); //eigth Step - when both MS1 and MS2 are High. 
     digitalWrite(step_pin, LOW); // step
     digitalWrite(dir_pin, LOW); //Direction pin is set to low (Direction = CCW)

     
}

int calculate_step_count (float voltage, float prev_voltage) {
     int count = 0;
     if (voltage > 3.3 / 2)
     {
          digitalWrite(dir_pin, LOW);
          count = (voltage - prev_voltage) / (volt_per_full_step/8);
          if (count < 0)
          {
               digitalWrite(dir_pin, HIGH);
               count *= -1;
          }
     }

     else if (voltage < 3.3 / 2)
     {
          digitalWrite(dir_pin, HIGH);
          count = -1 * (voltage - prev_voltage) / (volt_per_full_step/8);
          if (count < 0)
          {
               digitalWrite(dir_pin, LOW);
               count *= -1;
          }
     }
     else
     {
          count = 0;
     }
}


