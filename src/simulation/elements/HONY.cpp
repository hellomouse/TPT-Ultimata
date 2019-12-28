#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_HONY PT_HONY 228
Element_HONY::Element_HONY()
{
	Identifier = "DEFAULT_PT_HONY";
	Name = "HONY";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_HONY::update;
	Graphics = &Element_HONY::graphics;
}

//#TPT-Directive ElementHeader Element_HONY static int update(UPDATE_FUNC_ARGS)
int Element_HONY::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_HONY static int graphics(GRAPHICS_FUNC_ARGS)
int Element_HONY::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_HONY::~Element_HONY() {}
