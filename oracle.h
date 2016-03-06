//
// Created by scorpion on 01.03.16.
//

#pragma once

#include "city.h"
#include "person.h"
#include "driver.h"
#include "action.h"
#include "vertex.h"
#include "outputer.h"


struct Oracle {

    double workTime;

    Oracle(double debug = 5) {
        workTime = debug;
        startTime = clock();
        drivers.clear();
        persons.clear();
        airports.clear();
        garages.clear();
        houses.clear();
    }

    Oracle(Oracle &other) : seed(other.seed), aSize(other.aSize), qSize(other.qSize), dSize(other.dSize),
                            driverSize(other.driverSize) {
        drivers = other.drivers;
        persons = other.persons;
        airports = other.airports;
        garages = other.garages;
        houses = other.houses;
        city = other.city;
        outputer = other.outputer;
        N = other.N;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                city.setTime(i, j, other.city.getTime(i, j));
                city.setDist(i, j, other.city.getDist(i, j));
            }
    }
    Test test;

    void readSeed() {
        cin >> seed;
        test.generate (seed);
    }

    int abc;

    void readSizes() {
        abc = 0;
        aSize = test.a;
        dSize = test.g;
        qSize = test.q;
        driverSize = test.d;
    }

    Vertex readVertex() {
        int x, y;
        x = test.point[abc].x;
        y = test.point[abc].y;
        abc++;
        return Vertex(x, y);
    }

    void readData() {
        readSeed();
        readSizes();
        N = dSize + aSize + qSize;
        outputer = Outputer(driverSize, seed);
        for (int i = 0; i < aSize; i++) {
            Vertex vv = readVertex();
            vv.setId(i);
            airports.push_back(vv);
        }
        for (int i = aSize; i < aSize + dSize; i++) {
            Vertex vv = readVertex();
            vv.setId(i);
            garages.push_back(vv);
        }
        for (int i = N - qSize; i < N; i++) {
            Vertex vv = readVertex();
            vv.setId(i);
            houses.push_back(vv);
        }
        abc = 0;
        for (int i = 0; i < qSize; i++) {
            int fr, to, mo;
            fr = test.query[i].from;
            to = test.query[i].to;
            mo = test.query[i].moment.value;
            persons.push_back(Person(fr, to, mo, i));
        }
        for (int i = 0; i < driverSize; i++) {
            int gar = 0, start = 0, fin = 0;
            gar = test.driver[i].garage;
            start = test.driver[i].momentStart.value;
            fin = test.driver[i].momentFinish.value;
            Driver dr = Driver(gar, i, aSize);
            dr.set_time(start, fin);
            drivers.push_back(dr);
        }
        city = City(N);

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                city.setDist(i, j, test.gdist[i][j]);
            }
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                city.setTime(i, j, test.gtime[i][j].value);
            }
        }
        copyElements();
    }

    void finish() {
        outputer.printAns();
    }

    void showStat();

    void copyElements();

    void reverseCopy();

    void preprocess();

    vector<Driver> drivers, copyDrivers;
    vector<Person> persons, copyPersons;
    vector<Vertex> airports, copyAirports;
    vector<Vertex> garages, copyGarages;
    vector<Vertex> houses, copyHoueses;
    set<int> notDone;
    City city;
    Outputer outputer;
    int seed;
    int dSize, aSize, qSize, driverSize, N;
    vector<vector<Person> > flights;

    virtual ~Oracle() { }

    Action actionBuilder;

    double calcScore();

    void run();

    void moveDriver(int driverId, int cityid);

    void putIn(int driverId, int personId);

    void putOut(int driverId, int personId);

    void putIn(int driverId, int personId, int personId2);

    void putOut(int driverId, int personId, int personId2);

    void home(int driverId);

    int WaitTime;
    double startTime;

    virtual bool canDriverGetOrder(int driverId, int orderId) = 0;

    virtual void solve() = 0;

    void makeFlights();

    int finishTime(int driverId, int personId);
};