#include "simulation/ElementCommon.h"
#include "simulation/Spaceship.h"
#include <algorithm>
#include <vector>

namespace SHIPS {
	std::vector<int> ships;
	std::array<int, 2> components{ PT_COTR, PT_HULL };

	bool isComponent(int typ) {
		for (unsigned int i = 0; i < sizeof(components) / sizeof(components[0]); ++i)
			if (components[i] == typ) return true;
		return false;
	}

	bool isShip(int id) {
		for (auto itr = ships.begin(); itr != ships.end(); ++itr) {
			if (*itr == id) return true;
		}
		return false;
	}

	void cloneTMP(Simulation* sim, int i, int x, int y) {
		int r, rx, ry;
		for (rx = -1; rx <= 1; rx++)
			for (ry = -1; ry <= 1; ry++)
				if (BOUNDS_CHECK && (rx || ry)) {
					r = sim->pmap[y + ry][x + rx];
					if (!r || !isShip(sim->parts[ID(r)].tmp) || !isShip(ID(r))) continue;
					if (isComponent(TYP(sim->parts[ID(r)].tmp)) || isComponent(TYP(r))) {
						sim->parts[i].tmp = sim->parts[ID(r)].tmp;
						if (TYP(r) == PT_COTR)
							sim->parts[i].tmp = ID(r);
						return;
					}
				}
	}
}