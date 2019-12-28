#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_SFLD PT_SFLD 230
Element_SFLD::Element_SFLD()
{
	Identifier = "DEFAULT_PT_SFLD";
	Name = "SFLD";
	Colour = PIXPACK(0x80A0EF);
	MenuVisible = 1;
	MenuSection = SC_LIQUID;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.98f;
	Loss = 1.0f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 30;

	DefaultProperties.temp = 20.0f;
	HeatConduct = 70;
	Description = "Superfluid neon.";

	Properties = TYPE_LIQUID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 27.102f;
	HighTemperatureTransition = PT_NEON;

	Update = &Element_SFLD::update;
	Graphics = &Element_SFLD::graphics;
}

//#TPT-Directive ElementHeader Element_SFLD static int update(UPDATE_FUNC_ARGS)
int Element_SFLD::update(UPDATE_FUNC_ARGS) {
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_SFLD static int graphics(GRAPHICS_FUNC_ARGS)
int Element_SFLD::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_SFLD::~Element_SFLD() {}
