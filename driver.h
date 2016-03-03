//
// Created by scorpion on 01.03.16.
//

#pragma once

#include "vertex.h"
#include "person.h"


struct Driver {

    int id_garage;
    pair <int, int> onWorkTime;
    vector < Person > passengers;

    // driver id
    int did;
    int restTime;
    int currentTime;

    int currentDistance;
    int currentCity;
    bool onMove;

    Driver(int id = 0, int did = 0) : id_garage(id), did(did) {
        currentDistance = 0;
        currentCity = id;
        restTime = 9 * 60 * 60;
        onMove = false;
    }

    void set_time(int start = 0, int finish = 0) {
        onWorkTime = make_pair(start, finish);
        currentTime = start;
    }

};