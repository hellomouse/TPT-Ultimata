#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_CRBN PT_CRBN 215
Element_CRBN::Element_CRBN()
{
	Identifier = "DEFAULT_PT_CRBN";
	Name = "CRBN";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_CRBN::update;
	Graphics = &Element_CRBN::graphics;
}

//#TPT-Directive ElementHeader Element_CRBN static int update(UPDATE_FUNC_ARGS)
int Element_CRBN::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_CRBN static int graphics(GRAPHICS_FUNC_ARGS)
int Element_CRBN::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_CRBN::~Element_CRBN() {}
