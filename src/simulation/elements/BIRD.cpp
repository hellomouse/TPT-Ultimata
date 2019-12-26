#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_BIRD PT_BIRD 217
Element_BIRD::Element_BIRD()
{
	Identifier = "DEFAULT_PT_BIRD";
	Name = "BIRD";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_BIRD::update;
	Graphics = &Element_BIRD::graphics;
}

//#TPT-Directive ElementHeader Element_BIRD static int update(UPDATE_FUNC_ARGS)
int Element_BIRD::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_BIRD static int graphics(GRAPHICS_FUNC_ARGS)
int Element_BIRD::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_BIRD::~Element_BIRD() {}
