#include "template.h"
#include "models.h"
#include <unistd.h>

vector<string> parseFlags(int argc, char **argv) {
    vector<string> rv;
    for(int i = 0; i < argc; i++) {
        rv.pb(string(argv[i]));
    }
    return rv;
}

vector<pair<int, vector<Street*>>> prop(Sim G) {
    for(auto car: G.cars) {
        for(auto p: car->path) {
            G.streetMap[p]->score++;
        }
    }
    vector<pair<int, vector<Street*>>> res;
    for(auto nodeStreets: G.graph) {
        vector<Street*> streets;
        for(auto p: nodeStreets) {
            if (p->score) {
                streets.pb(p);
            }
        }
        if(!streets.empty()) {            
            sort(streets.begin(), streets.end(), [](auto a, auto b) {
                return (a->score > b->score);
            });
            for(int i = 0; i < streets.size(); i++) {
                streets[i]->score = (streets.size()-i)%10;
            }
            res.pb({streets[0]->end, streets});
        }
    }
    return res;
}

vector<pair<int, vector<Street*>>> variance(Sim G) {
    for(auto car: G.cars) {
        int time = 0;
        for(auto p: car->path) {
            G.streetMap[p]->times.insert(time);
            time += G.streetMap[p]->length;
        }
    }
    vector<pair<int, vector<Street*>>> res;
    for(auto nodeStreets: G.graph) {
        vector<Street*> streets;
        for(auto p: nodeStreets) {
            if (p->times.size() > 0) {
                double mean = 0, variance = 0, total = 0;
                mean += accumulate(all(p->times), 0);
                mean /= p->times.size();
                for(int tim: p->times) {
                    variance += (tim - mean) * (tim - mean);
                }
                variance /= p->times.size();
                p->score = variance;
                streets.pb(p);
            }
        }
        if(!streets.empty()) {
            sort(streets.begin(), streets.end(), [](auto a, auto b) {
                return (a->score < b->score);
            });
            res.pb({streets[0]->end, streets});
        }
    }
    return res;
}

Sim input(string filename) {
    ifstream fin;
    fin.open("data/"+filename);
    Sim G;
    fin >> G.D >> G.I >> G.S >> G.V >> G.F;
    G.graph = vector<vector<Street*>>(G.I);
    for(int i = 0; i < G.S; i++) {
        auto street = new Street();
        fin >> street->begin >> street->end >> street->name >> street->length;
        G.graph[street->end].emplace_back(street);
        G.streetMap[street->name] = street;
    }
    for(int i = 0; i < G.V; i++) {
        auto car = new Car();
        int p;
        fin >> p;
        for(int j = 0; j < p; j++)  {
            string street;
            fin >> street;
            car->path.pb(street);
        }
        G.cars.pb(car);
    }
    return G;
}

void output(vector<pair<int, vector<Street*>>> res) {
    cout << res.size() << endl;
    for(auto streets: res) {
        cout << streets.first << endl;
        cout << streets.second.size() << endl;
        for(auto street: streets.second) {
            cout << street->name << ' ' << (int)1 << endl;
        }
    }
}

int main(int argc, char **argv) {
    auto flags = parseFlags(argc, argv);
    auto G = input(flags[2]);
    auto res = prop(G);
    output(res);
    return 0;
}
