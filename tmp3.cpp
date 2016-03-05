#include <bits/stdc++.h> 

using namespace std;//
// Created by scorpion on 01.03.16.
//



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
};//
// Created by scorpion on 01.03.16.
//


struct City {

    City(){
        d = nullptr;
        t = nullptr;
    }

    City(const City &other) {
        deleteM();
        n = other.n;
        allocateM();
    }

    City &operator = (const City &other) {
        if (this == &other) {
            return *this;
        }
        deleteM();
        n = other.n;
        allocateM();
    }

    City(int n):n(n) {
        allocateM();
    }

    void setTime(int u, int v, int tt) {
        t[u][v] = tt;
    }

    void setDist(int u, int v, int dd) {
        d[u][v] = dd;
    }

    int getTime(int u, int v) {
        return t[u][v];
    }

    int getDist(int u, int v) {
        return d[u][v];
    }

    void allocateM() {
        t = new int *[n + 1];
        d = new int *[n + 1];
        for (int i = 0; i <= n; i++) {
            t[i] = new int[n + 1]{};
            d[i] = new int[n + 1]{};
        }
    }

    void deleteM() {
        if (t) {
            for (int i = 0; i < n; i++)
                delete[] t[i];
            delete[] t;
        }
        if (d) {
            for (int i = 0; i < n; i++)
                delete[] d[i];
            delete[] d;
        }
        t = nullptr;
        d = nullptr;
    }


    ~City() {
        deleteM();
    }

//public:
    int n;
    int **d;
    int **t;
};//
// Created by scorpion on 01.03.16.
//



struct Action {
public:

    Action& getInstance() {
        static Action instance;
        return instance;
    }

    void setAction(actions type) {
        actionType = type;
    }

    void setTime(int time) {
        actionTime = time;
    }

    void setVertex(int v) {
        actionVertex = v;
    }

    void setFirst(int first) {
        firstP = first;
    }

    void setSecond(int second) {
        secondP = second;
    }

    string getAction() {
        string result = actionRepr[actionType];
        if (actionType != actions::end)
            result += " " + to_str(actionTime);
        switch (actionType) {
            case actions::move: {
                result += " " + to_str(actionVertex);
                break;
            }
            case actions:: pick1: {
                result += " " + to_str(firstP);
                break;
            }
            case actions:: drop1: {
                result += " " + to_str(firstP);
                break;
            }
            case actions:: pick2: {
                result += " " + to_str(firstP);
                result += " " + to_str(secondP);
                break;
            }
            case actions:: drop2: {
                result += " " + to_str(firstP);
                result += " " + to_str(secondP);
                break;
            }
        }
        return result;
    }


public:
    Action(){}
    string actionRepr[6] = {"move", "pick1", "pick2", "drop1", "drop2", "end"};
    string to_str(int value) {
        string ret = "";
        if (value == 0)
            return "0";
        while (value > 0) {
            ret.push_back('0' + (value % 10));
            value /= 10;
        }
        reverse(ret.begin(), ret.end());
        return ret;
    }
    actions actionType;
    int actionTime, actionVertex, firstP, secondP;
};//
// Created by scorpion on 01.03.16.
//


struct Person {
    bool toAirport;
    int from, to;
    int queryTime;
    int id;
    int home;
    int airport;
    Person(int from = 0, int to = 0, int tim = 0, int id = 0) : from(from), to(to), queryTime(tim), id(id) {
        toAirport = to < from;
        home = (from > to) ? from : to;
        airport = to + from - home;
    }

    bool operator < (const Person &other) const {
        return (queryTime < other.queryTime || queryTime == other.queryTime && to < other.to ||
                queryTime == other.queryTime && to == other.to && id < other.id);
    }
};//
// Created by scorpion on 02.03.16.
//



struct Outputer {

    Outputer(){}

    Outputer(const Outputer &other) {
        ret.clear();
        numberOfDrivers = other.numberOfDrivers;
        seed = other.seed;
        ret.resize(numberOfDrivers);
    }

    Outputer&operator = (const Outputer &other) {
        if (this == &other)
            return (*this);
        ret.clear();
        ret = other.ret;
        seed = other.seed;
        numberOfDrivers = other.numberOfDrivers;
    }

    Outputer(int n, int seed = 0) : numberOfDrivers(n), seed(seed){
        ret.clear();
        ret.resize(n);
    }

    void addActionToDriver(int driver, string action) {
        ret[driver].push_back(action);
    }

    void printAns() {
        cout << seed << endl;
        for (int i = 0; i < numberOfDrivers; i++) {
            cout << "driver " << i << ": " << (ret[i].size() == 0 ? "no" : "yes") << "\n";
            for (int j = 0; j < ret[i].size(); j++)
                cout << ret[i][j] << "\n";
            if (ret[i].size() > 0)
                assert(ret[i].back() == "end");
        }
    }

    void clear() {
        for (int i = 0; i < numberOfDrivers; i++)
            ret[i].clear();
    }

    ~Outputer() {
        for (int i = 0; i < numberOfDrivers; i++)
            ret[i].clear();
        ret.clear();
    }

    int seed;
    int numberOfDrivers;
    vector <vector <string> > ret;
};//
// Created by scorpion on 01.03.16.
//




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
        return (rand() % 10 <= pr);
    }

    void set_time(int start = 0, int finish = 0) {
        onWorkTime = make_pair(start, finish);
        currentTime = start;
    }

    bool inAiport;


};//
// Created by scorpion on 01.03.16.
//



struct Oracle {

    double workTime;

    Oracle(double debug = 5){
        workTime = debug;
        startTime = clock();
        drivers.clear();
        persons.clear();
        airports.clear();
        garages.clear();
        houses.clear();
    }

    Oracle(Oracle &other) : seed(other.seed), aSize(other.aSize), qSize(other.qSize), dSize(other.dSize), driverSize(other.driverSize){
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

    void readSeed(){
        cin >> seed;
    }

    void readSizes(){
        cin >> aSize >> dSize >> qSize >> driverSize;
    }

    Vertex readVertex() {
        int x, y;
        cin >> x >> y;
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
        for (int i = 0; i < qSize; i++) {
            int fr, to, mo;
            cin >> fr >> to >> mo;
            persons.push_back(Person(fr, to, mo, i));
        }
        for (int i = 0; i < driverSize; i++) {
            int gar = 0, start = 0, fin = 0;
            cin >> gar >> start >> fin;
            Driver dr = Driver(gar, i, aSize);
            dr.set_time(start, fin);
            drivers.push_back(dr);
        }
        city = City(N);

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int x;
                cin >> x;
                city.setDist(i, j, x);
            }
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int x;
                cin >> x;
                city.setTime(i, j, x);
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

    vector < Driver > drivers,copyDrivers;
    vector < Person > persons, copyPersons;
    vector < Vertex > airports, copyAirports;
    vector < Vertex > garages, copyGarages;
    vector < Vertex > houses, copyHoueses;
    set <int> notDone;
    City city;
    Outputer outputer;
    int seed;
    int dSize, aSize, qSize, driverSize, N;
    vector < vector < Person> > flights;
    virtual ~Oracle() {}
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
};//
// Created by scorpion on 02.03.16.
//



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
}//
// Created by scorpion on 02.03.16.
//


struct StupidOracle : public Oracle {
    StupidOracle(double debug = 5) : Oracle(debug) { }
    void solve();
    void clearSolve();
    bool canDriverGetOrder(int driverId, int orderId);
    void assignOrder(int driverId, int orderId);
    void assignOrder(int driverId, int orderId, int secondId);
    int StickTime(int driverId, int orderId);
    int StickTime(int dr, int fst, int sec);
    virtual ~StupidOracle(){}
    bool flag;
    bool canGet2Order(int dId, int fId, int sId);
    int aiportRand;
};//
// Created by scorpion on 02.03.16.
//



void StupidOracle::solve() {
    flag = rand() % 2;
    aiportRand = rand() % 3 + 5;
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
    for (int q = 0; q < flights.size(); q++) {
        vector <Person> currentFlight = flights[q];
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
                    int value = StickTime(x.second, fst, snd);
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
                    int value = StickTime(x, fst, snd);
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
    if (pr.toAirport && dr.inAiport || !pr.toAirport && !dr.inAiport)
        dr.did = dr.did;
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
    if (pr1.toAirport)
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
    int st = persons[orderId].from;
    int mid = persons[orderId].to;
    int fi = persons[secondId].to;
    moveDriver(driverId, st);
    putIn(driverId, orderId, secondId);
    moveDriver(driverId, mid);
    putOut(driverId, orderId);
    moveDriver(driverId, fi);
    putOut(driverId, secondId);
}

int StupidOracle::StickTime(int dr1, int fst, int sec) {
    Driver &dr = drivers[dr1];
    Person &pr1 = persons[fst];
    Person &pr2 = persons[sec];
    int finTime = pr1.queryTime + city.getTime(pr1.from, pr1.to) + city.getTime(pr1.to, pr2.to);
    finTime -= dr.currentTime;
    int di = city.getDist(dr.currentCity, pr1.from) + city.getDist(pr1.from, pr1.to) + city.getDist(pr1.to, pr2.to);
    return finTime * 60 + di;
}


int main() {
    srand(time(0));
    ios_base :: sync_with_stdio(false);
    cin.tie(0);
    StupidOracle mainOracle;
    mainOracle.readData();
    //Oracle copyOracle(mainOracle);
    mainOracle.run();
    mainOracle.finish();
    //mainOracle.showStat();
    return 0;
}
