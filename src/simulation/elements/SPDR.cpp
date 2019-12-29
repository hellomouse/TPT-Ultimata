#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_SPDR PT_SPDR 228
Element_SPDR::Element_SPDR()
{
	Identifier = "DEFAULT_PT_SPDR";
	Name = "SPDR";
	Colour = PIXPACK(0x444444);
	MenuVisible = 1;
	MenuSection = SC_ORGANIC;
	Enabled = 1;

	Advection = 0.7f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.96f;
	Loss = 0.80f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 10;
	Explosive = 0;
	Meltable = 0;
	Hardness = 30;

	Weight = 32;

	HeatConduct = 70;
	Description = "Spider. Spins webs and eats captured creatures.";

	Properties = TYPE_PART;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = 5.0f;
	HighPressureTransition = PT_BCTR;
	LowTemperature = 273.15f - 5.0f;
	LowTemperatureTransition = PT_SNOW;
	HighTemperature = 273.15f + 60.0f;
	HighTemperatureTransition = PT_DUST;

	Update = &Element_SPDR::update;
	Create = &Element_SPDR::create;
}

//#TPT-Directive ElementHeader Element_SPDR static void create(ELEMENT_CREATE_FUNC_ARGS)
void Element_SPDR::create(ELEMENT_CREATE_FUNC_ARGS) {
	sim->parts[i].tmp = RNG::Ref().chance(1, 2);
}

//#TPT-Directive ElementHeader Element_SPDR static int update(UPDATE_FUNC_ARGS)
int Element_SPDR::update(UPDATE_FUNC_ARGS) {
	/**
	 * Properties:
	 * tmp: left or right (0 = left, 1 = right)
	 */
	int rx, ry, r, rt;
	bool touching = false;

	for (rx = -1; rx < 2; ++rx)
		for (ry = -1; ry < 2; ++ry)
			if (BOUNDS_CHECK && (rx || ry) && (rx == 0 || ry == 0)) {
				r = pmap[y + ry][x + rx];
				if (!r) continue;
				rt = TYP(r);

				if (sim->elements[rt].Properties & TYPE_SOLID)
					touching = true;
			}

	// Dont move if touching a solid
	if (touching) {
		parts[i].vx = 0;
		parts[i].vy = 0;
	}


	// Move horizontally
	if (!pmap[y][x + (parts[i].tmp ? -1 : 1)]) {
		// Cannot move without falling
		if (!pmap[y + 1][x + (parts[i].tmp ? -1 : 1)] && !pmap[y + 2][x + (parts[i].tmp ? -1 : 1)])
			parts[i].tmp = parts[i].tmp == 1 ? 0 : 1;
		else
			parts[i].x += (parts[i].tmp ? -1 : 1);
	}
	// Move up if possible
	else if (touching && !pmap[y - 1][x])
		--parts[i].y;
	// No move possible, try spinning a web and moving to its mid point, starting at 90 degree angles
	// on either side
	else {
		
	}
	

	return 0;
}

Element_SPDR::~Element_SPDR() {}
