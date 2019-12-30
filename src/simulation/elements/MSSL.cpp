#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_MSSL PT_MSSL 237
Element_MSSL::Element_MSSL()
{
	Identifier = "DEFAULT_PT_MSSL";
	Name = "MSSL";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_MSSL::update;
	Graphics = &Element_MSSL::graphics;
}

//#TPT-Directive ElementHeader Element_MSSL static int update(UPDATE_FUNC_ARGS)
int Element_MSSL::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_MSSL static int graphics(GRAPHICS_FUNC_ARGS)
int Element_MSSL::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_MSSL::~Element_MSSL() {}
