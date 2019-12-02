#include <iostream>
#include "vector"
#include <string.h>
#include <queue>
#include "./105060007.h"
#include "./LinkedList.h"
#include "./Viterbi.h"
#include "bitset"
using namespace std;

/*--------------------------------------------------------*/
// input:
//      # of decoded bits N
//      the bit signal-to-noise ratio SNR (in dB)
//      the seed for the random generator SEED
//      hard/soft decsion

// viterbi decode:
//      metric = the distance of the shortest path up to now
//      survivor = the shortest path up to now
//      state s = (s1, s2, ... , s5, s6)
//          input first fed to s1, shift to right
//      same metric => choose the upmost branch as survivor
//      
/*--------------------------------------------------------*/

int N = 1;
double SNR;
double var = 0.3981; // variance
int decision = HARD;

vector<bool> u;
// bool u[63] = {true, false, false, false, false, false}; // input info bits
vector<bool> x1, x2;
vector<double> Yhat1, Yhat2;    // (AWGN) channel output
queue< bitset<6>> state;

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
    state.push(bitset<6>(0));
    u.resize(N+31, false);
    u[0] = true;
    cout << "info: ";
    for(int i=0; i<=N+31-6; i++) u[i+6] = (u[i] + u[i+1]) % 2;
    for(int i=0; i<N+31; i++){
        bitset<6> last = state.back();
        last <<= 1;
        if(u[i]) last.set(0);
        state.push(last);
    }
        
    
    for(int i=0; i<N+31; i++) cout << u[i] << " ";
    cout << endl;

    
        cout << "state~~~~" << endl;
    for(int i=0; i<state.size();){
        cout << state.front() << " ";
        state.pop();
    }
    cout << endl;
}

// Encode the information sequence
void Encode(){      
    x1.resize(N+31);
    x2.resize(N+31);
    
    // initial state=000000
    for(int i=0; i<6; i++) u.insert(u.begin(), false);

    for(int i=6; i<N+31+6; i++){
        x1[i-6] = (u[i] + u[i-2] + u[i-3] + u[i-5] + u[i-6])%2;
        x2[i-6] = (u[i] + u[i-1] + u[i-2] + u[i-3] + u[i-6])%2;
    }

    // cout << endl << "x1: ";
    // for(int i=0; i<N+31; i++) cout << x1[i] << " ";
    // cout << endl << "x2: ";
    // for(int i=0; i<N+31; i++) cout << x2[i] << " "; 
    for(int i=0; i<N+31; i++) cout << x1[i] << x2[i] << " ";
}

void AWGN(){
    double a, b;
    // normalize the input to AWGN to +-1
    for(int i=0; i<N+31; i++){
        a = (x1[i]==0) ? 1 : -1;
        b = (x2[i]==0) ? 1 : -1;

        normal(pow(var, 0.5));
        a = a + n1;
        b = b + n2;

        if(decision == HARD){
            Yhat1[i] = (a < 0) ? -1 : 1;
            Yhat2[i] = (b < 0) ? -1 : 1;

            Yhat1[i] = (Yhat1[i] == 1) ? 0 : 1;
            Yhat2[i] = (Yhat2[i] == 1) ? 0 : 1;
        }
        else if(decision == UNQUANTIZED_SOFT){
            Yhat1[i] = a;
            Yhat2[i] = b;
        }
        else cout << "which decision~~~:3?";
    }
}

int main(){
    // Initialize();
    InputGenerator();
    Encode();
    // AWGN();
    
    decode(N+31);
}