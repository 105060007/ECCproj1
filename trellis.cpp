#include <iostream>
#include <queue>
#include <bitset>

using namespace std;

struct Trellis 
{ 
    queue<bool> out1, out2;
    queue<bitset<6> > lastState;
    queue<bitset<6> > currState;
    double metric = 0;
};