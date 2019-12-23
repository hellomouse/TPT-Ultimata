#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_COTR PT_COTR 208
Element_COTR::Element_COTR()
{
	Identifier = "DEFAULT_PT_COTR";
	Name = "COTR";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_COTR::update;
	Graphics = &Element_COTR::graphics;
}

//#TPT-Directive ElementHeader Element_COTR static int update(UPDATE_FUNC_ARGS)
int Element_COTR::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_COTR static int graphics(GRAPHICS_FUNC_ARGS)
int Element_COTR::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_COTR::~Element_COTR() {}
