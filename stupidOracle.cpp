//
// Created by scorpion on 02.03.16.
//

#include "stupidOracle.h"


void StupidOracle::solve() {
    solve1();
}


bool StupidOracle::canDriverGetOrder(int driverId, int orderId) {
    Driver &dr = drivers[driverId];
    Person &pr = persons[orderId];
    if (!dr.canAirport(aiportRand, pr.airport))
        return false;
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
        if (getMinimalTime(driverId, orderId) == -1)
            return false;
    }

    ///!!!!!
    // Не успеев вернуться в гараж к концу рабочего дня
    if (pr.queryTime + 20 * 60 + city.getTime(pr.to, dr.id_garage) + city.getTime(pr.from, pr.to) > dr.onWorkTime.second)
        return false;
    if (dr.onMove)
        return false;
    //!!! может еще что-то забыл?
    return true;
}

int StupidOracle::stickTime(int driverId, int orderId) {
    int finTime = finishTime(driverId, orderId);
    Person &pr = persons[orderId];
    Driver &dr = drivers[driverId];
    finTime -= dr.currentTime;
    int onTime = city.getTime(dr.currentCity, pr.from) + 20 * 60 + city.getTime(pr.from, pr.to);
    // finTime???
    return onTime * 60 + city.getDist(dr.currentCity, pr.from) + city.getDist(pr.from, pr.to);
}

void StupidOracle::assignOrder(int driverId, int orderId) {
    int st = persons[orderId].from;
    int fi = persons[orderId].to;
    if (! persons[orderId].toAirport) {
        moveDriver(driverId, st);
        putIn(driverId, orderId);
        moveDriver(driverId, fi);
        putOut(driverId, orderId);
    } else {
        Driver &dr = drivers[driverId];
        dr.currentTime = getMinimalTime(driverId, orderId);
        moveDriver(driverId, st);
        putIn(driverId, orderId);
        moveDriver(driverId, fi);
        putOut(driverId, orderId);
    }
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
                int value = stickTime(x.second, orderId);
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
                int value = stickTime(x, orderId);
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
    if (pr1.toAirport)
        return get2InTheCity(dId, fId, sId);
    if (pr1.from != pr2.from)
        return false;
    if (pr1.queryTime != pr2.queryTime)
        return false;
    int spendTime = city.getTime(dr.currentCity, pr1.from);
    int onR = 0;
        onR += 30 * 60 + city.getTime(pr1.to, pr2.to);
        onR += city.getTime(pr1.from, pr1.to) + city.getTime(pr2.to, dr.id_garage);
    int finishTime = pr1.queryTime + onR;
        if (dr.restTime < spendTime + onR)
            return false;
        if (finishTime > dr.onWorkTime.second)
            return false;
    if (city.getTime(pr2.from, pr2.to) + 40 * 60 < city.getTime(pr1.from, pr1.to) + city.getTime(pr1.to, pr2.to))
            return false;
    return true;
}

void StupidOracle::assignOrder(int driverId, int orderId, int secondId) {
    bool toAirport = persons[orderId].toAirport;
    int st = persons[orderId].from;
    int mid = persons[orderId].to;
    int fi = persons[secondId].to;
    Driver &dr = drivers[driverId];
    Person &pr1 = persons[orderId];
    Person &pr2 = persons[secondId];
    if (!toAirport) {
        moveDriver(driverId, st);
        putIn(driverId, orderId, secondId);
        moveDriver(driverId, mid);
        putOut(driverId, orderId);
        moveDriver(driverId, fi);
        putOut(driverId, secondId);
    } else {
        int mid = persons[secondId].from;
        int staT = getMinimalTime(driverId, orderId, secondId);
        dr.currentTime = staT;
        moveDriver(driverId, st);
        putIn(driverId, orderId);
        moveDriver(driverId, mid);
        putIn(driverId, secondId);
        moveDriver(driverId, fi);
        putOut(driverId, orderId, secondId);
    }
}


int StupidOracle::stickTime(int dr1, int fst, int sec, bool toAir) {
    Driver &dr = drivers[dr1];
    Person &pr1 = persons[fst];
    Person &pr2 = persons[sec];
        int finTime = finishTime(dr1, fst, sec);
        int onTime = finTime - 2 * dr.currentTime + getMinimalTime(dr1, fst, sec);
        int di = city.getDist(dr.currentCity, pr1.from) + city.getDist(pr1.from, pr1.to) + city.getDist(pr1.to, pr2.to);
        return onTime * 60 + di;
}

bool StupidOracle::get2InTheCity(int driderId, int fstId, int secId) {
    //return false;
    if (!canDriverGetOrder(driderId, fstId))
        return false;
    if (!canDriverGetOrder(driderId, secId))
        return false;
    Driver &dr = drivers[driderId];
    Person &pr1 = persons[fstId];
    Person &pr2 = persons[secId];
    if (pr1.to != pr2.to)
        return false;
    return getMinimalTime(driderId, fstId, secId) > -1;
}


void StupidOracle::solve1() {
    int howMuch = 1 + rand() % 20;
    aiportRand = rand() % 30 + 20;
    for (int i = 0; i < qSize; i++)
        notDone.insert(i);
    int mult = rand() % 2;
    WaitTime = 60 * mult;
    long long flag2 = rand() % 2;
    int border = rand() % 5 + 3;
    set<pair<long long, int> > currentFree;
    vector<int> values(driverSize, 0);
    set<int> wannaBeHired;
    for (int i = 0; i < driverSize; i++)
        wannaBeHired.insert(i);
    for (int q = 0; q < flights.size(); q++) {
        vector <Person> currentFlight;
        currentFlight.clear();
        for (int w = 0; q + w < flights.size() && w < 1; w++) {
            int u = q + w;
            for (int i = 0; i < flights[u].size(); i++)
                if (notDone.find(flights[u][i].id) != notDone.end()) {
                    currentFlight.push_back(flights[u][i]);
                }
        }
        random_shuffle(currentFlight.begin(), currentFlight.end());
        int n = (int) currentFlight.size();
        vector<bool> ok(n, false);
        for (int i = 0; i < n; i++) {
            if (ok[i])
                continue;
            int fst = currentFlight[i].id;
            int pi = i;
            int pj = -1;
            int h1 = persons[fst].home;
            int snd = -1;
            for (int j = 0; j < n; j++)if (i != j) {
                    if (ok[j])
                        continue;
                    if (snd == -1 or city.getDist(h1, persons[snd].home) >
                                     city.getDist(h1, currentFlight[j].id)) {
                        snd = currentFlight[j].id;
                        pj = j;
                    }
                }
            if (snd == -1)
                continue;
            int bestStick = (int) 1e9;
            int pos = -1;
            for (auto x : currentFree)
                if (canGet2Order(x.second, fst, snd)) {
                    int value = stickTime(x.second, fst, snd, persons[fst].toAirport);
                    if (bestStick > value) {
                        bestStick = value;
                        pos = x.second;
                    }
                }
            if (pos != -1) {
                currentFree.erase({values[pos], pos});
                ok[pi] = true;
                ok[pj] = true;
                values[pos] += rand() % 10;
                currentFree.insert({values[pos], pos});
                assignOrder(pos, fst, snd);
                notDone.erase(fst);
                notDone.erase(snd);
                continue;
            }
            vector<pair<int, int> > may_be_new;
            may_be_new.clear();
            for (auto x : wannaBeHired) {
                if (canGet2Order(x, fst, snd)) {
                    int value = stickTime(x, fst, snd, persons[snd].toAirport);
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
            assignOrder(pos, fst, snd);
            notDone.erase(fst);
            notDone.erase(snd);
            ok[pi] = ok[pj] = true;
        }
        vector < Person> tmpVector;
        tmpVector.clear();
        for (int i = 0; i < currentFlight.size(); i++) {
            if (!ok[i])
                tmpVector.push_back(currentFlight[i]);
        }
        currentFlight = tmpVector;
        for (int i = 0; i < currentFlight.size(); i++) {
            int orderId = currentFlight[i].id;
            int bestStick = (int) 1e9;
            int pos = -1;
            for (auto x : currentFree)
                if (canDriverGetOrder(x.second, orderId)) {
                    int value = stickTime(x.second, orderId);
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
                    int value = stickTime(x, orderId);
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

void StupidOracle::doHarlemShake(const vector<int> &dri) {
    return;
}
