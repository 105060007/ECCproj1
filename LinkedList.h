// C++ code
#include <iostream>
// #include <bitset>
#include <string>
using namespace std;

// class LinkedList;

class Node{
private:
    string state;
    // std::bitset<6> state;   // which state
    double metric;          // the distance of the shortest path up to now
    Node *next0;
    Node *next1;
    Node *last;
public:
    Node(string a, double m){
        state = a;
        metric = m;
        next0 = 0;
        next1 = 0;
    };

    // friend class LinkedList;
};