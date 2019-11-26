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

vector<bool> u;
// bool u[63] = {true, false, false, false, false, false}; // input info bits
vector<bool> x1;
vector<bool> x2;

// get user input
void Initialize(){
    cin >> N;
    cin >> SNR;
    cin >> SEED;
}

// generate the information bits
void InputGenerator(){
    u.resize(N+31, false);
    u[0] = true;
    // u[0] = true; u[1] = false; u[2] = false; 
    // u[3] = false; u[4] = false; u[5] = false;
    cout << "info: ";
    for(int i=0; i<=N+31-6; i++){
        u[i+6] = (u[i] + u[i+1]) % 2;
    }
    for(int i=0; i<N+31; i++) cout << u[i] << " ";
}

// Encode the information sequence
void Encode(){      
    x1.resize(N+31);
    x2.resize(N+31);
    // initial state=000000 + initial input=100000
    // bool tmp[12] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
    // for(int i=6; i<12; i++){
    //     x1[i-6] = (tmp[i] + tmp[i-2] + tmp[i-3] + tmp[i-5] + tmp[i-6])%2;
    //     x2[i-6] = (tmp[i] + tmp[i-1] + tmp[i-2] + tmp[i-3] + tmp[i-6])%2;
    // }

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

int main(){
    // Initialize();
    InputGenerator();
    
    Encode();

}