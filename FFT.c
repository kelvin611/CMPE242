#include "FFT.h"
#include <math.h>
#define SAMPLE_NUM 8192

struct Complex
{
    double a;        //Real Part
    double b;        //Imaginary Part
}   X[SAMPLE_NUM + 1], U, W, T, Tmp;

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