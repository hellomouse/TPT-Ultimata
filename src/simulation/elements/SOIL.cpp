#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_SOIL PT_SOIL 224
Element_SOIL::Element_SOIL()
{
	Identifier = "DEFAULT_PT_SOIL";
	Name = "SOIL";
	Colour = PIXPACK(0x9F7616);
	MenuVisible = 1;
	MenuSection = SC_ORGANIC;
	Enabled = 1;

	Advection = 0.7f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.2f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 2;
	Hardness = 2;

	Weight = 55;

	HeatConduct = 70;
	Description = "Soil. Turns into mud when wet, great for growing seeds.";

	Properties = TYPE_PART;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 273.15f + 60.0f;
	HighTemperatureTransition = PT_CLST;

	Update = &Element_SOIL::update;
	Graphics = &Element_SOIL::graphics;
	Create = &Element_CLST::create;
}

//#TPT-Directive ElementHeader Element_SOIL static int update(UPDATE_FUNC_ARGS)
int Element_SOIL::update(UPDATE_FUNC_ARGS) {
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_SOIL static int graphics(GRAPHICS_FUNC_ARGS)
int Element_SOIL::graphics(GRAPHICS_FUNC_ARGS) {
	int z = (cpart->tmp - 5) * 8;//speckles!
	*colr += z;
	*colg += z;
	*colb += z;
	return 0;
}

Element_SOIL::~Element_SOIL() {}
