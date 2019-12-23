#include "simulation/ElementCommon.h"
#include "simulation/Spaceship.h"
#include <algorithm>
#include <vector>

namespace SHIPS {
	void cloneTMP(Simulation* sim, int i, int x, int y) {
		int r, rx, ry;
		for (rx = -1; rx <= 1; rx++)
			for (ry = -1; ry <= 1; ry++)
				if (BOUNDS_CHECK && (rx || ry)) {
					r = sim->pmap[y + ry][x + rx];
					if (!r) continue;
					for (unsigned int it = 0; it < 1; ++i)
						if (components[it] == ID(r)) {
							sim->parts[i].tmp = sim->parts[ID(r)].tmp;
							return;
						}
				}
	}
}