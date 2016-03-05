#include <iostream>

#include "stupidOracle.h"

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