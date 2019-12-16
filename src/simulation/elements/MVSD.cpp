#include "simulation/ElementCommon.h"
#include "simulation/mvsd/movingsolids.h"

//#TPT-Directive ElementClass Element_MVSD PT_MVSD 196
Element_MVSD::Element_MVSD() {
	Identifier = "DEFAULT_PT_MVSD";
	Name = "MVSD";
	Colour = PIXPACK(0xFF0000);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	HeatConduct = 255;
	Description = "Moving Solid.";

	Properties = TYPE_SOLID;

	Update = &Element_MVSD::update;
	Graphics = &Element_MVSD::graphics;
}

//#TPT-Directive ElementHeader Element_MVSD static int update(UPDATE_FUNC_ARGS)
int Element_MVSD::update(UPDATE_FUNC_ARGS) {
	/**
	 * Moving solid setup: moving solids are grouped by their ID, which is defined by tmp2
	 * If tmp2 is 0 the particle will automatically begin a floodfill to detect other MVSD that
	 * belongs to this moving solid, and group any that are not part of this moving solid group
	 */
	if (parts[i].tmp2 == 0)
		MOVINGSOLID::create_moving_solid(parts, pmap, i);

	/**
	 * On collision with non-moving solid object it will
	 * determine whether to mark itself as having collided
	 * with the object by setting self flags to 1
	 */
	int r, rx, ry;
	bool collided = false;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry)) {
				r = pmap[y+ry][x+rx];
				if (!r) continue;
				int rt = TYP(r);

				if (rt != PT_MVSD || (parts[ID(r)].tmp2 != parts[i].tmp2)) {
					collided = true;
				}
			}

	parts[i].flags = collided ? 1 : 0;

	return 0;
}

//#TPT-Directive ElementHeader Element_MVSD static int graphics(GRAPHICS_FUNC_ARGS)
int Element_MVSD::graphics(GRAPHICS_FUNC_ARGS) {
	// graphics code here
	// return 1 if nothing dymanic happens here

	if (cpart->flags > 0) {
		*colb = 255;
		*colr = 0;
		*colg = 0;
	}

	return 0;
}

Element_MVSD::~Element_MVSD() {}
