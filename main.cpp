#include <iostream>

#include "stupidOracle.h"

int main() {
    srand(time(0));
    ios_base :: sync_with_stdio(false);
    cin.tie(0);
    StupidOracle mainOracle(29.5);
    mainOracle.readData();
    //Oracle copyOracle(mainOracle);
    mainOracle.run();
    mainOracle.finish();
    //mainOracle.showStat();
    return 0;
}