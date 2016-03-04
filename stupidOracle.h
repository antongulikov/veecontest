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
    int StickTime(int driverId, int orderId);
    int StickTime(int dr, int fst, int sec);
    virtual ~StupidOracle(){}
    bool flag;
    bool canGet2Order(int dId, int fId, int sId);
};