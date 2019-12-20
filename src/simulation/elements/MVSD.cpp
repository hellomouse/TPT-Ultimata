#include "simulation/ElementCommon.h"
#include "simulation/mvsd/movingsolids.h"
#include <iostream>

namespace MVSD {
	void reset(int x, int y, Particle *parts, int pmap[YRES][XRES], int state_id) {
		if (x < 0 || y < 0 || x > XRES || y > YRES)
        	return;
		int id = ID(pmap[y][x]);
		if (parts[id].type != PT_MVSD || parts[id].tmp2 != state_id)
        	return;
		parts[id].tmp2 = 0;

		reset(x - 1, y, parts, pmap, state_id);
		reset(x + 1, y, parts, pmap, state_id);
		reset(x, y - 1, parts, pmap, state_id);
		reset(x, y + 1, parts, pmap, state_id);
	}
}

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
	 * 
	 * pavg[0] and pavg[1] store the solid's velocity for updating when a solid is cut
	 * into 2 solids
	 */
	if (parts[i].tmp2 == 0)
		MOVINGSOLID::create_moving_solid(parts, pmap, i);

	int px = (int)(parts[i].x + 0.5);
	int py = (int)(parts[i].y + 0.5);

	/** 
	 * Failed to find group, flood fill current state id to 0 and
	 * recreate the solid. This happens sometimes during undo-redo operations
	 * (maybe redo/undo changes particle ids?) Also if flags = 1 redo, there
	 * is a chance the solid was cut
	 */
	if (MOVINGSOLID::solids.count(parts[i].tmp2) == 0 || parts[i].flags == 1) {
		MVSD::reset(px, py, parts, pmap, parts[i].tmp2);
		MOVINGSOLID::create_moving_solid(parts, pmap, i);

		// If the solid was cut, we'll need to set its velocity to the original
		if (parts[i].flags == 1) {
			MOVINGSOLID::solids[parts[i].tmp2].set_velocity(parts[i].pavg[0], parts[i].pavg[1]);
			parts[i].pavg[0] = parts[i].pavg[1] = 0.0f;
		}

		parts[i].flags = 0;
		return 0;
	}


	/* Ctype is not a solid */
	if (!(sim->elements[parts[i].ctype].Properties & TYPE_SOLID))
		parts[i].ctype = 0;

	/* Basic ctype mimicing */
	if (parts[i].ctype) {
		// Melt if ctype allows it
		if (sim->elements[parts[i].ctype].HighTemperatureTransition != NT
				&& parts[i].temp > sim->elements[parts[i].ctype].HighTemperature) {
			int temp = sim->create_part(-3, x, y, sim->elements[parts[i].ctype].HighTemperatureTransition);
			parts[temp].ctype = parts[i].ctype;
			parts[temp].temp = parts[i].temp;
			parts[temp].vx = MOVINGSOLID::solids[parts[i].tmp2].getVX();
			parts[temp].vy = MOVINGSOLID::solids[parts[i].tmp2].getVY();
			sim->kill_part(i);
			return 0;
		}

		// Shatter depending on pressure transition
	}



	int r, rx, ry;

	// TODO remove
	if (parts[i].tmp == 3) parts[i].tmp = 0;






	// Check surrounding particles. These are "touching"
	// collisions, as in the solid is flush with another particle
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry)) {
				// Allow wall collisions
				if (sim->IsWallBlocking(x + rx, y + ry, PT_MVSD))
					MOVINGSOLID::solids[parts[i].tmp2].add_collision(
						MOVINGSOLID::Collision(x + rx, y + ry, MOVINGSOLID::STATIC, px, py, x + rx, y + ry));

				r = pmap[y+ry][x+rx];
				if (!r) {
					continue;
				}
				int rt = TYP(r);

				// Collision with another moving solid
				if (rt == PT_MVSD && parts[ID(r)].tmp2 != parts[i].tmp2) {
					MOVINGSOLID::solids[parts[i].tmp2].add_collision(
						MOVINGSOLID::Collision(x + rx, y + ry, MOVINGSOLID::MOVING, px, py, x + rx, y + ry));
				}

				else if (rt != PT_MVSD) {
					parts[i].tmp = 3; // TODO deco
					if (sim->elements[rt].Properties & TYPE_SOLID)
						MOVINGSOLID::solids[parts[i].tmp2].add_collision(
							MOVINGSOLID::Collision(x + rx, y + ry, MOVINGSOLID::STATIC, px, py, x + rx, y + ry));
					else if (sim->elements[rt].Properties & TYPE_PART)
						MOVINGSOLID::solids[parts[i].tmp2].add_collision(
							MOVINGSOLID::Collision(x + rx, y + ry, MOVINGSOLID::NONSTATIC, px, py, x + rx, y + ry));
				}
			}

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
						MOVINGSOLID::Collision((int)sx, (int)sy, 
						sim->elements[TYP(r)].Properties & TYPE_PART ? MOVINGSOLID::NONSTATIC : MOVINGSOLID::STATIC,
						px, py, (int)round(sx), (int)round(sy)));

					int dx = (int)round(sx) - px;
					int dy = (int)round(sy) - py;

					if (dx != 0) dx += dx < 0 ? 1 : -1;
					if (dy != 0) dy += dy < 0 ? 1 : -1;
						
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

	return 0;
}

//#TPT-Directive ElementHeader Element_MVSD static int graphics(GRAPHICS_FUNC_ARGS)
int Element_MVSD::graphics(GRAPHICS_FUNC_ARGS) {
	// Mimic ctype color
	if (cpart->ctype) {
		*colr = PIXR(ren->sim->elements[cpart->ctype].Colour);
		*colg = PIXG(ren->sim->elements[cpart->ctype].Colour);
		*colb = PIXB(ren->sim->elements[cpart->ctype].Colour);
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
