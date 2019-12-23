#include "simulation/ElementCommon.h"
#include <vector>

namespace SHIPS {
    std::vector<int> ships;
    std::array<int, 2> components{ PT_COTR, PT_HULL };
    void cloneTMP(Simulation* sim, int i, int x, int y);
}