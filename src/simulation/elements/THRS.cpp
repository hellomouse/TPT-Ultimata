#include "simulation/ElementCommon.h"
#include "simulation/Spaceship.h"
#include <math.h>
#include <vector>



namespace THRUSTERS {
	std::unordered_map<int, std::pair<float, float>> thrusters;

	std::vector<std::pair<int, int>> direction(Simulation* sim, int i, int x, int y) {
		int r, rx, ry;
		std::vector <std::pair<int, int>> accelerationEfficiency;
		for (rx = -1; rx <= 1; rx++)
			for (ry = -1; ry <= 1; ry++)
				if (BOUNDS_CHECK && (rx || ry)) { // Scan around for other components
					r = sim->pmap[y + ry][x + rx];
					if (r) continue;
					accelerationEfficiency.push_back(std::make_pair(rx, ry));
				}
		return accelerationEfficiency;
	}

 	void calculateThrust(Simulation* sim, int i) {
		int x = sim->parts[i].x;
		int y = sim->parts[i].y;

		if (!SHIPS::isShip(sim->parts[i].pavg[0]))
			return;

		THRUSTERS::thrusters[i].first = 0;
		THRUSTERS::thrusters[i].second = 0;

		int numDirections;
		std::array<float, 2> thrust_acceleration = { 0.001, 0.001 };
		std::vector<std::pair<int, int>> directions = THRUSTERS::direction(sim, i, x, y);
		numDirections = directions.size();
		if (numDirections) {
			thrust_acceleration[0] /= numDirections;
			thrust_acceleration[1] /= numDirections;
			for (auto direction : directions) {
				THRUSTERS::thrusters[i].first += -1 * direction.first * thrust_acceleration[0];
				THRUSTERS::thrusters[i].second += -1 * direction.second * thrust_acceleration[1];
			}
		}
	}
}

//#TPT-Directive ElementClass Element_THRS PT_THRS 211
Element_THRS::Element_THRS()
{
	Identifier = "DEFAULT_PT_THRS";
	Name = "THRS";
	Colour = PIXPACK(0x030163);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_THRS::update;
	Graphics = &Element_THRS::graphics;
	Create = &Element_THRS::create;
}

//#TPT-Directive ElementHeader Element_THRS static void create(ELEMENT_CREATE_FUNC_ARGS)
void Element_THRS::create(ELEMENT_CREATE_FUNC_ARGS) {
	sim->parts[i].pavg[0] = -1;
	SHIPS::clonePAVG(sim, i, x, y);
	THRUSTERS::thrusters[i] = { 0, 0 };
}

//#TPT-Directive ElementHeader Element_THRS static int update(UPDATE_FUNC_ARGS)
int Element_THRS::update(UPDATE_FUNC_ARGS)
{
	if (parts[i].pavg[0] < 0) {
		SHIPS::clonePAVG(sim, i, x, y);
	}
	return 0;
}

//#TPT-Directive ElementHeader Element_THRS static int graphics(GRAPHICS_FUNC_ARGS)
int Element_THRS::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_THRS::~Element_THRS() {}
