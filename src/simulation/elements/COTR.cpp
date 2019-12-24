#include "simulation/ElementCommon.h"
#include "simulation/Spaceship.h"
#include "simulation/Thruster.h"
#include "common/tpt-rand.h"
//#include <bits/stdc++.h>
#include <climits>
#include <vector>

//#TPT-Directive ElementClass Element_COTR PT_COTR 208
Element_COTR::Element_COTR()
{
	Identifier = "DEFAULT_PT_COTR";
	Name = "COTR";
	Colour = PIXPACK(0x5c0e0e);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	// element properties here
	Update = &Element_COTR::update;
	Graphics = &Element_COTR::graphics;
	Create = &Element_COTR::create;
}

//#TPT-Directive ElementHeader Element_COTR static void create(ELEMENT_CREATE_FUNC_ARGS)
void Element_COTR::create(ELEMENT_CREATE_FUNC_ARGS) {
	SHIPS::createShip(i);
	sim->parts[i].tmp = 0; // If COTR should send a signal
}

//#TPT-Directive ElementHeader Element_COTR static int update(UPDATE_FUNC_ARGS)
int Element_COTR::update(UPDATE_FUNC_ARGS)
{
	if (!sim->parts[i].tmp) {
		sim->parts[i].tmp = 1;
		for (auto component : SHIPS::ships[i]) {
			if (sim->parts[component].type != PT_THRS) continue;
				THRUSTERS::calculateThrust(sim, component); // This should be called from THRS.cpp when it detects the MOVE signal from FILT
		}
		return 0;
	}
	
	SHIPS::translate(sim, i);

	sim->parts[i].tmp = 0;
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
