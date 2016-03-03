//
// Created by scorpion on 01.03.16.
//

#pragma once

#include <bits/stdc++.h>

using namespace std;

enum actions { move, pick1, pick2, drop1, drop2, end };

struct Vertex {
    int x, y;
    int id;
    Vertex(int x = 0, int y = 0) : x(x), y(y) {}

    int dist(const Vertex & other) const {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    void setId(int ide) {
        id = ide;
    }
};