//
// Created by scorpion on 01.03.16.
//

#pragma once

#include "vertex.h"
#include "person.h"


struct Driver {

    int id_garage;
    pair<int, int> onWorkTime;
    vector<Person> passengers;

    // driver id
    int did;
    int restTime;
    int currentTime;

    int currentDistance;
    int currentCity;
    bool onMove;
    int lastAirport;
    int nAirports;

    Driver(int id = 0, int did = 0, int nA = 0) : id_garage(id), did(did), nAirports(nA) {
        currentDistance = 0;
        inAiport = false;
        currentCity = id;
        restTime = 9 * 60 * 60;
        onMove = false;
        lastAirport = -1;
    }

    void setAirport(int airport) {
        lastAirport = airport;
    }

    void setCity(int newCity) {
        currentCity = newCity;
        if (currentCity < nAirports)
            lastAirport = currentCity;
    }

    bool canAirport(int pr, int neA) {
        if (lastAirport == -1)
            return true;
        if (neA == lastAirport) {
            return true;
        }
        return true;
    }

    void set_time(int start = 0, int finish = 0) {
        onWorkTime = make_pair(start, finish);
        currentTime = start;
    }

    bool inAiport;


};