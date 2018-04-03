#include "quadnode.hpp"
#include <iostream.h>

void quadnode::copy (quadnode &source)
{
        cout << " Beginning copy \n";
    xul = source.xul;
    yul = source.yul;
    xur = source.xur;
    yur = source.yur;
    xll = source.xll;
    yll = source.yll;
    xlr = source.xlr;
    ylr = source.ylr;
    rows = source.rows;
    cols = source.cols;
    L1energy = source.L1energy;
    L2energy = source.L2energy;
    max = source.max;
    min = source.min;
    epsilon = source.epsilon;
    isnotnull = source.isnotnull;
    id = source.id;
    NUMBER_OF_GREY_LEVELS = source.NUMBER_OF_GREY_LEVELS;
    cout << " Copy of " << source.id << " complete \n";
}