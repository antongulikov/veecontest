#include <iostream>

#include "stupidOracle.h"

int main() {
    srand(time(0));
    StupidOracle mainOracle(3);
    mainOracle.readData();
    //Oracle copyOracle(mainOracle);
    mainOracle.run();
    mainOracle.finish();
    //mainOracle.showStat();
    return 0;
}