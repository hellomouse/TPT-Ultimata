#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_TRBN PT_TRBN 239
Element_TRBN::Element_TRBN() {
	Identifier = "DEFAULT_PT_TRBN";
	Name = "TRBN";
	Colour = PIXPACK(0xDDDDDD);
	MenuVisible = 1;
	MenuSection = SC_SENSOR;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;
	Weight = 100;

	HeatConduct = 0;
	Description = "Turbine. Makes SPRK when in moving air or particles. pavg0 = air velocity, pavg1 = particle velocity.";

	Properties = TYPE_SOLID | PROP_NOCTYPEDRAW;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 273.15f + 2000.0f;
	HighTemperatureTransition = PT_METL;

	Update = &Element_TRBN::update;
	Graphics = &Element_TRBN::graphics;
	Create = &Element_TRBN::create;
}

//#TPT-Directive ElementHeader Element_TRBN static void create(ELEMENT_CREATE_FUNC_ARGS)
void Element_TRBN::create(ELEMENT_CREATE_FUNC_ARGS) {
	sim->parts[i].pavg[0] = 0.3f;
	sim->parts[i].pavg[1] = 1.0f;
}

//#TPT-Directive ElementHeader Element_TRBN static int update(UPDATE_FUNC_ARGS)
int Element_TRBN::update(UPDATE_FUNC_ARGS) {
	// tmp = Graphics iteration
	float vx = sim->vx[y / CELL][x / CELL];
	float vy = sim->vy[y / CELL][x / CELL];
	float air_speed = sqrtf(vx * vx + vy * vy);

	int rx, ry, r;
	bool fastpart = air_speed > sim->parts[i].pavg[0];
	bool already_animate = false;
	for (rx = -1; rx <= 1; ++rx)
	for (ry = -1; ry <= 1; ++ry)
		if (BOUNDS_CHECK && (rx || ry)) {
			r = pmap[y + ry][x + rx];
			if (!r) continue;

			float absv = sqrtf(parts[ID(r)].vx * parts[ID(r)].vx + parts[ID(r)].vy * parts[ID(r)].vy);
			if (absv > sim->parts[i].pavg[1])
				fastpart = true;

			if (fastpart && !already_animate) {
				++parts[i].tmp; // Graphics
				already_animate = true;
			}

			// Shred birds
			if (fastpart && TYP(r) == PT_BIRD) {
				sim->part_change_type(ID(r), x + rx, y + ry, PT_BLOD);
			}

			if (fastpart && sim->elements[TYP(r)].Properties & PROP_CONDUCTS) {
				parts[ID(r)].ctype = TYP(r);
				parts[ID(r)].life = 4;
				sim->part_change_type(ID(r), x + rx, y + ry, PT_SPRK);
				return 0;
			}
		}

	return 0;
}

//#TPT-Directive ElementHeader Element_TRBN static int graphics(GRAPHICS_FUNC_ARGS)
int Element_TRBN::graphics(GRAPHICS_FUNC_ARGS) {
	float m = ((cpart->tmp + nx + ny) % 6) / 5.0f;
	m = 0.4f + 0.6f * m;
	
	*colr *= m;
	*colg *= m;
	*colb *= m;

	return 0;
}

Element_TRBN::~Element_TRBN() {}
