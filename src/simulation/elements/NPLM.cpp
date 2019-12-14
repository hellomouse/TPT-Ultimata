#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_NPLM PT_NPLM 191
Element_NPLM::Element_NPLM()
{
	Identifier = "DEFAULT_PT_NPLM";
	Name = "NPLM";
	Colour = PIXPACK(0xB00000);  //Get it BOOOO
	MenuVisible = 1;
	MenuSection = SC_EXPLOSIVE;
	Enabled = 1;
	
	Advection = 0.5f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.97f;
	Collision = 0.0f;
	Gravity = 0.2f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	
	Weight = 24;
	
	DefaultProperties.temp = R_TEMP + 273.15f;
	DefaultProperties.life = 1200;

	HeatConduct = 88;
	Description = "Napalm. Ultra-long burning explosive (Use sparingly!).";
	
	Properties = TYPE_LIQUID | PROP_LIFE_KILL;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 4000.0f;
	HighTemperatureTransition = PT_H2;
	
	Update = Update = &Element_NPLM::update;
	
}

//#TPT-Directive ElementHeader Element_NPLM static int update(UPDATE_FUNC_ARGS)
int Element_NPLM::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, rt;
	for (rx = -1; rx < 2; rx++){
		for (ry = -1; ry < 2; ry++){
			if (BOUNDS_CHECK && (rx || ry)) {
				r = pmap[y + ry][x + rx];
				if (TYP(r) == PT_NPLM)
					r = sim->photons[y + ry][x + rx];
				if (!r)
					continue;

				if (parts[ID(r)].type == PT_FIRE || parts[ID(r)].type == PT_PLSM || parts[ID(r)].type == PT_LAVA
												 || parts[i].temp >= 1273.15f || parts[i].tmp == 1) {
					parts[i].tmp = 1;
					sim->create_part(-1, parts[i].x - 1, parts[i].y - 1, PT_FIRE);

					// More fire!
					// sim->create_part(-1, parts[i].x - 1, parts[i].y + 1, PT_FIRE);
					// sim->create_part(-1, parts[i].x + 1, parts[i].y + 1, PT_FIRE);
					// sim->create_part(-1, parts[i].x + 1, parts[i].y - 1, PT_FIRE);

					sim->create_part(-1, parts[i].x - 1, parts[i].y - 1, PT_BCOL);
					sim->create_part(-1, parts[i].x - 1, parts[i].y + 1, PT_NITR);
					sim->create_part(-1, parts[i].x + 1, parts[i].y + 1, PT_BCOL);
					// sim->create_part(-1, parts[i].x + 1, parts[i].y - 1, PT_THRM);
					
					parts[ID(r)].vx = parts[ID(r)].vx * 2;
					parts[ID(r)].vy = parts[ID(r)].vy * 2;
					parts[ID(r)].life = rand() % 400;
					parts[i].temp += 50.0f;
					parts[i].life -= 1;
				}
			}
		}
	}
	return 0;
}

Element_NPLM::~Element_NPLM() {}