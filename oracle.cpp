//
// Created by scorpion on 02.03.16.
//

#include "oracle.h"




void Oracle::moveDriver(int driverId, int cityId) {
    Driver &dr = drivers[driverId];
    // Уже у нуженм городе
    if (dr.currentCity == cityId)
        return;
    actionBuilder.setAction(actions::move);
    actionBuilder.setTime(dr.currentTime);
    actionBuilder.setVertex(cityId);
    int duringTime = city.getTime(dr.currentCity, cityId);
    dr.restTime -= duringTime;
    dr.currentDistance += city.getDist(dr.currentCity, cityId);
    dr.currentTime += city.getTime(dr.currentCity, cityId);
    dr.setCity(cityId);
    dr.onMove = true;
    dr.inAiport = false;
    outputer.addActionToDriver(driverId, actionBuilder.getAction());
}

void Oracle::putIn(int driverId, int personId) {
    Driver &dr = drivers[driverId];
    Person &pr = persons[personId];
    actionBuilder.setAction(actions::pick1);
    int actionTime = dr.currentTime;
    if (!pr.toAirport) {
        actionTime = pr.queryTime;
    }
    actionBuilder.setTime(actionTime);
    actionBuilder.setFirst(pr.id);
    //cout << drivers[driverId].currentTime << endl;
    dr.currentTime = actionTime + 10 * 60;
    dr.onMove = false;
    //cout << actionTime << " " << drivers[driverId].currentTime << endl;
    //exit(0);
    dr.passengers.push_back(pr);
    outputer.addActionToDriver(driverId, actionBuilder.getAction());
}


void Oracle::putOut(int driverId, int personId) {
    Driver &dr = drivers[driverId];
    Person &pr = persons[personId];
    actionBuilder.setAction(actions::drop1);
    actionBuilder.setTime(dr.currentTime);
    actionBuilder.setFirst(pr.id);
    if (pr.toAirport)
        dr.inAiport = true;
    dr.currentTime += 10 * 60;
    for (int i = 0; i < dr.passengers.size(); i++)
        if (dr.passengers[i].id == personId) {
            dr.passengers.erase(dr.passengers.begin() + i);
            break;
        }
    dr.onMove = false;
    outputer.addActionToDriver(driverId, actionBuilder.getAction());
}

void Oracle::putOut(int driverId, int personId, int personId2) {
    Driver &dr = drivers[driverId];
    Person &pr = persons[personId];
    Person &pr2 = persons[personId2];
    if (pr.toAirport)
        dr.inAiport = true;
    actionBuilder.setAction(actions::drop2);
    actionBuilder.setTime(dr.currentTime);
    actionBuilder.setFirst(pr.id);
    actionBuilder.setSecond(pr2.id);
    dr.currentTime += 10 * 60;
    assert(dr.passengers.size() == 2);
    dr.passengers.pop_back();
    dr.passengers.pop_back();
    dr.onMove = false;
    outputer.addActionToDriver(driverId, actionBuilder.getAction());
}


void Oracle::home(int driverId) {
    Driver &dr = drivers[driverId];
    if (dr.currentTime > dr.onWorkTime.second) {
        cerr << dr.currentTime << " " << dr.currentDistance << " " << dr.onWorkTime.second << endl;
    }
    assert(dr.currentTime <= dr.onWorkTime.second);
    actionBuilder.setAction(actions::end);
    outputer.addActionToDriver(driverId, actionBuilder.getAction());
}

void Oracle::copyElements() {
    copyAirports.clear();
    for (int i = 0; i < airports.size(); i++)
        copyAirports.push_back(airports[i]);
    copyDrivers.clear();
    for (int i = 0; i < drivers.size(); i++) {
        copyDrivers.push_back(drivers[i]);
    }
    copyPersons.clear();
    for (int i = 0; i < persons.size(); i++)
        copyPersons.push_back(persons[i]);
    copyGarages.clear();
    copyHoueses.clear();
    for (int i = 0; i < houses.size(); i++)
        copyHoueses.push_back(houses[i]);
    for (int i = 0; i < garages.size(); i++)
        copyGarages.push_back(garages[i]);
}

void Oracle::reverseCopy() {
    airports.clear();
    drivers.clear();
    persons.clear();
    houses.clear();
    garages.clear();
    for (int i = 0; i < copyAirports.size(); i++)
        airports.push_back(copyAirports[i]);
    for (int i = 0; i < copyDrivers.size(); i++)
        drivers.push_back(copyDrivers[i]);
    for (int i = 0; i < copyPersons.size(); i++)
        persons.push_back(copyPersons[i]);
    for (int i = 0; i < copyGarages.size(); i++)
        garages.push_back(copyGarages[i]);
    for (int i = 0; i < copyHoueses.size(); i++)
        houses.push_back(copyHoueses[i]);
}

double Oracle::calcScore() {
    long long ret = 0;
    for (int i = 0; i < drivers.size(); i++)
        if (drivers[i].currentTime != drivers[i].onWorkTime.first) {
            ret += 300000 + drivers[i].currentDistance;
        }
    long long x = 0;
    long long y = 0;
    for (int i = 0; i < persons.size(); i++) {
        x += city.getDist(persons[i].from, persons[i].to);
        y += city.getTime(persons[i].from, persons[i].to);
    }
    long long z = y / 32400;
    while (y > z * 32400)
        z++;
    long long s = x + z * 300000;
    ret += 1500000 * 1ll * notDone.size();
    return (double) 1000. * s / (ret + 0.);
}

void Oracle::preprocess() {
    //srand(rand());
}


void Oracle ::run() {
    bestScore = -1;
    int cnt = 0;
    makeFlights();
    while (true) {
        if ((double)(clock() - startTime) > 3 * CLOCKS_PER_SEC)
            break;
        reverseCopy();
        preprocess();
        solve();
        double getScore = calcScore();
        if (getScore > bestScore) {
            bestScore = getScore;
            bestOutput = outputer;
        }
        //break;
        cnt++;
        outputer.clear();
    }
    vector <int> driversPosition;
    pair < vector <int> , int> tmpp = getDriversPosition(bestOutput);
    driversPosition = tmpp.first;
    int pos1 = tmpp.second;
    //for (int i = 0; i < driversPosition.size(); i++)
      //  driversPosition[i] = i;
    const int STEPS_PER_TEMP = 10;
    const double COOLING_FRACTION = 0.99;

    int i1, i2;
    double temperature;
    double startVal;
    double delta, merit, flip, index;

    temperature = 1.0;

    doHarlemShake(driversPosition);

    curVal = -calcScore(); //initialize

    int n = (int)drivers.size();
    while (true) {
        if ((double)(clock() - startTime) > workTime * CLOCKS_PER_SEC)
            break;
        temperature *= COOLING_FRACTION;
        startVal = curVal;

        for (int j = 0; j < STEPS_PER_TEMP; j++) {
            cnt++;
            i1 = rand() % n;
            i2 = rand() % n;

            flip = (rand() % 1000000) * 1.0 / 999999.0;
            delta = change(driversPosition, i1, i2);
            index = (-delta / curVal) / temperature;
            merit = 1.0 / (1.0 + exp(index));

            //minimize energy
            if (delta < 0) //good
                curVal += delta;
            else {
                if (merit > flip) //not so good
                    curVal += delta;
                else {
                    change(driversPosition, i2, i1);
                }
            }

        }
        if (curVal - startVal < 0.0) //succeed
            temperature = temperature / COOLING_FRACTION;
    }
    cerr << cnt << endl;
    cerr << bestScore << endl;
    outputer = bestOutput;
}

void Oracle::showStat() {
    vector < int > inAirport(airports.size(), 0);
    for (int i = 0; i < persons.size(); i++)
        inAirport[min(persons[i].from, persons[i].to)]++;
    cerr << "Aiports:\n";
    sort(persons.begin(), persons.end());
    for (int i = 0; i  < inAirport.size(); i++) {
        cerr << i << " " << inAirport[i] << endl;
    }
    int cnt1 = 1;
    for (int i = 1; i < persons.size(); i++) {
        if (persons[i].queryTime != persons[i - 1].queryTime) {
            cerr << persons[i].toAirport << " " << cnt1 << " " << persons[i].queryTime << endl;
            cnt1 = 0;
        }
        cnt1++;
    }
    copyElements();
}

void Oracle::putIn(int driverId, int personId, int personId2) {
    Driver &dr = drivers[driverId];
    Person &pr = persons[personId];
    Person &pr2 = persons[personId2];
    actionBuilder.setAction(actions::pick2);
    int actionTime = dr.currentTime;
    if (!pr.toAirport) {
        actionTime = pr.queryTime;
    }
    actionBuilder.setTime(actionTime);
    actionBuilder.setFirst(pr.id);
    actionBuilder.setSecond(pr2.id);
    //cout << drivers[driverId].currentTime << endl;
    dr.currentTime = actionTime + 10 * 60;
    dr.onMove = false;
    //cout << actionTime << " " << drivers[driverId].currentTime << endl;
    //exit(0);
    dr.passengers.push_back(pr);
    dr.passengers.push_back(pr2);
    outputer.addActionToDriver(driverId, actionBuilder.getAction());
}

void Oracle::makeFlights() {
    flights.clear();
    sort(persons.begin(), persons.end());
    vector < Person > flight;
    flight.push_back(persons[0]);
    for (int i = 1; i < persons.size(); i++) {
        if (persons[i].queryTime == persons[i - 1].queryTime && (persons[i].to == persons[i - 1].to && persons[i].toAirport ||
                                                                 !persons[i].toAirport && persons[i].from == persons[i - 1].from)) {
            flight.push_back(persons[i]);
        } else {
            flights.push_back(flight);
            flight.clear();
        }
    }
    if (flight.size() > 0) {
        flights.push_back(flight);
    }
    reverseCopy();
}//
int Oracle::finishTime(int driverId, int orderId) {
    Person &pr = persons[orderId];
    Driver &dr = drivers[driverId];
    int finTime = 0;
    if (!pr.toAirport) {
        finTime = pr.queryTime + city.getTime(pr.from, pr.to) + 20 * 60;
    } else {
        finTime = getMinimalTime(driverId, orderId);
        finTime += city.getTime(dr.currentCity, pr.from) + city.getTime(pr.from, pr.to) + 20 * 60;
    }
    return finTime;
}


int Oracle::getMinimalTime(int dId, int fstId) {
    Driver &dr = drivers[dId];
    Person &pr1 = persons[fstId];
    if (!pr1.toAirport)
        return pr1.queryTime;
    int timfa = city.getTime(pr1.from, pr1.to);
    int tof = city.getTime(dr.currentCity, pr1.from);
    int left = pr1.queryTime - tof - 80 * 60 - timfa;
    left = max(left, dr.currentTime);
    int right = pr1.queryTime - tof - 30 * 60 - timfa;
    /*for (int x = dr.currentTime; x <= pr1.queryTime - timfa; x += 10) {
        int in1 = x + tof;
        if (pr1.queryTime - in1 > 60 * 60 + timfa + 20 * 60)
            continue;
        int in2 = 10 * 60 + in1 + timfs;
        if (pr2.queryTime - in2 > 60 * 60 + 20 * 60 + timsa)
            continue;
        int fin = in2 + timsa + 20 * 60;
        if (fin > pr1.queryTime || fin > pr2.queryTime)
            continue;
        return x;
    }*/
    if (left > right)
        return -1;
    return left;
}

int Oracle::getMinimalTime(int dId, int fstId, int secId) {
    Driver &dr = drivers[dId];
    Person &pr1 = persons[fstId];
    Person &pr2 = persons[secId];
    if (!pr1.toAirport)
        return pr1.queryTime;
    if (pr1.to != pr2.to)
        return -1;
    int timfa = city.getTime(pr1.from, pr1.to);
    int timsa = city.getTime(pr2.from, pr2.to);
    int timfs = city.getTime(pr1.from, pr2.from);
    int tof = city.getTime(dr.currentCity, pr1.from);
    int left = pr1.queryTime - tof - 80 * 60 - timfa;
    left = max(left, dr.currentTime);
    left = max(left, pr2.queryTime - tof - 10 * 60 - timfs - 80 * 60 - timsa);
    int right = pr1.queryTime - timfs - tof - 30 * 60 - timsa;
    right = min(right, pr2.queryTime - timfs - tof - 30 * 60 - timsa);
    /*for (int x = dr.currentTime; x <= pr1.queryTime - timfa; x += 10) {
        int in1 = x + tof;
        if (pr1.queryTime - in1 > 60 * 60 + timfa + 20 * 60)
            continue;
        int in2 = 10 * 60 + in1 + timfs;
        if (pr2.queryTime - in2 > 60 * 60 + 20 * 60 + timsa)
            continue;
        int fin = in2 + timsa + 20 * 60;
        if (fin > pr1.queryTime || fin > pr2.queryTime)
            continue;
        return x;
    }*/
    if (left > right)
        return -1;
    return left;
}

int Oracle::finishTime(int driverId, int personId, int secId) {
    Person &pr1 = persons[personId];
    Person &pr2 = persons[secId];
    Driver &dr = drivers[driverId];
    int finishTime = 0;
    if (pr1.toAirport) {
        finishTime = getMinimalTime(driverId, personId, secId);
        finishTime += 30 * 60;
        finishTime += city.getTime(dr.currentCity, pr1.from) + city.getTime(pr1.from, pr2.from) + city.getTime(pr2.from, pr2.to);
    } else {
        finishTime = getMinimalTime(driverId, personId, secId);
        finishTime += 30 * 60;
        finishTime += city.getTime(pr1.from, pr1.to) + city.getTime(pr1.to, pr2.to);
    }
    return finishTime;
}

double Oracle::change(vector<int> &a, int p1, int p2) {
    int delta = min((int)a.size() - max(p1, p2) - 1, 10);
    delta = max(delta, 1);
    delta = 1;
    for (int i = 0; i < delta; i++)
        swap(a[p1 + i], a[p2 + i]);
    reverseCopy();
    preprocess();
    doHarlemShake(a);
    double tt = calcScore();
    if (tt > bestScore) {
        bestOutput = outputer;
        bestScore = tt;
    }
    outputer.clear();
    return -tt - curVal;
}

pair < vector<int>, int> Oracle::getDriversPosition(const Outputer &out) {
    vector <int> result;
    vector <Driver> fir;
    vector <Driver> sec;
    result.clear();
    for (int i = 0; i < out.ret.size(); i++) {
        if (out.ret[i].size() > 2)
            fir.push_back(drivers[i]);
        else
            sec.push_back(drivers[i]);
    }
    sort(fir.begin(), fir.end());
    sort(sec.begin(), sec.end());
    for (int i = 0; i < fir.size(); i++)
        result.push_back(fir[i].did);
    for (int i = 0; i < sec.size(); i++)
        result.push_back(sec[i].did);
    return {result, fir.size()};
}
