#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "simulation/ElementCommon.h"
#include <vector>
namespace SHIPS {
    extern std::vector<int> ships;
    extern std::array<int, 2> components;
    void cloneTMP(Simulation* sim, int i, int x, int y);
    bool isShip(int id);
    bool isComponent(int typ);
}

#endif