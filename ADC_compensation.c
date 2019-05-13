#include "ADC_compensation.h"

/**
 * The ADC_compensation function compensates the imperfect non-linear relationship between input voltage values and output digital values and fix this relationship to become perfectly linear. 
 */
unsigned int ADC_compensation(unsigned int ADC_val) {

    double x[11] = { 0,0.33,0.66,0.99,1.32,1.65,1.98,2.31,2.64,2.97,3.3 };
    unsigned int y[11] = { 0,2655,5313,7971,10624,13282,15933,18560,21261,23839,26460 };
    int i = 0;
    double a = 26400 / 3.3;
    double ADC_voltage = 0.0;

    for (int i = 0; i < 11; i++) {
        if ((ADC_val >= y[i]) && (ADC_val <= y[i + 1])) {
            double slope = ((double)(y[i + 1] - y[i])) / ((double)(x[i + 1] - x[i]));
            ADC_voltage = ((double)(ADC_val - y[i]) / (slope)) + x[i];

            double q = y[i] - slope * x[i];
            ADC_val += ((a - slope)*ADC_voltage - q);
            return ADC_val;
        }
    }
}