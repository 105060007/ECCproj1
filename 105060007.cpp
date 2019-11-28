#include <iostream>
#include "vector"
#include <string.h>
#include <queue>
#include "./105060007.h"
using namespace std;

/*--------------------------------------------------------*/
// input:
//     # of decoded bits N
//     the bit signal-to-noise ratio SNR (in dB)
//     the seed for the random generator SEED
//     hard/soft decsion
/*--------------------------------------------------------*/

int N = 33;
double SNR;
double var = 0.3981; // variance
int decision = HARD;

vector<bool> u;
// bool u[63] = {true, false, false, false, false, false}; // input info bits
vector<bool> x1, x2;
vector<double> Yhat1, Yhat2;    // (AWGN) channel output

// get user input
void Initialize(){
    double SNRindB;
    cin >> N;
    cin >> SNRindB;
    // 10 log10(SNR) = SNRindB
    SNR = pow(10.0, SNRindB/10);
    var = 1/SNR;
    cin >> SEED;
    // input set decision~~~?
}

// generate the information bits
void InputGenerator(){
    u.resize(N+31, false);
    u[0] = true;
    cout << "info: ";
    for(int i=0; i<=N+31-6; i++)
        u[i+6] = (u[i] + u[i+1]) % 2;
    // for(int i=0; i<N+31; i++) cout << u[i] << " ";
}

// Encode the information sequence
void Encode(){      
    x1.resize(N+31);
    x2.resize(N+31);
    
    // initial state=000000 + initial input=100000
    // use queue to store state?(feed forward shift reg)
    for(int i=0; i<6; i++) u.insert(u.begin(), false);

    for(int i=6; i<N+31+6; i++){
        x1[i-6] = (u[i] + u[i-2] + u[i-3] + u[i-5] + u[i-6])%2;
        x2[i-6] = (u[i] + u[i-1] + u[i-2] + u[i-3] + u[i-6])%2;
    }

    cout << endl << "x1: ";
    for(int i=0; i<N+31; i++) cout << x1[i] << " ";
    cout << endl << "x2: ";
    for(int i=0; i<N+31; i++) cout << x2[i] << " "; 
}

void HorSDecision(){
    double a, b;
    if(decision == HARD){
        for(int i=0; i<N+31; i++){
            normal(pow(var, 0.5));
            a = x1[i] + n1;
            b = x2[i] + n2;

            Yhat1[i] = (a < 0) ? -1 : 1;
            Yhat2[i] = (b < 0) ? -1 : 1;
        }
    }
    else if(decision == UNQUANTIZED_SOFT){
        for(int i=0; i<N+31; i++){
            normal(pow(var, 0.5));
            a = x1[i] + n1;
            b = x2[i] + n2;

            // soft decision...
        }
    }
    else{
        cout << "which decision~~~:3?";
    }

}

int main(){
    // Initialize();
    InputGenerator();
    Encode();
    HorSDecision();

    // Viterbi Decoding
}