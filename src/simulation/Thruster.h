#include "simulation/ElementCommon.h"
#include <vector>
#include <unordered_map>

#ifndef THRUSTER_H
#define THRUSTER_H

namespace THRUSTERS {
	std::vector<std::pair<int, int>> direction(Simulation* sim, int i, int x, int y);
	void calculateThrust(Simulation* sim, int i);
	std::vector<std::pair<int, int>> direction(Simulation* sim, int i, int x, int y);
	extern std::unordered_map<int, std::pair<float, float>> thrusters;
}
#endif
