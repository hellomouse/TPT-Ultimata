#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_ANT PT_ANT 216
Element_ANT::Element_ANT()
{
	Identifier = "DEFAULT_PT_ANT";
	Name = "ANT";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	Advection = 0.1f;
	AirDrag = 0.09f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.2f;
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

	Properties = TYPE_PART;

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
