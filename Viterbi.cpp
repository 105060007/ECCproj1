

    // q[0] = 0;
    // int current;
    // double p = 10000;
    // bitset<6> now(0);
    // queue<bool> out1, out2;

    // for(int t=1; t<N+31; t++){
    //     bitset<6> next0, next1;
    //     next0 = now;
    //     next0 <<= 1;
    //     next1 = next0;
    //     next1.set(0);

    //     if(metric[t][next0.to_ulong()] <= metric[t][next1.to_ulong()]){
    //         q[t] = next0.to_ulong();
    //         out1.push(a[q[t]-1][0]/10);
    //         out2.push(a[q[t]-1][0]%10);
    //     } 
    //     else{
    //         q[t] = next1.to_ulong();
    //         out1.push(a[q[t]-1][1]/10);
    //         out2.push(a[q[t]-1][1]%10);
    //     } 

    //     // if(t==4) cout << "0->" << metric[t][next0.to_ulong()] << "  1->" << metric[t][next1.to_ulong()];

    //     now = q[t];
    // }

    // cout << "state: " << endl;
    // for(int i=0; i<N+31; i++) cout << bitset<6>(q[i]) << " ";
    // cout << endl;

    // for(int i=0; i<out1.size();){
    //     std::cout << out1.front() << out2.front() << " ";
    //     out1.pop();
    //     out2.pop();
    // }
    // // std::cout << out1.front() << out2.front() << " ";


// http://www.csie.ntnu.edu.tw/~u91029/HiddenMarkovModel.html
#include <bitset>
#include "vector"
#include "./Viterbi.h"
#include <math.h>
#include <iostream>
#include <stack>
using namespace std;

vector<double> Y1, Y2;
// double* Yhat0;
// double* Y1;

const int StateNum = 64;    // # of state, every state transits to 2 states
int T = 15;                 // total length
int a[StateNum][2];         // 現在這state吃0/1換到下一個state的output
vector<int> q;
int Dec = 0;

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

    // hard or soft
    if(Dec == 0) m = abs(Y1[t]-decoded1) + abs(Y2[t]-decoded2);
    else{
        m += (Y1[t]>=0) ? -decoded1 : decoded1;
        m += (Y2[t]>=0) ? -decoded2 : decoded2;
    }
    return m;
}

void decode( std::vector< double> Yhat1, std::vector< double> Yhat2, int TotalLength, int d ){
// void decode(int TotalLength ){
    cout << endl << "Decode~" << endl;
    Y1 = Y1;
    Y2 = Y2;
    T = TotalLength;
    Dec = d;
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