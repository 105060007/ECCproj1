#include <vector>
#include <queue>
#include <bitset>
using namespace std;

// 6 Q
// out1 out2 info last/now state  metric


vector<bool> x1, x2;
queue<bool> q;          // info bits~?
queue<double> metric;   // best metric now
queue<vector<bool> > s;    // survivor

double BranchMetric(int output, int t){

}

void Decode(){
    vector<bool> initialState(6, false);  // (s1 s2 .. s5 s6)
    metric.push(0);
    s.push(initialState);

    for(int t=0; t<32; t++){
        vector<bool> currentState = s.back();
        // if 0 => output = ?
        int y1 = currentState[6-2]+currentState[6-3]+currentState[6-5]+currentState[6-6];
        int y2 = currentState[6-1]+currentState[6-2]+currentState[6-3]+currentState[6-6];
        int out0 = (0+y1)%2 *10 + (0+y2)%2;
        int out1 = (1+y1)%2 *10 + (1+y2)%2;
        
        // if(BranchMetric(out0, t))
    }
}