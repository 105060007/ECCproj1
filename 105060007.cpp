#include <iostream>
#include "vector"
#include <string.h>
#include "./105060007.h"
using namespace std;

/*--------------------------------------------------------*/
// input:
//     # of decoded bits N
//     the bit signal-to-noise ratio SNR (in dB)
//     the seed for the random generator SEED
//     hard/soft decsion
/*--------------------------------------------------------*/

int N;
double SNR;

bool u[63] = {true, false, false, false, false, false}; // input info bits
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
    // u[0] = true; u[1] = false; u[2] = false; 
    // u[3] = false; u[4] = false; u[5] = false;
    for(int i=0; i<=57; i++){
        u[i+6] = (u[i] + u[i+1]) % 2;
    }
    
    // if input length > 63
    // for(int i=63; i<83; i++)
    //     cout << (u[(i-5)%63] + u[(i-6)%63])%2 << " ";
    // cout << endl;
}

// Encode the information sequence
void Encode(){      
    
}

int main(){
    // Initialize();
    InputGenerator();
    for(int i=0; i<63; i++){
        cout << u[i] << " ";
    }
    

}