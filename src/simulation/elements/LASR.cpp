#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_LASR PT_LASR 207
Element_LASR::Element_LASR()
{
	Identifier = "DEFAULT_PT_LASR";
	Name = "LASR";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_LASR::update;
	Graphics = &Element_LASR::graphics;
}

//#TPT-Directive ElementHeader Element_LASR static int update(UPDATE_FUNC_ARGS)
int Element_LASR::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_LASR static int graphics(GRAPHICS_FUNC_ARGS)
int Element_LASR::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_LASR::~Element_LASR() {}
