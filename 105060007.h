#include <math.h>

#define TRUNCATION_LENGTH 32

enum{
    HARD,               // decode on a BSC, SNR: 1~6dB
    UNQUANTIZED_SOFT    // 1~4dB, increment = 0.5dB
};

unsigned long long SEED = 666;
unsigned long long RANV;
int RANI = 0;
double n1, n2;

double Ranq1();

// output 2 indep. normal RV n1 & n2
void normal(double delta){
    double x1, x2, s;
    do{
        x1 = Ranq1();
        x2 = Ranq1();
        x1 = 2*x1 - 1;
        x2 = 2*x2 - 1;
        s = x1*x1 + x2*x2;
    } while(s>=1.0);
    double a = (-2)*log(s) / s;
    n1 = delta*x1*pow(a, 0.5);
    n2 = delta*x2*pow(a, 0.5);
}

double Ranq1(){
    if(RANI==0){
        RANV = SEED ^ 4101842887655102017LL;
        RANV ^= RANV >> 21;
        RANV ^= RANV << 35;
        RANV ^= RANV >> 4;
        RANV *= 2685821657736338717LL;
        RANI++;
    }
    RANV ^= RANV >> 21;
    RANV ^= RANV << 35;
    RANV ^= RANV >> 4;
    return RANV * 2685821657736338717LL * 5.42101086242752217E-20;
}