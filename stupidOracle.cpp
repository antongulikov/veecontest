//
// Created by scorpion on 02.03.16.
//

#include "stupidOracle.h"

StupidOracle::StupidOracle() { }

void StupidOracle::solve() {
    flag = rand() % 2;
    for (int i = 0; i < qSize; i++)
         notDone.insert(i);
    int mult = rand() % 20 + 80;
     WaitTime = 60 * mult;
        long long flag2 = rand() % 2;
        int border = rand() % 5 + 3;
        set<pair<long long, int> > currentFree;
        vector<int> values(driverSize, 0);
        set<int> wannaBeHired;
        for (int i = 0; i < driverSize; i++)
            wannaBeHired.insert(i);
        vector<pair<long long, int> > orders;
        orders.clear();
    mult = rand() % 4 + 1;
    for (auto i : notDone)
        orders.push_back({(persons[i].queryTime - persons[i].toAirport * ((20 * 60 + 1.5 * city.getTime(persons[i].from, persons[i].to)) * flag2 +
                                                                          (1 - flag2) * WaitTime)) * 1ll * 1000000 * mult + 3 * city.getDist(persons[i].from, persons[i].to), persons[i].id});
    sort(orders.begin(), orders.end());
        for (int i = 0; i < orders.size(); i++) {
            int orderId = orders[i].second;
            int bestStick = (int) 1e9;
            int pos = -1;
            for (auto x : currentFree)
                if (canDriverGetOrder(x.second, orderId)) {
                    int value = StickTime(x.second, orderId);
                    if (bestStick > value) {
                        bestStick = value;
                        pos = x.second;
                    }
                }
            if (pos != -1) {
                currentFree.erase({values[pos], pos});
                values[pos] += rand() % 10;
                currentFree.insert({values[pos], pos});
                assignOrder(pos, orderId);
                notDone.erase(orderId);
                continue;
            }
            vector<pair<int, int> > may_be_new;
            may_be_new.clear();
            for (auto x : wannaBeHired) {
                if (canDriverGetOrder(x, orderId)) {
                    int value = StickTime(x, orderId);
                    may_be_new.push_back({value, x});
                }
            }
            sort(may_be_new.begin(), may_be_new.end());
            if (may_be_new.size() > 0) {
                int value = may_be_new[0].first;
                int x = may_be_new[0].second;
                if (value < bestStick) {
                    bestStick = value;
                    pos = x;
                }
            }
            if (pos == -1) {
                continue;
            }
            for (int w = 1; w < min(border, (int) may_be_new.size()); w++) {
                wannaBeHired.erase(may_be_new[w].second);
                currentFree.insert({0, may_be_new[w].second});
            }
            wannaBeHired.erase(pos);
            values[pos] += rand() % 10;
            currentFree.insert({values[pos], pos});
            assignOrder(pos, orderId);
            notDone.erase(orderId);
        }
    clearSolve();

    for (auto x: currentFree) {
        Driver &dr = drivers[x.second];
        if (dr.currentCity == dr.id_garage)
            continue;
        moveDriver(x.second, dr.id_garage);
        home(x.second);
    }
    // Go home!
}


bool StupidOracle::canDriverGetOrder(int driverId, int orderId) {
    Driver &dr = drivers[driverId];
    Person &pr = persons[orderId];
    if (dr.onWorkTime.first > pr.queryTime)
        return false;
    if (dr.onWorkTime.second < pr.queryTime)
        return false;
    if (dr.passengers.size() > 0)
        return false;
    double spendTime = city.getTime(dr.currentCity, pr.from);
    spendTime += city.getTime(pr.from, pr.to) + 20 * 60 + city.getTime(pr.to, dr.id_garage);
    // Не успеем отвезти и вернуться домой
    if (dr.restTime < spendTime)
        return false;
    // Не успеем доехать к началу запроса
    if (!pr.toAirport)
    if (dr.currentTime + city.getTime(dr.currentCity, pr.from) > pr.queryTime)
        return false;
    // Не успею довезти парня в аэропорт
    if (pr.toAirport) {
        int ti = dr.currentTime + city.getTime(dr.currentCity, pr.from) + 20 * 60 + city.getTime(pr.from, pr.to);
        if (ti > pr.queryTime)
            return false;
        if (ti < pr.queryTime - 60 * 60 && (flag == 0))
            return false;
    }
    // Не успеев вернуться в гараж к концу рабочего дня
    if (pr.queryTime + 20 * 60 + city.getTime(pr.to, dr.id_garage) + city.getTime(pr.from, pr.to) > dr.onWorkTime.second)
        return false;
    if (dr.onMove)
        return false;
    //!!! может еще что-то забыл?
    return true;
}

int StupidOracle::StickTime(int driverId, int orderId) {
    Person &pr = persons[orderId];
    Driver &dr = drivers[driverId];
    int finTime = 0;
    if (!pr.toAirport) {
        finTime = pr.queryTime + city.getTime(pr.from, pr.to) + 20 * 60;
    } else {
        finTime = pr.queryTime;
    }
    finTime -= dr.currentTime;
    int onTime = city.getTime(dr.currentCity, pr.from) + 20 * 60 + city.getTime(pr.from, pr.to);
    return finTime * 60 + city.getDist(dr.currentCity, pr.from) + city.getDist(pr.from, pr.to);
}

void StupidOracle::assignOrder(int driverId, int orderId) {
    int st = persons[orderId].from;
    int fi = persons[orderId].to;
    moveDriver(driverId, st);
    if (persons[orderId].toAirport && flag) {
        Driver &dr = drivers[driverId];
        dr.currentTime = max(dr.currentTime, persons[orderId].queryTime - 60 * 60 - 20 * 60 - city.getTime(st, fi));
    }
    putIn(driverId, orderId);
    moveDriver(driverId, fi);
    putOut(driverId, orderId);
}

void StupidOracle::clearSolve() {
    int mult = rand() % 20 + 80;
    WaitTime = 60 * mult;
    long long flag2 = rand() % 2;
    int border = rand() % 5 + 3;
    set <pair <long long, int> > currentFree;
    vector <int> values(driverSize, 0);
    set <int> wannaBeHired;
    for (int i = 0; i < driverSize; i++)
        wannaBeHired.insert(i);
    for (int i = 0; i < driverSize; i++) {
        if (drivers[i].currentDistance > 0) {
            currentFree.insert({drivers[i].currentTime, drivers[i].did});
            values[drivers[i].did] = drivers[i].currentTime;
            wannaBeHired.erase(drivers[i].did);
        }
    }
    vector < pair < long long, int> > orders;
    orders.clear();
    mult = rand() % 4 + 1;
    for (auto i : notDone)
        orders.push_back({(persons[i].queryTime - persons[i].toAirport * ((20 * 60 + 1.5 * city.getTime(persons[i].from, persons[i].to)) * flag2 +
                                                                          (1 - flag2) * WaitTime)) * 1ll * 1000000 * mult + 3 * city.getDist(persons[i].from, persons[i].to), persons[i].id});
    sort(orders.begin(), orders.end());
    for (int i = 0; i < orders.size(); i++) {
        int orderId = orders[i].second;
        int bestStick = (int)1e9;
        int pos = -1;
        for (auto x : currentFree)
            if (canDriverGetOrder(x.second, orderId)) {
                int value = StickTime(x.second, orderId);
                if (bestStick > value) {
                    bestStick = value;
                    pos = x.second;
                }
            }
        if (pos != -1) {
            currentFree.erase({values[pos], pos});
            values[pos] += rand() % 10;
            currentFree.insert({values[pos], pos});
            assignOrder(pos, orderId);
            notDone.erase(orderId);
            continue;
        }
        vector < pair <int, int > > may_be_new;
        may_be_new.clear();
        for (auto x : wannaBeHired) {
            if (canDriverGetOrder(x, orderId)) {
                int value = StickTime(x, orderId);
                may_be_new.push_back({value, x});
            }
        }
        sort(may_be_new.begin(), may_be_new.end());
        if (may_be_new.size() > 0) {
            int value = may_be_new[0].first;
            int x = may_be_new[0].second;
            if (value < bestStick) {
                bestStick = value;
                pos = x;
            }
        }
        if (pos == -1) {
            continue;
        }
        for (int w = 1; w < min(border, (int)may_be_new.size()); w++) {
            wannaBeHired.erase(may_be_new[w].second);
            currentFree.insert({0, may_be_new[w].second});
        }
        wannaBeHired.erase(pos);
        values[pos] += rand() % 10;
        currentFree.insert({values[pos], pos});
        assignOrder(pos, orderId);
        notDone.erase(orderId);
    }

    for (auto x: currentFree) {
        Driver &dr = drivers[x.second];
        moveDriver(x.second, dr.id_garage);
        home(x.second);
    }
}

bool StupidOracle::canGet2Order(int dId, int fId, int sId) {
    if (!canDriverGetOrder(dId, fId))
        return false;
    if (!canDriverGetOrder(dId, sId))
        return false;
    Driver &dr = drivers[dId];
    Person &pr1 = persons[fId];
    Person &pr2 = persons[sId];
}
