#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_ANT PT_ANT 216
Element_ANT::Element_ANT()
{
	Identifier = "DEFAULT_PT_ANT";
	Name = "ANT";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_ORGANIC;
	Enabled = 1;

	Advection = 0.1f;
	AirDrag = 0.09f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 1;

	Flammable = 30;
	Explosive = 1;
	Meltable = 0;
	Hardness = 2;

	Weight = 90;

	HeatConduct = 150;
	Description = "Ants, will seek out SUGR. Flammable.";

	Properties = TYPE_ENERGY;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_ANT::update;
	Graphics = &Element_ANT::graphics;
}

//#TPT-Directive ElementHeader Element_ANT static int update(UPDATE_FUNC_ARGS)
int Element_ANT::update(UPDATE_FUNC_ARGS)
{
	/**
	 * tmp: dir: 0 = up, 1 = up-right, ... 7 = left-up
	 * tmp2: state (0 or 1)
	 */
	int rx, ry, r;
	int dir = parts[i].tmp;
	int state = parts[i].tmp2;

	if (dir == 0) rx = 0, ry = -1;
	else if (dir == 1) rx = 1, ry = 0;
	else if (dir == 2) rx = 0, ry = 1;
	else if (dir == 3) rx = -1, ry = 0;

	// 8 directions
	// if (dir == 0) rx = 0, ry = -1;
	// else if (dir == 1) rx = 1, ry = -1;
	// else if (dir == 2) rx = 1, ry = 0;
	// else if (dir == 3) rx = 1, ry = 1; // Bottom-down
	// else if (dir == 4) rx = 0, ry = 1;
	// else if (dir == 5) rx = -1, ry = 1;
	// else if (dir == 6) rx = -1, ry = 0;
	// else if (dir == 7) rx = -1, ry = -1;

	if (BOUNDS_CHECK) {
		r = pmap[y + ry][x + rx];
		parts[i].x = x + rx;
		parts[i].y = y + ry;

		if (!state) {
			sim->create_part(-3, x, y, PT_BRCK);

			// Turn randomly left or right
			parts[i].tmp = (parts[i].tmp + (RNG::Ref().chance(1, 2) ? 3 : 5)) % 4;
		} else {
			sim->kill_part(ID(r));
		}

		if (!r) {
			parts[i].tmp2 = 0;
		} else {
			parts[i].tmp2 = 1;
		}
	}


	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_ANT static int graphics(GRAPHICS_FUNC_ARGS)
int Element_ANT::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_ANT::~Element_ANT() {}
