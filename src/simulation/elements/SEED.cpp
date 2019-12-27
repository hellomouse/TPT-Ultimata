#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_SEED PT_SEED 222
Element_SEED::Element_SEED()
{
	Identifier = "DEFAULT_PT_SEED";
	Name = "SEED";
	Colour = PIXPACK(0xA3B808);
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
	PhotonReflectWavelengths = 0x3FFFFFC0;

	Weight = 32;

	HeatConduct = 70;
	Description = "Seed, sprouts into trees when planted in soil.";

	Properties = TYPE_PART;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_SEED::update;
	Graphics = &Element_SEED::graphics;
}

//#TPT-Directive ElementHeader Element_SEED static int update(UPDATE_FUNC_ARGS)
int Element_SEED::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_SEED static int graphics(GRAPHICS_FUNC_ARGS)
int Element_SEED::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_SEED::~Element_SEED() {}
