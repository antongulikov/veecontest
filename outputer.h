//
// Created by scorpion on 02.03.16.
//

#pragma once

#include "action.h"

struct Outputer {

    Outputer(){}

    Outputer(const Outputer &other) {
        ret.clear();
        numberOfDrivers = other.numberOfDrivers;
        seed = other.seed;
        ret.resize(numberOfDrivers);
    }

    Outputer&operator = (const Outputer &other) {
        if (this == &other)
            return (*this);
        ret.clear();
        ret = other.ret;
        seed = other.seed;
        numberOfDrivers = other.numberOfDrivers;
    }

    Outputer(int n, int seed = 0) : numberOfDrivers(n), seed(seed){
        ret.clear();
        ret.resize(n);
    }

    void addActionToDriver(int driver, string action) {
        ret[driver].push_back(action);
    }

    void printAns() {
        cout << seed << endl;
        for (int i = 0; i < numberOfDrivers; i++) {
            cout << "driver " << i << ": " << (ret[i].size() == 0 ? "no" : "yes") << "\n";
            for (int j = 0; j < ret[i].size(); j++)
                cout << ret[i][j] << "\n";
            if (ret[i].size() > 0)
                assert(ret[i].back() == "end");
        }
    }

    void clear() {
        for (int i = 0; i < numberOfDrivers; i++)
            ret[i].clear();
    }

    ~Outputer() {
        for (int i = 0; i < numberOfDrivers; i++)
            ret[i].clear();
        ret.clear();
    }

    int seed;
    int numberOfDrivers;
    vector <vector <string> > ret;
};