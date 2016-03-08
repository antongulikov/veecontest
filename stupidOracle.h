//
// Created by scorpion on 02.03.16.
//

#include "oracle.h"

struct StupidOracle : public Oracle {
    StupidOracle(double debug = 5) : Oracle(debug) { }

    void solve();

    void clearSolve();

    bool canDriverGetOrder(int driverId, int orderId);

    void assignOrder(int driverId, int orderId);

    void assignOrder(int driverId, int orderId, int secondId);

    void doHarlemShake(const vector <int> &dri);

    int stickTime(int driverId, int orderId);

    int stickTime(int dr, int fst, int sec, bool toA);

    virtual ~StupidOracle() { }

    bool flag;

    bool canGet2Order(int dId, int fId, int sId);

    int aiportRand;

    bool get2InTheCity(int driderId, int fstId, int secId);

    void solve1();
};