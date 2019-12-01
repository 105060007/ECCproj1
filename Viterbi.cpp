// http://www.csie.ntnu.edu.tw/~u91029/HiddenMarkovModel.html
#include <bitset>
#include "vector"

std::vector<double> Yhat1, Yhat2;

const int N = 64;           // # of state, every state transits to 2 states
const int M = 4;            // output(y we guess)
const int T = 15;           // total length
// double π[N];                // probability = =
// b[][] -> output有M種，分別有不同機率
int a[N][2];                 // 現在這state吃0/1換到下一個state的output
double metric[T][N];            // metric so far
int LastState[T][N];            // 上一個state是誰~

void InitializeA(){
    for(int i=0; i<64; i++){
        // i = 4 = 000100(s6 s5 ... s2 s1)
        // input = 0  -> next = 001001 = 9
        std::bitset<6> x(i);
        a[i][0] = (0 + x[6-2] + x[6-3] + x[6-5] + x[6-6])%2*10
                     + (0 + x[6-1] + x[6-2] + x[6-3] + x[6-6])%2;
                     
        a[i][1] = (1 + x[6-2] + x[6-3] + x[6-5] + x[6-6])%2*10
                     + (1 + x[6-1] + x[6-2] + x[6-3] + x[6-6])%2;
    }
}

double BranchMetric(int output, int t){
    // 在地t個，output結果 vs encoded結果
    // Yhat1[t], Yhat2[t];
}


// o = output of AWGN channel
// q = state sequence~?

double decode(int* q){
    InitializeA();
    for (int t=0; t<T; ++t){
        for (int j=0; j<N; ++j){
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
    for (int j=0; j<N; ++j)
        if (metric[T-1][j] < p){
            p = metric[T-1][j];     // final length
            q[T-1] = j;
        }
            
 
    for (int t=T-1; t>0; --t)
        q[t-1] = LastState[t][q[t]];
 
    return p;
}