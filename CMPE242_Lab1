/**
 * Student Name: Chong Hang Cheong
 * Student ID: 007323537
 * Class : CMPE 242
 * Lab 1
 * Date : 03/12/2019
 * Prof. Hua Li
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>    
#include <fcntl.h>
#include <unistd.h>    // read/write usleep
#include <stdlib.h>    // exit function
#include <inttypes.h>  // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define SAMPLE_NUM 8192
 // Connect ADDR to GRD.
 // Setup to use ADC0 single ended
int fd;
int asd_address = 0x48;
int16_t val;
uint8_t writeBuf[3];
uint8_t readBuf[2];
unsigned int ADC_result = 0;
float voltage_float = 0.0;
double total_time = 0;
double avg_time = 0.0;
const float VPS = 4.096 / 32768.0; // volts per step
unsigned int samples[SAMPLE_NUM + 1] = { 0 };
double samples_compensated[SAMPLE_NUM + 1] = { 0 };
double start_time[SAMPLE_NUM + 1] = { 0 };
double end_time[SAMPLE_NUM + 1] = { 0 };
double power_spectrum[SAMPLE_NUM + 1] = { 0 };
struct timespec time_start;
struct timespec time_end;
int error_count = 0;

struct Complex
{
    double a;        //Real Part
    double b;        //Imaginary Part
}          X[SAMPLE_NUM + 1], U, W, T, Tmp;

/**
 * FFT function is provide by Prof.
 * Input
 * N - Number of Sample points
 * M - 2^M = N
 * 
 * Output
 * FFT result is stored in struct member, Complex X[]
 */

void FFT(void)
{
    int M = log2(SAMPLE_NUM);
    int N = pow(2, M);

    int i = 1, j = 1, k = 1;
    int LE = 0, LE1 = 0;
    int IP = 0;

    for (k = 1; k <= M; k++)
    {
        LE = pow(2, M + 1 - k);
        LE1 = LE / 2;

        U.a = 1.0;
        U.b = 0.0;

        W.a = cos(M_PI / (double)LE1);
        W.b = -sin(M_PI / (double)LE1);

        for (j = 1; j <= LE1; j++)
        {
            for (i = j; i <= N; i = i + LE)
            {
                IP = i + LE1;
                T.a = X[i].a + X[IP].a;
                T.b = X[i].b + X[IP].b;
                Tmp.a = X[i].a - X[IP].a;
                Tmp.b = X[i].b - X[IP].b;
                X[IP].a = (Tmp.a * U.a) - (Tmp.b * U.b);
                X[IP].b = (Tmp.a * U.b) + (Tmp.b * U.a);
                X[i].a = T.a;
                X[i].b = T.b;
            }
            Tmp.a = (U.a * W.a) - (U.b * W.b);
            Tmp.b = (U.a * W.b) + (U.b * W.a);
            U.a = Tmp.a;
            U.b = Tmp.b;
        }
    }

    int NV2 = N / 2;
    int NM1 = N - 1;
    int K = 0;

    j = 1;
    for (i = 1; i <= NM1; i++)
    {
        if (i >= j) goto TAG25;
        T.a = X[j].a;
        T.b = X[j].b;

        X[j].a = X[i].a;
        X[j].b = X[i].b;
        X[i].a = T.a;
        X[i].b = T.b;
    TAG25:    K = NV2;
    TAG26:    if (K >= j) goto TAG30;
        j = j - K;
        K = K / 2;
        goto TAG26;
    TAG30:    j = j + K;
    }
}

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

int main() {

    // open device on /dev/i2c-1 the default on Raspberry Pi B
    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {
        printf("Error: Couldn't open device! %d\n", fd);
        exit(1);
    }

    // connect to ADS1115 as i2c slave
    if (ioctl(fd, I2C_SLAVE, asd_address) < 0) {
        printf("Error: Couldn't find device on address!\n");
        exit(1);
    }

    // set config register and start conversion
    // AIN0 and GND, 4.096v, 860s/s
    // Refer to page 19 area of spec sheet
    writeBuf[0] = 1; // config register is 1
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

    if (write(fd, writeBuf, 3) != 3) {
        perror("Write to register 1");
        exit(1);
    }


    printf("-----------------------Ready!---------------------------\n");
    sleep(1);

    readBuf[0] = 0;
    if (write(fd, readBuf, 1) != 1) {
        perror("Conversion register select");
        exit(-1);
    }

    int count = 1;
    printf("-----------------------Start!---------------------------\n");
    while (1) {

        clock_gettime(CLOCK_REALTIME, &time_start);
        start_time[count] = time_start.tv_nsec;
        // read conversion register
        if (read(fd, readBuf, 2) != 2) {
            perror("Read conversion");
            exit(-1);
        }
        // could also multiply by 256 then add readBuf[1]
        ADC_result = readBuf[0] << 8 | readBuf[1];

        // with +- LSB sometimes generates very low neg number.
        if (ADC_result < 0)   ADC_result = 0;

        samples[count] = ADC_result;

        count++;
        if (count == (SAMPLE_NUM+1))   break;
        usleep(590);
        clock_gettime(CLOCK_REALTIME, &time_end);
        end_time[count - 1] = time_end.tv_nsec;
    }

    printf("Sampling is done!\n");
    FILE *fptr10 = fopen("time.txt", "w");
    for (int i = 1; i < SAMPLE_NUM+1; i++) {
        int time_consumption = (end_time[i] - start_time[i]) / 1000;
        if (time_consumption > 0) {
            total_time += time_consumption;
            fprintf(fptr10,"Time it takes for the conversion: %d us \n", time_consumption);
        }
        else {
            error_count++;
        }
    }
    printf("Total Conversion Time it takes: %f us \n", total_time);
    printf("Avg Conversion Time it takes: %f us \n", total_time / (SAMPLE_NUM - error_count));
    fprintf(fptr10,"Total Conversion Time it takes: %f us \n", total_time);
    fprintf(fptr10,"Avg Conversion Time it takes: %f us \n", total_time / (SAMPLE_NUM - error_count));
    fclose(fptr10);
    // power down ADS1115
    writeBuf[0] = 1;    // config register is 1
    writeBuf[1] = 0b11000011; // bit 15-8 0xC3 single shot on
    writeBuf[2] = 0b10000101; // bits 7-0  0x85
    if (write(fd, writeBuf, 3) != 3) {
        perror("Write to register 1");
        exit(1);
    }

    close(fd);

    FILE *fptr = fopen("samples.txt", "w");
    for (int i = 0; i < SAMPLE_NUM; i++){
        fprintf(fptr, "%d\n",samples[i]);
    }
    fclose(fptr);


    /**
     * Use Compensation Function to make the relationship between input voltage and output digital values to become perfectly linear
     * 
     */

     FILE *fptr2 = fopen("samples_compensated.txt", "w");
    for (int i = 0; i <= SAMPLE_NUM; i++) {
        samples_compensated[i] = ADC_compensation(samples[i]) * (4.096 * 2) / (pow(2, 16));
        fprintf(fptr2, "%f\n",samples_compensated[i]);
    }
    fclose(fptr2);
    /**
     * Run the FFT to decompose the sequence of our digitized input values into components of different frequencies. 
     *
     */

    for (int i = 1; i <= SAMPLE_NUM; i++)
    {
        X[i].a = samples_compensated[i];
        X[i].b = 0.0;
    }
    
    FILE *fptr3 = fopen("fft_before.txt", "w");

    for (int i = 0; i <= SAMPLE_NUM; i++){
        //printf("X[%d]:real == %f  imaginary == %f\n", i, X[i].a, X[i].b);
        fprintf(fptr3,"X[%d]:real == %f  imaginary == %f\n", i, X[i].a, X[i].b);
    }
    fclose(fptr3);    
    FFT();
    FILE *fptr4 = fopen("fft_after.txt", "w");
    for (int i = 0; i <= SAMPLE_NUM; i++){
        //printf("X[%d]:real == %f  imaginary == %f\n", i, X[i].a, X[i].b);
        fprintf(fptr4,"X[%d]:real == %f  imaginary == %f\n", i, X[i].a, X[i].b);
    }
    fclose(fptr4);       

    /**
    * Power spectrum
    * Generate the Plot of Power Spectrum
    */
    
    FILE *fptr5 = fopen("power_spectrum.txt", "w");
    
    for (int i = 0; i <= SAMPLE_NUM; i++) {
        power_spectrum[i] = sqrt(pow((X[i].a), 2) + pow((X[i].b), 2));
        //fprintf(fptr, "power_spectrum[%d] == %f\n", i, power_spectrum[i]);
        fprintf(fptr5, "%d   %f\n", i, power_spectrum[i]);
    }
    fclose(fptr5);
    
    FILE *fptr6 = fopen("power_spectrum_analysis.txt", "w");
    float m0_index = 0.80;
    int m0 = m0_index*(SAMPLE_NUM/2 -1);
    fprintf(fptr6,"m0 = %f\n", m0_index);
    double total_energy = 0;
    double energy_m0 = 0;

    for (int i = 0; i < (SAMPLE_NUM/2 -1); i++) {
        if (i <= m0) {
            energy_m0 += power_spectrum[i];
        }
        total_energy += power_spectrum[i];
    }

    /**
     * Check if the efficiency is higher than 80% when m0 is set to 0.8 * (SAMPEL_NUM/2 - 1)
     * Also, print both the highest frequency component and the highest frequency index onto the console window
     */
    double efficiency = (energy_m0/total_energy)*100;
    printf("efficiency = %f\%\n", efficiency);
    fprintf(fptr6,"efficiency = %f\%\n", efficiency);
    //if (power_spectrum[((SAMPLE_NUM - 1) / 2) - 1] < 0.1) {
    if(efficiency >= m0_index*100){    
        printf("No aliasing! :D\n");
        printf("Highest Frequency Index: %d\n", ((SAMPLE_NUM - 1) / 2) - 1);
        printf("Highest Frequency Component: %f\n", power_spectrum[((SAMPLE_NUM - 1) / 2) - 1]);
        fprintf(fptr6,"Aliasing does not occur!\n");
        fprintf(fptr6,"Highest Frequency Index: %d\n", ((SAMPLE_NUM - 1) / 2) - 1);
        fprintf(fptr6,"Highest Frequency Component: %f\n", power_spectrum[((SAMPLE_NUM - 1) / 2) - 1]);
    }
    else {
        printf("Aliasing has occured! :( \n");
        fprintf(fptr6, "Aliasing has occured!\n");
    }

    fclose(fptr6);
    return 0;
}

sampled_data[SAMPLE_NUM] = {};
//Sample the input data
while (int i = 0 < SAMPEL_NUM){
    sampled_data[i] = read_ADC();
}
//Use Compensation Function to make the relationship between input voltage and output digital values to become perfectly linear
while (int i = 0 < SAMPEL_NUM){
    sampled_data_in_V[i] = ADC_compensation(sampled_data[i]) * 3.3/26400;
}
// Run the FFT and generate the power_spectrum plot
FFT();
while (int i = 0 < SAMPEL_NUM){
    power_spectrum[i] = sqrt(pow((X[i].a), 2) + pow((X[i].b), 2));
}
// Check if aliasing has occured based on the criteria: efficiency >= 80%
if(efficiency >= 80)
{
    printf("No aliasing!\n");
    printf("Highest Frequency Index: %d\n", ((SAMPLE_NUM - 1) / 2) - 1);
    printf("Highest Frequency Component: %f\n", power_spectrum[((SAMPLE_NUM - 1) / 2) - 1]);
}
