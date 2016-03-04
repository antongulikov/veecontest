//
// Created by scorpion on 01.03.16.
//

#pragma once

struct Person {
    bool toAirport;
    int from, to;
    int queryTime;
    int id;
    int home;
    Person(int from = 0, int to = 0, int tim = 0, int id = 0) : from(from), to(to), queryTime(tim), id(id) {
        toAirport = to < from;
        home = (from > to) ? from : to;
    }

    bool operator < (const Person &other) const {
        return (queryTime < other.queryTime || queryTime == other.queryTime && to < other.to ||
                queryTime == other.queryTime && to == other.to && id < other.id);
    }
};