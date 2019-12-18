#include "simulation/ElementCommon.h"
#include "simulation/mvsd/movingsolids.h"
#include <iostream>

//#TPT-Directive ElementClass Element_MVSD PT_MVSD 196
Element_MVSD::Element_MVSD() {
	Identifier = "DEFAULT_PT_MVSD";
	Name = "MVSD";
	Colour = PIXPACK(0xFF0000);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	HeatConduct = 255;
	Weight = 100;
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

	/** Failed to find group */
	if (MOVINGSOLID::solids.count(parts[i].tmp2) == 0) {
		std::cout << "FAiled to find froup\n";
		return 0;
	}

	/**
	 * On collision with non-moving solid object it will
	 * determine whether to mark itself as having collided
	 * with the object by setting self flags to 1.
	 */
	int r, rx, ry;
	bool collided = false;
	bool is_outer = false;

	int px = (int)(parts[i].x + 0.5);
	int py = (int)(parts[i].y + 0.5);

	// TODO remove
	if (parts[i].tmp == 3) parts[i].tmp = 0;


	// int fvx, fvy;
	// MOVINGSOLID::solids[parts[i].tmp2].get_fake_velocity(fvx, fvy);

	// Check surrounding particles. These are "touching"
	// collisions, as in the solid is flush with another particle
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry)) {
				r = pmap[y+ry][x+rx];
				if (!r) {
					is_outer = true;
					continue;
				}
				int rt = TYP(r);

				// Collision with another moving solid
				if (rt == PT_MVSD && parts[ID(r)].tmp2 != parts[i].tmp2) {
					collided = true;
					MOVINGSOLID::solids[parts[i].tmp2].add_collision(
						MOVINGSOLID::Collision(x + rx, y + ry, MOVINGSOLID::MOVING, px, py, x + rx, y + ry));
					is_outer = true;
				}

				else if (rt != PT_MVSD) {
					collided = true;
					parts[i].tmp = 3; // TODO deco
					if (sim->elements[rt].Properties & TYPE_SOLID)
						MOVINGSOLID::solids[parts[i].tmp2].add_collision(
							MOVINGSOLID::Collision(x + rx, y + ry, MOVINGSOLID::STATIC, px, py, x + rx, y + ry));
					else if (sim->elements[rt].Properties & TYPE_PART)
						MOVINGSOLID::solids[parts[i].tmp2].add_collision(
							MOVINGSOLID::Collision(x + rx, y + ry, MOVINGSOLID::NONSTATIC, px, py, x + rx, y + ry));
					is_outer = true;
				}
			}

	// if (is_outer)
	// 	parts[i].tmp = 3;
	// else if (parts[i].tmp == 3 && !is_outer)
	// 	parts[i].tmp = 0;

	// Check for phasing into solids
	float vx_ = MOVINGSOLID::solids[parts[i].tmp2].getVX(),
		  vy_ = MOVINGSOLID::solids[parts[i].tmp2].getVY();

	if (vx_ || vy_) {
		float largest = fabs(vx_) > fabs(vy_) ? fabs(vx_) : fabs(vy_);
		if (largest <= 1) // Avoid "division by 0" effect where it ends up scanning the entire map
			return 0;

		float dvx = vx_ / largest, dvy = vy_ / largest;

		float sx = (int)(parts[i].x + 0.5), sy = (int)(parts[i].y + 0.5);
		int px = 0, py = 0;
		// int tx = (int)(parts[i].x + vx_ + 0.5), ty = (int)(parts[i].y + vy_ + 0.5);
		int count = 1;

		// TODO dont check same coordinate twice
		while (sx >= 0 && sy >= 0 && sx < XRES && sy < YRES && fabs(-dvx + dvx * count) <= fabs(vx_) && fabs(-dvy + dvy * count) <= fabs(vy_)) {
			if (px == (int)round(sy) && py == (int)round(sy))
				goto endloop;

			// Avoid rubberbanding
			if (fabs((int)round(sx) - px) > MOVINGSOLID::MAX_VELOCITY ||
				fabs((int)round(sy) - py) > MOVINGSOLID::MAX_VELOCITY)
				goto endloop;

			r = pmap[(int)round(sy)][(int)round(sx)];
			if (r) {
				// std::cout << i << ": " << sx << ", " << sy << "\n";
				if (TYP(r) != PT_MVSD && 
						(sim->elements[TYP(r)].Properties & TYPE_PART ||
						 sim->elements[TYP(r)].Properties & TYPE_SOLID)) {
					MOVINGSOLID::solids[parts[i].tmp2].add_collision(
						MOVINGSOLID::Collision((int)sx, (int)sy, MOVINGSOLID::NONSTATIC,
						px, py, (int)round(sx), (int)round(sy)));

					int dx = (int)round(sx) - px;
					int dy = (int)round(sy) - py;

					if (dx != 0) dx += dx < 0 ? 1 : -1;
					if (dy != 0) dy += dy < 0 ? 1 : -1;
						
					// std::cout << parts[i].x << ", " << parts[i].y << ": " << dx << ": " << dy << "\n";
					MOVINGSOLID::solids[parts[i].tmp2].update_delta(dx, dy);
					parts[i].tmp = 1;
					break;
				} 
				else if (parts[ID(r)].tmp2 != parts[i].tmp2) {

				}
				
			}

			endloop:
			px = (int)round(sx); py = (int)round(sy);
			sx += dvx;
			sy += dvy;
			count++;
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
	if (cpart->tmp == 1) {
		*colb = 255;
		*colr = 255;
		*colg = 0;
	}
	if (cpart->tmp == 2) {
		*colb = 0;
		*colr = 255;
		*colg = 255;
	}
	if (cpart->tmp == 3) {
		*colb = 255;
		*colr = 255;
		*colg = 255;
	}

	return 0;
}

Element_MVSD::~Element_MVSD() {}
