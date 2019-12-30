#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_MUD PT_MUD 225
Element_MUD::Element_MUD()
{
	Identifier = "DEFAULT_PT_MUD";
	Name = "MUD";
	Colour = PIXPACK(0x493804);
	MenuVisible = 1;
	MenuSection = SC_ORGANIC;
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

	DefaultProperties.temp = 30.0f + 273.15f;
	HeatConduct = 29;
	Description = "Mud. Sticky liquid, can be baked into BRCK.";

	Properties = TYPE_LIQUID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 500.0f + 273.15f;
	HighTemperatureTransition = PT_BRCK;

	Update = &Element_MUD::update;
	Graphics = &Element_MUD::graphics;
}

//#TPT-Directive ElementHeader Element_MUD static int update(UPDATE_FUNC_ARGS)
int Element_MUD::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_MUD static int graphics(GRAPHICS_FUNC_ARGS)
int Element_MUD::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_MUD::~Element_MUD() {}
