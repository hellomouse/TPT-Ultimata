#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_GLUE PT_GLUE 198
Element_GLUE::Element_GLUE()
{
	Identifier = "DEFAULT_PT_GLUE";
	Name = "GLUE";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f  * CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 35;

	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	HeatConduct = 29;
	Description = "Glue. Use it to stick powders together into solids.";

	Properties = TYPE_LIQUID|PROP_NEUTPENETRATE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 273.15f + 150.0f;
	HighTemperatureTransition = NT;

	Update = &Element_GLUE::update;
	Graphics = &Element_GLUE::graphics;
}

//#TPT-Directive ElementHeader Element_GLUE static int update(UPDATE_FUNC_ARGS)
int Element_GLUE::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_GLUE static int graphics(GRAPHICS_FUNC_ARGS)
int Element_GLUE::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_GLUE::~Element_GLUE() {}
