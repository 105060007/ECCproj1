// http://www.csie.ntnu.edu.tw/~u91029/HiddenMarkovModel.html
#include <bitset>
#include "vector"
#include "./Viterbi.h"
#include <iostream>
#include <stack>
using namespace std;

// std::vector<double> Yhat0, Yhat1;
// double* Yhat0;
// double* Yhat1;

const int StateNum = 64;    // # of state, every state transits to 2 states
int T = 15;                 // total length
int a[StateNum][2];         // 現在這state吃0/1換到下一個state的output
vector<int> q;

void InitializeA(){
    for(int i=0; i<64; i++){
        // i = 4 = 000100(s6 s5 ... s2 s1)
        // input = 1  -> next = 001001 = 9
        std::bitset<6> x(i);

        a[i][0] = (0 + x[6-2] + x[6-3] + x[6-5] + x[6-6])%2*10
                     + (0 + x[6-1] + x[6-2] + x[6-3] + x[6-6])%2;
                     
        a[i][1] = (1 + x[6-2] + x[6-3] + x[6-5] + x[6-6])%2*10
                     + (1 + x[6-1] + x[6-2] + x[6-3] + x[6-6])%2;
    }
}

double BranchMetric(int output, int t){
    // 在第t個，output結果 vs AWGN encoded結果
    double m=0;
    double decoded1, decoded2;
    decoded1 = output / 10;
    decoded2 = output % 10;

    // m = ((Yhat0[t]-decoded0)^2 + (Yhat1[t]-decoded1)^2);
    m = -decoded1 -decoded2;
    return m;
}

// void decode( std::vector< double> Y0, std::vector< double> Y1, int TotalLength ){
void decode(int TotalLength ){
    cout << endl << "Decode~" << endl;
    // Yhat0 = Y0;
    // Yhat1 = Y1;
    T = TotalLength;
    q.resize(T);
    
    double metric[T][StateNum];            // metric so far
    int LastState[T][StateNum];            // 上一個state是誰~

    InitializeA();
    for (int t=0; t<T; ++t){
        for (int j=0; j<StateNum; ++j){
            if (t == 0) metric[t][j] = 0;
            else{
                // 存前一步跟最小metric
                std::bitset<6> currentState(j);
                std::bitset<6> lastState0, lastState1;
                lastState0 = currentState;
                lastState0 >>= 1;
                lastState1 = lastState0;
                lastState1.set(5);

                double m0 = metric[t-1][j]+BranchMetric(a[lastState0.to_ulong()][currentState[0]], t);
                double m1 = metric[t-1][j]+BranchMetric(a[lastState1.to_ulong()][currentState[0]], t);

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
        if (metric[T-1][j] < p){
            p = metric[T-1][j];     // final length
            q[T-1] = j;
            current = j;
        }
    }

    q[T-1] = 35;
    current = 35;

    
    std::stack<int> out1;
    std::stack<int> out2;
    out1.push(a[q[T-1]][current%2]/10);
    out2.push(a[q[T-1]][current%2]%10);
 
    for (int t=T-1; t>0; --t){
        q[t-1] = LastState[t][q[t]];
        current = q[t];
        out1.push(a[q[t-1]][current%2]/10);
        out2.push(a[q[t-1]][current%2]%10);
    }
    
    cout << "state: " << endl;
    for(int i=0; i<T; i++) cout << bitset<6>(q[i]) << " ";
    cout << endl;

    for(int i=0; i<out1.size();){
        std::cout << out1.top() << out2.top() << " ";
        out1.pop();
        out2.pop();
    }
}