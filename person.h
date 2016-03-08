//
// Created by scorpion on 01.03.16.
//

#pragma once

struct Person {
    bool toAirport;
    int from, to;
    int queryTime;
    int actualTime;
    int id;
    int home;
    int airport;

    Person(int from = 0, int to = 0, int tim = 0, int id = 0) : from(from), to(to), queryTime(tim), id(id) {
        toAirport = to < from;
        home = (from > to) ? from : to;
        airport = to + from - home;
        actualTime = queryTime;
        if (toAirport)
            actualTime -= 60 * 60 * 1;
    }

    bool operator<(const Person &other) const {
        return (actualTime < other.actualTime || actualTime == other.actualTime && to < other.to ||
                actualTime == other.actualTime && to == other.to && id < other.id);
    }
};