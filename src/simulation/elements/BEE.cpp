#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_BEE PT_BEE 226
Element_BEE::Element_BEE()
{
	Identifier = "DEFAULT_PT_BEE";
	Name = "BEE";
	Colour = PIXPACK(0xffc414);
	MenuVisible = 1;
	MenuSection = SC_ORGANIC;
	Enabled = 1;

	Advection = 0.7f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.96f;
	Loss = 0.80f;
	Collision = 0.00f;
	Gravity = 0.10f;
	Diffusion = 1.00f;
	HotAir = 0.0000f * CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 100;

	HeatConduct = 70;
	Description = "Bees. Pollinates PLNT, makes honey and wax hives.";

	Properties = TYPE_GAS | PROP_DEADLY;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = 10.0f;
	HighPressureTransition = PT_DUST;
	LowTemperature = 273.15f - 5.0f;
	LowTemperatureTransition = PT_SNOW;
	HighTemperature = 273.15f + 70.0f;
	HighTemperatureTransition = PT_DUST;

	Update = &Element_BEE::update;
	Graphics = &Element_BEE::graphics;
	Create = &Element_BEE::create;
}

//#TPT-Directive ElementHeader Element_BEE static void create(ELEMENT_CREATE_FUNC_ARGS)
void Element_BEE::create(ELEMENT_CREATE_FUNC_ARGS) {
	sim->parts[i].tmp = RNG::Ref().between(0, 7);
}


//#TPT-Directive ElementHeader Element_BEE static int update(UPDATE_FUNC_ARGS)
int Element_BEE::update(UPDATE_FUNC_ARGS) {
	/**
	 * Properties:
	 * tmp - color variation (0 - 6 inclusive)
	 */

	return 0;
}

//#TPT-Directive ElementHeader Element_BEE static int graphics(GRAPHICS_FUNC_ARGS)
int Element_BEE::graphics(GRAPHICS_FUNC_ARGS) {
	*colr *= (float)cpart->tmp / 6;
	*colg *= (float)cpart->tmp / 6;
	*colb *= (float)cpart->tmp / 6;
	return 0;
}

Element_BEE::~Element_BEE() {}
