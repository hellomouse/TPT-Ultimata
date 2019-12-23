#include "simulation/ElementCommon.h"
#include "simulation/vehicles/vehicle.h"
#include "simulation/vehicles/cybertruck.h"

//#TPT-Directive ElementClass Element_TANK PT_TANK 213
Element_TANK::Element_TANK()
{
	Identifier = "DEFAULT_PT_TANK";
	Name = "TANK";
	Colour = PIXPACK(0x4D5564);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	Advection = 0.01f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.15f; // NOTE: TANKS UPDATES TWICE PER FRAME
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 20;
	Description = "Soviet Tank. STKM can ride, comes with several upgrades.";

	DefaultProperties.life = 600; // Default 600 HP

	Properties = TYPE_PART | PROP_NOCTYPEDRAW | PROP_VEHICLE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_TANK::update;
	Graphics = &Element_TANK::graphics;
	ChangeType = &Element_TANK::changeType;
}

//#TPT-Directive ElementHeader Element_TANK static void changeType(ELEMENT_CHANGETYPE_FUNC_ARGS)
void Element_TANK::changeType(ELEMENT_CHANGETYPE_FUNC_ARGS) {
	if (to == PT_NONE && sim->parts[i].life <= 0) {
		int j, t;
		for (auto px = CYBERTRUCK_PIXELS.begin(); px != CYBERTRUCK_PIXELS.end(); ++px) {
			t = rand() % 100;
			if (t < 20)
				j = Element_CYTK::create_part(sim, px->x, px->y, PT_BGLA, sim->parts[i].pavg[0], sim->parts, i);
			else if (t < 70)
				j = Element_CYTK::create_part(sim, px->x, px->y, PT_BRMT, sim->parts[i].pavg[0], sim->parts, i);
			else
				j = Element_CYTK::create_part(sim, px->x, px->y, PT_BREC, sim->parts[i].pavg[0], sim->parts, i);
			if (j > -1) {
				sim->parts[j].dcolour = 0xAF000000 | PIXRGB(px->r, px->g, px->b);
				sim->parts[j].vx = sim->parts[i].vx;
				sim->parts[j].vy = sim->parts[i].vy;
				sim->parts[j].temp = sim->parts[i].temp;
			}
		}
	}
}

//#TPT-Directive ElementHeader Element_TANK static int update(UPDATE_FUNC_ARGS)
int Element_TANK::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_TANK static int graphics(GRAPHICS_FUNC_ARGS)
int Element_TANK::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_TANK::~Element_TANK() {}
