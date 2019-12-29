#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_TIME PT_TIME 234
Element_TIME::Element_TIME()
{
	Identifier = "DEFAULT_PT_TIME";
	Name = "TIME";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_POWERED;
	Enabled = 1;

	Description = "Time.";

	// element properties here

	Update = &Element_TIME::update;
	Graphics = &Element_TIME::graphics;
}

//#TPT-Directive ElementHeader Element_TIME static int update(UPDATE_FUNC_ARGS)
int Element_TIME::update(UPDATE_FUNC_ARGS)
{
	// update code here
	sim->time_dilation[y / CELL][x / CELL] = 2;
	sim->time_dilation[y / CELL][x / CELL - 1] = 2;
	sim->time_dilation[y / CELL][x / CELL + 1] = 2;
	sim->time_dilation[y / CELL - 1][x / CELL] = 2;
	sim->time_dilation[y / CELL + 1][x / CELL] = 2;

	return 0;
}

//#TPT-Directive ElementHeader Element_TIME static int graphics(GRAPHICS_FUNC_ARGS)
int Element_TIME::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_TIME::~Element_TIME() {}
