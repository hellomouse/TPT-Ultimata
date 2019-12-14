#include "simulation/ElementCommon.h"
#include "simulation/quantum/quantum.h"

//#TPT-Directive ElementClass Element_ION PT_ION 193
Element_ION::Element_ION()
{
	Identifier = "DEFAULT_PT_ION";
	Name = "ION";
	Colour = PIXPACK(0xFF0000);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	Update = &Element_ION::update;
	Graphics = &Element_ION::graphics;
}

//#TPT-Directive ElementHeader Element_ION static int update(UPDATE_FUNC_ARGS)
int Element_ION::update(UPDATE_FUNC_ARGS) {
	/**
	 * ION stores a state id, which is the quantum state its linked to, inside
	 * of its tmp2 variable
	 */

	// Create quantum state if none:
	if (parts[i].flags != 1) {
		QUANTUM::create_particle_state(parts, i);
		parts[i].flags = 1;
	}

	return 0;
}

//#TPT-Directive ElementHeader Element_ION static int graphics(GRAPHICS_FUNC_ARGS)
int Element_ION::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here

	return 0;
}

Element_ION::~Element_ION() {}
