#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_GNSH PT_GNSH 214
Element_GNSH::Element_GNSH()
{
	Identifier = "DEFAULT_PT_GNSH";
	Name = "GNSH";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_GNSH::update;
	Graphics = &Element_GNSH::graphics;
}

//#TPT-Directive ElementHeader Element_GNSH static int update(UPDATE_FUNC_ARGS)
int Element_GNSH::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_GNSH static int graphics(GRAPHICS_FUNC_ARGS)
int Element_GNSH::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_GNSH::~Element_GNSH() {}
