//
// Created by scorpion on 06.03.16.
//

#pragma once

struct LCG // for portability: 48-bit random numbers engine used in Java
{
    static const int64_t LCG_MULT = 0x5DEECE66DLL;
    static const int64_t LCG_ADD  =         0xBLL;
    static const int64_t LCG_MASK = (1LL << 48) - 1;

    int64_t state;

    void init (int64_t seed)
    {
        state = (seed ^ LCG_MULT) & LCG_MASK;
    }

    int next ()
    {
        state = ((state * LCG_MULT) + LCG_ADD) & LCG_MASK;
        return (int) (state >> 17);
    }

    int uniform (int range) // [0..range)
    {
        if (range <= 0)
        {
            assert (false);
        }
        if ((range & -range) == range)
        {
            return (int) ((range * 1LL * next ()) >> 31);
        }
        int limit = (0x7FFFFFFF / range) * range;
        int temp;
        do
        {
            temp = next ();
        }
        while (temp >= limit);
        return temp % range;
    }

    int uniform (int lo, int hi) // [lo..hi]
    {
        return lo + uniform (hi - lo + 1);
    }
};

struct Time
{
    int value;

    Time ()
    {
    }

    Time (int newValue)
    {
        value = newValue;
    }

    Time (const Time & other)
    {
        value = other.value;
    }

    Time operator + (const Time other) const
    {
        return Time (value + other.value);
    }
};

bool operator < (const Time a, const Time b)
{
    return a.value < b.value;
}

Time seconds (int value)
{
    return Time (value);
}

Time minutes (int value)
{
    return Time (value * 60);
}

Time hours (int value)
{
    return Time (value * 60 * 60);
}

struct Point
{
    int x;
    int y;

    Point ()
    {
    }

    Point (int newX, int newY)
    {
        x = newX;
        y = newY;
    }
};

bool operator < (const Point a, const Point b)
{
    if (a.x != b.x)
    {
        return a.x < b.x;
    }
    return a.y < b.y;
}

int64_t distSquared (const Point & p, const Point & q)
{
    return (q.x - p.x) * 1LL * (q.x - p.x) +
           (q.y - p.y) * 1LL * (q.y - p.y);
}

struct Query
{
    int from;
    int to;
    Time moment;

    Query ()
    {
    }

    Query (int newFrom, int newTo, Time newMoment)
    {
        from = newFrom;
        to = newTo;
        moment = newMoment;
    }
};

struct Driver2
{
    int garage;
    Time momentStart;
    Time momentFinish;

    Driver2 ()
    {
    }

    Driver2 (int newGarage, Time newMomentStart, Time newMomentFinish)
    {
        garage = newGarage;
        momentStart = newMomentStart;
        momentFinish = newMomentFinish;
    }
};

struct Test
{
    static const Point origin;

    LCG rnd;
    unsigned seed;
    int r, a, g, q, n, dm, d;

    vector <Point> point;
    vector <Query> query;
    vector <Driver2> driver;
    vector <vector <int> > gdist;
    vector <vector <Time> > gtime;

    Point uniformFreePoint (set <Point> & visited, int r)
    {
        Point res;
        do
        {
            res.x = rnd.uniform (-r, +r);
            res.y = rnd.uniform (-r, +r);
        }
        while (distSquared (origin, res) > r * 1LL * r ||
               visited.find (res) != visited.end ());
        return res;
    }

    void addFarPoints (set <Point> & visited, int num)
    {
        int startPos = point.size ();
        for (int i = 0; i < num; i++)
        {
            vector <Point> temp;
            vector <int64_t> minDistSquared;
            int64_t maxDistSquared = -1;
            for (int j = 0; j <= i; j++)
            {
                Point curPoint = uniformFreePoint (visited, r);
                int64_t curDistSquared = r * 9LL * r;
                for (int k = 0; k < i; k++)
                {
                    curDistSquared = min (curDistSquared,
                                          distSquared (point[startPos + k],
                                                       curPoint));
                }
                maxDistSquared = max (maxDistSquared,
                                      curDistSquared);
                temp.push_back (curPoint);
                minDistSquared.push_back (curDistSquared);
            }

            int j = 0;
            while (maxDistSquared != minDistSquared[j])
            {
                j++;
                if (j > i)
                {
                    assert (false);
                }
            }
            point.push_back (temp[j]);
            visited.insert (temp[j]);
        }
    }

    Point skewFreePoint (set <Point> & visited, int r)
    {
        Point res;
        do
        {
            Point pre;
            do
            {
                pre.x = rnd.uniform (-r, +r);
                pre.y = rnd.uniform (-r, +r);
            }
            while (distSquared (origin, pre) > r * 1LL * r);

            static const int p = 1000003;
            int s = rnd.uniform (1, p);
            res = Point ((int)
                                 (round (pre.x * 1.0 * s / p)), (int)
                                 (round (pre.y * 1.0 * s / p)));
        }
        while (visited.find (res) != visited.end ());
        return res;
    }

    void generatePoints ()
    {
        point.clear ();
        point.reserve (n);
        set <Point> visited;

        addFarPoints (visited, a);

        addFarPoints (visited, g);

        for (int i = 0; i < q; i++)
        {
            Point curPoint = skewFreePoint (visited, r);
            point.push_back (curPoint);
            visited.insert (curPoint);
        }
    }

    void generateQueries ()
    {
        query.clear ();
        query.reserve (q);
        set <Time> times [a];
        int curNumber = a + g;
        while ((int) query.size () < q)
        {
            int t = rnd.uniform (0, 1);
            int atemp = rnd.uniform (1, a);
            int b = rnd.uniform (0, atemp - 1);
            Time m;
            do
            {
                m = minutes (rnd.uniform (3 * 60, 21 * 60));
            }
            while (times[b].find (m) != times[b].end ());
            times[b].insert (m);
            int w = rnd.uniform (1, 20);
            int c;
            do
            {
                c = rnd.uniform (1, w);
            }
            while ((int) query.size () + c > q);
            for (int i = 0; i < c; i++)
            {
                query.push_back (Query
                                         ((t == 0) ? curNumber : b,
                                          (t == 0) ? b : curNumber, m));
                curNumber++;
            }
        }
    }

    void generateDrivers ()
    {
        driver.clear ();
        driver.reserve (d);
        for (int i = 0; i < d; i++)
        {
            int gtemp = rnd.uniform (1, g);
            int h = rnd.uniform (0, gtemp - 1);
            Time m = hours (rnd.uniform (0, 12));
            driver.push_back (Driver2 (h + a, m, m + hours (12)));
        }
    }

    void generateMatrices ()
    {
        gdist = vector <vector <int> > (n, vector <int> (n));
        gtime = vector <vector <Time> > (n, vector <Time> (n));
        for (int u = 0; u < n; u++)
        {
            for (int v = 0; v < n; v++)
            {
                int e = (int) (ceil (sqrt (distSquared
                                                   (point[u], point[v]) * 1.0)));
                int t = (int) (ceil (e * 60.0 / 1000));
                gdist[u][v] = rnd.uniform (e, 2 * e);
                gtime[u][v] = seconds (rnd.uniform (t, 2 * t));
            }
        }
    }

    void generate (unsigned newSeed)
    {
        seed = newSeed;
        rnd.init (seed);

        r = rnd.uniform (10000, 50000);

        do
        {
            a = rnd.uniform (1, 5);
            g = rnd.uniform (1, 10);
            q = rnd.uniform (500, 2000);
            n = a + g + q;
        }
        while (n > 2000);

        dm = (int) (ceil (r * 1.0 * q / 100000));
        d = rnd.uniform (dm, 2 * dm);

        generatePoints ();
        generateQueries ();
        generateDrivers ();
        generateMatrices ();
    }

    void write (FILE * fout = stdout)
    {
    }
};

const Point Test::origin = Point (0, 0);
