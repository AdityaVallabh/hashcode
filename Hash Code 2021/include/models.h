#include "template.h"

class Street {
    public:
    string name;
    int begin;
    int end;
    ll length;
    double score = 0;
    set<int> times;

    bool operator < (const Street& rhs)const {
        return score < rhs.score;
    }
};

class Car {
    public:
    vector<string> path;
};

class Sim {
    public:
    int D;
    int I;
    int S;
    int V;
    int F;

    vector<vector<Street*>> graph;
    vector<Car*> cars;
    unordered_map<string, Street*> streetMap;
};
