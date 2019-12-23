#include "simulation/ElementCommon.h"
#include "simulation/Spaceship.h"


//#TPT-Directive ElementClass Element_HULL PT_HULL 212
Element_HULL::Element_HULL()
{
	Identifier = "DEFAULT_PT_HULL";
	Name = "HULL";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here

	Update = &Element_HULL::update;
	Graphics = &Element_HULL::graphics;
	Create = &Element_HULL::create;
}

//#TPT-Directive ElementHeader Element_HULL static void create(ELEMENT_CREATE_FUNC_ARGS)
void Element_HULL::create(ELEMENT_CREATE_FUNC_ARGS)
{
	SHIPS::cloneTMP(sim, i, x, y);
}

//#TPT-Directive ElementHeader Element_HULL static int update(UPDATE_FUNC_ARGS)
int Element_HULL::update(UPDATE_FUNC_ARGS)
{
	if (!parts[i].tmp) {
		SHIPS::cloneTMP(sim, i, x, y);
	}

	return 0;
}

//#TPT-Directive ElementHeader Element_HULL static int graphics(GRAPHICS_FUNC_ARGS)
int Element_HULL::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_HULL::~Element_HULL() {}
