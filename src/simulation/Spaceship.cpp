#include "simulation/ElementCommon.h"
#include "simulation/Spaceship.h"
#include <algorithm>
#include <unordered_map>
#include <vector>

/**
Properties:
	pavg[0] - Storing ship id in componenets connected to a COTR
*/

namespace SHIPS {
	
	std::unordered_map<int, std::vector<int>> ships;
	std::array<int, NUMSHIPCOMPONENTS> components{ PT_COTR, PT_HULL, PT_THRS };

	void createShip(int id) {
		ships[id] =  {};
	}

	bool isComponent(int typ) {
		for (unsigned int i = 0; i < NUMSHIPCOMPONENTS; ++i)
			if (components[i] == typ) return true;
		return false;
	}

	bool isShip(int id) {
		return ships.count(id) > 0;
	}

	void clonePAVG(Simulation* sim, int i, int x, int y) {
		int r, rx, ry;
		for (rx = -1; rx <= 1; rx++)
			for (ry = -1; ry <= 1; ry++)
				if (BOUNDS_CHECK && (rx || ry)) {
					r = sim->pmap[y + ry][x + rx];
					if (!r || sim->parts[ID(r)].pavg[0] < 0) continue;
					if (isShip(sim->parts[ID(r)].pavg[0]) || isShip(ID(r))) {
						if (isComponent(sim->parts[ID(r)].type) || isComponent(TYP(r))) {
							sim->parts[i].pavg[0] = sim->parts[ID(r)].pavg[0];
							if (TYP(r) == PT_COTR)
								sim->parts[i].pavg[0] = ID(r);
							return;
						}
					}

				}
	}
}