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
    dr.currentCity = cityId;
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
    srand(rand());
}


void Oracle ::run() {
    double bestScore = -1;
    int cnt = 0;
    Outputer bestOutput;
    makeFlights();
    while (true) {
        double now = clock();
        if ((now - startTime) / CLOCKS_PER_SEC > workTime)
            break;
        reverseCopy();
        preprocess();
        solve();
        double getScore = calcScore();
        if (getScore > bestScore) {
            bestScore = getScore;
            bestOutput = outputer;
        }
        cnt++;
        outputer.clear();
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
}