#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_MUD PT_MUD 225
Element_MUD::Element_MUD()
{
	Identifier = "DEFAULT_PT_MUD";
	Name = "MUD";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_MUD::update;
	Graphics = &Element_MUD::graphics;
}

//#TPT-Directive ElementHeader Element_MUD static int update(UPDATE_FUNC_ARGS)
int Element_MUD::update(UPDATE_FUNC_ARGS)
{
	// update code here

	return 0;
}

//#TPT-Directive ElementHeader Element_MUD static int graphics(GRAPHICS_FUNC_ARGS)
int Element_MUD::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_MUD::~Element_MUD() {}
