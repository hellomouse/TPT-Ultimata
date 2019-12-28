#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_SPDR PT_SPDR 227
Element_SPDR::Element_SPDR()
{
	Identifier = "DEFAULT_PT_SPDR";
	Name = "SPDR";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_SPDR::update;
	Graphics = &Element_SPDR::graphics;
}

//#TPT-Directive ElementHeader Element_SPDR static int update(UPDATE_FUNC_ARGS)
int Element_SPDR::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_SPDR static int graphics(GRAPHICS_FUNC_ARGS)
int Element_SPDR::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_SPDR::~Element_SPDR() {}
