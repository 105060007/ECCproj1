#include <iostream>
#include "vector"
#include <string.h>
#include <queue>
#include <stack>
#include "./105060007.h"
#include "bitset"
using namespace std;

int N = 10;
double SNR;
double var = 0.3981;        // variance
int decision = HARD;
const int StateNum = 64;
int a[StateNum][2];         // 現在這state吃0/1換到下一個state的output
vector<int> q;              // state sequence

vector<bool> u;
vector<bool> x1, x2;
vector<double> Yhat1, Yhat2;    // (AWGN) channel output

std::stack<int> out1;
std::stack<int> out2;

// get user input
void Initialize(){
    cin >> N;               // the # of decoded bit
    double SNRindB;
    cin >> SNRindB;
    // 10 log10(SNR) = SNRindB
    SNR = pow(10.0, SNRindB/10);
    var = 1/SNR;
    cin >> SEED;
    string str;
    cin >> str;
    if(str == "soft") decision = UNQUANTIZED_SOFT;
    else decision = HARD;
}

// generate the information bits
void InputGenerator(){
    u.resize(N+31, false);
    u[0] = true;
    for(int i=0; i<=N+31-6; i++) u[i+6] = (u[i] + u[i+1]) % 2;

    // 紀錄state
    queue< bitset<6> > state;
    state.push(bitset<6>(0));
    for(int i=0; i<N+31; i++){
        bitset<6> last = state.back();
        last <<= 1;
        if(u[i]) last.set(0);
        state.push(last);
    }

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

    cout << "received y: " << endl;
    for(int i=0; i<N+31; i++) cout << x1[i] << x2[i] << " ";
    cout << endl;
}

void AWGN(){
    Yhat1.resize(N+31);
    Yhat2.resize(N+31);
    double tmp1, tmp2;
    // normalize the input to AWGN to +-1
    for(int i=0; i<N+31; i++){
        tmp1 = (x1[i]==0) ? 1 : -1;
        tmp2 = (x2[i]==0) ? 1 : -1;

        normal(pow(var, 0.5));
        tmp1 = tmp1 + n1;
        tmp2 = tmp2 + n2;

        if(decision == HARD){
            Yhat1[i] = (tmp1 < 0) ? 0 : 1;
            Yhat2[i] = (tmp2 < 0) ? 0 : 1;
        }
        else if(decision == UNQUANTIZED_SOFT){
            Yhat1[i] = tmp1;
            Yhat2[i] = tmp2;
        }
        else cout << "which decision~~~:3?";
    }
}

void InitializeA(){
    for(int i=0; i<64; i++){
        // i = 4 = 000100(s6 s5 ... s2 s1), if input = 1  -> next = 001001 = 9
        std::bitset<6> x(i);

        a[i][0] = (0 + x[1] + x[2] + x[4] + x[5])%2*10
                     + (0 + x[0] + x[1] + x[2] + x[5])%2;
                     
        a[i][1] = (1 + x[1] + x[2] + x[4] + x[5])%2*10
                     + (1 + x[0] + x[1] + x[2] + x[5])%2;
    }
}

// 在第t個，output結果 vs AWGN encoded結果之branch metric
double BranchMetric(int output, int t){
    double m = 0;
    double decoded1, decoded2;
    decoded1 = output / 10;
    decoded2 = output % 10;

    // hard or soft
    // if(decision == HARD) m = abs(Yhat1[t]-decoded1) + abs(Yhat2[t]-decoded2);
    // else{
    //     m += (Yhat1[t]>=0) ? -decoded1 : decoded1;
    //     m += (Yhat2[t]>=0) ? -decoded2 : decoded2;
    // }

    if(decision == HARD) m = abs(x1[t]-decoded1) + abs(x2[t]-decoded2);
    else{
        m += (x1[t]>=0) ? -decoded1 : decoded1;
        m += (x2[t]>=0) ? -decoded2 : decoded2;
    }

    return m;
}

void decode(){
    cout << endl << "Decode~" << endl;
    q.resize(N+31);
    
    double metric[N+31][StateNum];            // metric so far
    int LastState[N+31][StateNum];            // 上一個state是誰~

    InitializeA();
    for (int t=0; t<N+31; ++t){
        for (int j=0; j<StateNum; ++j){
            if (t == 0){
                metric[t][j] = 10000;
                if(j==0) metric[t][j] =0;
            }
            else{
                // 存前一步跟最小metric
                // currentState = (s6 s5 ... s2 s1), 前一個state可能為(0 s6 ... s2)或(1 s6 ... s2)
                std::bitset<6> currentState(j);
                std::bitset<6> lastState0, lastState1;
                lastState0 = currentState;
                lastState0 >>= 1;
                lastState1 = lastState0;
                lastState1.set(5);

                double m0 = metric[t-1][lastState0.to_ulong()]+BranchMetric(a[lastState0.to_ulong()][currentState[0]], t-1);
                double m1 = metric[t-1][lastState1.to_ulong()]+BranchMetric(a[lastState1.to_ulong()][currentState[0]], t-1);

                // 選擇metric較小的
                if(m0 <= m1){
                    metric[t][j] = m0;
                    LastState[t][j] = lastState0.to_ulong();
                }
                else{
                    metric[t][j] = m1;
                    LastState[t][j] = lastState1.to_ulong();
                }
            }
        }   
    }

    double p = 100000;
    int current = 0;
    for (int j=0; j<StateNum; j++){
        if (metric[N+31-1][j] < p){
            p = metric[N+31-1][j];      // final length
            q[N+31-1] = j;              // the last state
            current = j;
        }
    }
    
    out1.push(a[q[N+31-1]][current%2]/10);
    out2.push(a[q[N+31-1]][current%2]%10);

    // find the state sequence
    for (int t=N+31-1; t>0; --t){
        q[t-1] = LastState[t][q[t]];
        current = q[t];
        out1.push(a[q[t-1]][current%2]/10);
        out2.push(a[q[t-1]][current%2]%10);
    }
    
    cout << "state: " << endl;
    for(int i=0; i<N+31; i++) cout << bitset<6>(q[i]) << " ";
    cout << endl;
    
    
    
    for(int i=0; i<out1.size();){
        std::cout << out1.top() << out2.top() << " ";
        out1.pop();
        out2.pop();
    }
}


int main(){
    // Initialize();
    InputGenerator();
    Encode();
    AWGN();
    decode();
}