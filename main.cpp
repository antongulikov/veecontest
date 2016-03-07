#include <iostream>

#include "stupidOracle.h"

int main() {
    srand(2);
    StupidOracle mainOracle(3);
    mainOracle.readData();
    //Oracle copyOracle(mainOracle);
    mainOracle.run();
    mainOracle.finish();
    //mainOracle.showStat();
    return 0;
}