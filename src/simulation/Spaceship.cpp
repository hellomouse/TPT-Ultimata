#include "simulation/ElementCommon.h"
#include "simulation/Spaceship.h"
#include "simulation/Thruster.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <math.h>

/**
Properties:
	pavg[0] - Storing ship id in componenets connected to a COTR
*/

namespace SHIPS {
	
	std::unordered_map<int, std::vector<int>> ships;
	std::array<int, NUMSHIPCOMPONENTS> components{ PT_COTR, PT_NAVI, PT_LASR, PT_HULL, PT_THRS, PT_RADR, PT_FILT };
	std::unordered_map<int, std::pair<float, float>> acceleration;

	void createShip(int id) {
		ships[id] =  {};
		acceleration[id] = { 0, 0 };
	}

	void calculateAcceleration(Simulation* sim, int id) {
		for (auto component : ships[id]) {
			if (sim->parts[component].type != PT_THRS) continue; // We only need to find thrusters
			// Add the acceleration calculated by each thruster to the acceleration pair of the body
			if (std::fabs(THRUSTERS::thrusters[component].first) < (THRUSTERS::thrusterAcceleration / 8))
				THRUSTERS::thrusters[component].first = 0;
			if (std::fabs(THRUSTERS::thrusters[component].second) < (THRUSTERS::thrusterAcceleration / 8))
				THRUSTERS::thrusters[component].second = 0;
			acceleration[id].first += THRUSTERS::thrusters[component].first;
			acceleration[id].second += THRUSTERS::thrusters[component].second;
			THRUSTERS::thrusters[component].first = 0;
			THRUSTERS::thrusters[component].second = 0;
		}
	}

	void translate(Simulation* sim, int id) {
		calculateAcceleration(sim, id); // Calculate net acceleration from accelertion calculated and stored in each thruster
		sim->parts[id].pavg[0] += acceleration[id].first; // Add acceleration in x to velocity x
		sim->parts[id].pavg[1] += acceleration[id].second; // Add acceleration in y to velocity y
		int rx = sim->parts[id].pavg[0]; // Velocity of body to add
		int ry = sim->parts[id].pavg[1];
		sim->parts[id].x += rx; // Teleport COTR because it's not in the components vector
		sim->parts[id].y += ry;
		// Teleport the body component by component
		for (auto itr = ships[id].begin(); itr != ships[id].end(); ++itr) {
			sim->parts[*itr].x += rx;
			sim->parts[*itr].y += ry;
		}
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
				if (BOUNDS_CHECK && (rx || ry)) { // Scan around for other components
					r = sim->pmap[y + ry][x + rx];
					if (!r || sim->parts[ID(r)].pavg[0] < 0) continue; // Skip check if doesn't exist or other component doesn't have an id either
					if (isShip(sim->parts[ID(r)].pavg[0]) || isShip(ID(r))) { // If ship id is valid then we can add it to this component
						// Checks if component adjacent is a component type or if it's pointer is
						if (isComponent(sim->parts[ID(r)].type) || isComponent(TYP(r))) {
							sim->parts[i].pavg[0] = sim->parts[ID(r)].pavg[0];
							if (TYP(r) == PT_COTR)
								sim->parts[i].pavg[0] = ID(r);
							ships[sim->parts[i].pavg[0]].push_back(i); // Add component to ship's component vector
							return;
						}
					}

				}
	}
}