#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_BLOD PT_BLOD 223
Element_BLOD::Element_BLOD()
{
	Identifier = "DEFAULT_PT_BLOD";
	Name = "BLOD";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_BLOD::update;
	Graphics = &Element_BLOD::graphics;
}

//#TPT-Directive ElementHeader Element_BLOD static int update(UPDATE_FUNC_ARGS)
int Element_BLOD::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_BLOD static int graphics(GRAPHICS_FUNC_ARGS)
int Element_BLOD::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_BLOD::~Element_BLOD() {}
