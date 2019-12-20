#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_COLD PT_COLD 201
Element_COLD::Element_COLD()
{
	Identifier = "DEFAULT_PT_COLD";
	Name = "COLD";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_COLD::update;
	Graphics = &Element_COLD::graphics;
}

//#TPT-Directive ElementHeader Element_COLD static int update(UPDATE_FUNC_ARGS)
int Element_COLD::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_COLD static int graphics(GRAPHICS_FUNC_ARGS)
int Element_COLD::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_COLD::~Element_COLD() {}
