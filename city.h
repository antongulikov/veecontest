//
// Created by scorpion on 01.03.16.
//

#pragma once

struct City {

    City(){
        d = nullptr;
        t = nullptr;
    }

    City(const City &other) {
        deleteM();
        n = other.n;
        allocateM();
    }

    City &operator = (const City &other) {
        if (this == &other) {
            return *this;
        }
        deleteM();
        n = other.n;
        allocateM();
    }

    City(int n):n(n) {
        allocateM();
    }

    void setTime(int u, int v, int tt) {
        t[u][v] = tt;
    }

    void setDist(int u, int v, int dd) {
        d[u][v] = dd;
    }

    int getTime(int u, int v) {
        return t[u][v];
    }

    int getDist(int u, int v) {
        return d[u][v];
    }

    void allocateM() {
        t = new int *[n + 1];
        d = new int *[n + 1];
        for (int i = 0; i <= n; i++) {
            t[i] = new int[n + 1]{};
            d[i] = new int[n + 1]{};
        }
    }

    void deleteM() {
        if (t) {
            for (int i = 0; i < n; i++)
                delete[] t[i];
            delete[] t;
        }
        if (d) {
            for (int i = 0; i < n; i++)
                delete[] d[i];
            delete[] d;
        }
        t = nullptr;
        d = nullptr;
    }


    ~City() {
        deleteM();
    }

//public:
    int n;
    int **d;
    int **t;
};