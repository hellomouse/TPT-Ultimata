#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_SOIL PT_SOIL 224
Element_SOIL::Element_SOIL()
{
	Identifier = "DEFAULT_PT_SOIL";
	Name = "SOIL";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_SOIL::update;
	Graphics = &Element_SOIL::graphics;
}

//#TPT-Directive ElementHeader Element_SOIL static int update(UPDATE_FUNC_ARGS)
int Element_SOIL::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_SOIL static int graphics(GRAPHICS_FUNC_ARGS)
int Element_SOIL::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_SOIL::~Element_SOIL() {}
