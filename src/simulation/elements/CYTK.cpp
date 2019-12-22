#include "simulation/ElementCommon.h"
#include "simulation/Cybertruck.h"
#include <iostream>

//#TPT-Directive ElementClass Element_CYTK PT_CYTK 190
Element_CYTK::Element_CYTK()
{
	Identifier = "DEFAULT_PT_CYTK";
	Name = "CYTK";
	Colour = PIXPACK(0x000077);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	Advection = 0.01f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.15f; // NOTE: CYBERTRUCK UPDATES TWICE PER FRAME
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 0;
	Description = "Tesla Cybertruck.";

	Properties = TYPE_PART | PROP_NOCTYPEDRAW;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_CYTK::update;
	Graphics = &Element_CYTK::graphics;
}

//#TPT-Directive ElementHeader Element_CYTK static bool check_stkm(playerst *data, Simulation *sim, Particle *parts, int i)
bool Element_CYTK::check_stkm(playerst *data, Simulation *sim, Particle *parts, int i) {
	int xdiff = abs(data->legs[0] - parts[i].x);
	int ydiff = abs(data->legs[1] - parts[i].y);
	if (xdiff + ydiff < 10) {
		if (data->elem == PT_FIRE) // Flamethrower
			parts[i].tmp = 2;
		else if (data->rocketBoots) // Cyberthruster
			parts[i].tmp = 1;
		sim->kill_part(data->stkmID);
		return true;
	}
	return false;
}

//#TPT-Directive ElementHeader Element_CYTK static bool attempt_move(int dx, int dy, Simulation *sim, Particle *parts, int i)
bool Element_CYTK::attempt_move(int x, int y, Simulation *sim, Particle *parts, int i) {
	if (parts[i].vx == 0 && parts[i].vy == 0)
		return false;

	rotate(x, y, parts[i].pavg[0]);
	x += parts[i].x;
	y += parts[i].y;

	int r, rx, ry;

	// Test surrounding particles
	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			if (BOUNDS_CHECK) {
				if (sim->IsWallBlocking(x + rx, y + ry, PT_CYTK)) {
					parts[i].vx = parts[i].vy = 0;
					return true;
				}

				r = sim->pmap[y + ry][x + rx];
				if (!r) continue;
				if (TYP(r) != PT_CYTK &&
					(sim->elements[TYP(r)].Properties & TYPE_PART ||
					 sim->elements[TYP(r)].Properties & TYPE_SOLID)) {
					parts[i].vx = parts[i].vy = 0;
					return true;
				}
			}

	// Try moving in the direction of vx vy
	float largest = fabs(parts[i].vx) > fabs(parts[i].vy) ? fabs(parts[i].vx) : fabs(parts[i].vy);
	if (largest <= 1) { // Avoid "division by 0" effect where it ends up scanning the entire map
		return false;
	}
	float dvx = parts[i].vx / largest, dvy = parts[i].vy / largest;
	float sx = x, sy = y;
	int count = 1;

	while (sx >= 0 && sy >= 0 && sx < XRES && sy < YRES &&
			fabs(-dvx + dvx * count) <= fabs(parts[i].vx) && fabs(-dvy + dvy * count) <= fabs(parts[i].vy)) {
		r = sim->pmap[(int)round(sy)][(int)round(sx)];
		if (r) {
			if (TYP(r) != PT_CYTK &&
				(sim->elements[TYP(r)].Properties & TYPE_PART ||
				sim->elements[TYP(r)].Properties & TYPE_SOLID)) {
				parts[i].vx = parts[i].vy = 0;
				return true;
			}
		}

		sx += dvx; sy += dvy;
		count++;
	}
	return false;
}

//#TPT-Directive ElementHeader Element_CYTK static int create_part(Simulation *sim, int x, int y, int type, float theta, Particle *parts, int i)
int Element_CYTK::create_part(Simulation *sim, int x, int y, int type, float theta, Particle *parts, int i) {
	if (parts[i].pavg[1])
		x = -x; // Car going other way
	rotate(x, y, theta);
	x += parts[i].x; y += parts[i].y;
	return sim->create_part(-1, x, y, type);
}

//#TPT-Directive ElementHeader Element_CYTK static int update(UPDATE_FUNC_ARGS)
int Element_CYTK::update(UPDATE_FUNC_ARGS) {
	// NOTE: CYBERTRUCK UPDATES TWICE PER FRAME

	/**
	 * Properties:
	 * vx, vy (velocity)
	 * tmp2 = which STKM controls it (1 = STKM, 2 = STK2, 3 = AI car)
	 * tmp = rocket or flamethrower (0 none, 1 rocket, 2 flamethrower)
	 * temp = damage
	 * life = charge
	 * pavg[0] = rotation
	 * pavg[1] = direction of travel (left or right)
	 * 
	 * If touched by a FIGH the FIGH will attempt to use the cybertruck to run down
	 * the STKM and STK2
	 */

	// Collision checking and update
	// We'll consider moving in the direction of velocity
	// We only check collisions with 6 points on the rectangle that
	// bounds the cybertruck
	bool pbl = attempt_move(-CYBERWIDTH / 2, CYBERHEIGHT / 2, sim, parts, i);
	bool pbr = attempt_move(CYBERWIDTH / 2, CYBERHEIGHT / 2, sim, parts, i);
	bool pbc = attempt_move(0, CYBERHEIGHT / 2, sim, parts, i);
	bool tbl = attempt_move(-CYBERWIDTH / 2, -CYBERHEIGHT / 2, sim, parts, i);
	bool tbr = attempt_move(CYBERWIDTH / 2, -CYBERHEIGHT / 2, sim, parts, i);
	bool tbc = attempt_move(0, -CYBERHEIGHT / 2, sim, parts, i);
	bool has_collision = pbl || pbr || pbc || tbl || tbr || tbc;

	// Match terrain rotation
	if (pbl ^ pbr && !pbc) {
		parts[i].pavg[0] += pbl ? CYBER_ROTATION_SPEED : -CYBER_ROTATION_SPEED;
		parts[i].y -= 0.5;
	}
	if (tbl ^ tbr && !tbc) {
		parts[i].pavg[0] += tbl ? CYBER_ROTATION_SPEED : -CYBER_ROTATION_SPEED;
		parts[i].y += 0.5;
	}

	// If no collision rotate towards gravity every 10 frames
	if (!has_collision && sim->timer % 10 == 0) {
		float target_angle = -200.0f; // Placeholder
		switch (sim->gravityMode) {
		default:
		case 0: // Normal, vertical gravity
			target_angle = 0;
			break;
		case 1: // No gravity
			break;
		case 2: // Radial gravity
			target_angle = atan2(parts[i].y - YRES / 2, parts[i].x - XRES / 2) + 3.1415 / 2;
			break;
		}

		if (target_angle == -200.0f) {}
		else if (parts[i].pavg[0] > target_angle)
			parts[i].pavg[0] -= CYBER_ROTATION_SPEED;
		else
			parts[i].pavg[0] += CYBER_ROTATION_SPEED;
	}

	// If sim stkm or stkm2 exists then stop allowing control to this car
	if (parts[i].tmp2 == 1 && sim->player.spwn)
		parts[i].tmp2 = 0;
	else if (parts[i].tmp2 == 2 && sim->player2.spwn)
		parts[i].tmp2 = 0;

	// Prevent spawning of stkm and stkm2 if inside car
	if (parts[i].tmp2 == 1) sim->cybertruck_p1 = true;
	if (parts[i].tmp2 == 2) sim->cybertruck_p2 = true;

	// Player controls
	if (parts[i].tmp2 == 1 || parts[i].tmp2 == 2) {
		int command = parts[i].tmp2 == 1 ? sim->player.comm : sim->player2.comm;
		if (has_collision || parts[i].tmp == 1) { // Accelerating only can be done on ground or if rocket
			float vx = has_collision ? -CYBERSPEED : -CYBERFLYSPEED / 8.0f, vy = 0.0f;

			if (((int)command & 0x01) == 0x01) { // Left
				rotate(vx, vy, parts[i].pavg[0]);
				parts[i].vx += vx;
				parts[i].vy += vy;
				parts[i].pavg[1] = 0; // Set face direction
				parts[i].y -= 0.5;
			}
			if (((int)command & 0x02) == 0x02) { // Right
				vx *= -1;
				rotate(vx, vy, parts[i].pavg[0]);
				parts[i].vx += vx;
				parts[i].vy += vy;
				parts[i].pavg[1] = 1; // Set face direction
				parts[i].y -= 0.5;
			}

			// Limit max speed
			if (parts[i].vx > MAX_CYBER_SPEED)
				parts[i].vx = MAX_CYBER_SPEED;
			else if (parts[i].vx < -MAX_CYBER_SPEED)
				parts[i].vx = -MAX_CYBER_SPEED;
			if (parts[i].vy > MAX_CYBER_SPEED)
				parts[i].vy = MAX_CYBER_SPEED;
			else if (parts[i].vy < -MAX_CYBER_SPEED)
				parts[i].vy = -MAX_CYBER_SPEED;
		}

		if (((int)command & 0x04) == 0x04) { // Exit (up)
			// Flag that cybertruck has no stkm
			// Make sure new spawn location isn't too close or STKM
			// will instantly re-enter
			if (parts[i].tmp2 == 1) {
				sim->cybertruck_p1 = false;
				parts[i].tmp2 = 0;
				sim->player.stkmID = sim->create_part(-1, x - 15, y - 5, PT_STKM);
				return 0;
			}
			else if (parts[i].tmp2 == 2) {
				sim->cybertruck_p2 = false;
				parts[i].tmp2 = 0;
				sim->player2.stkmID = sim->create_part(-1, x - 15, y - 5, PT_STKM2);
				return 0;
			}
		}
		if (((int)command & 0x08) == 0x08) { // Fly or shoot (down)
			if (parts[i].tmp == 1) { // Rocket
				parts[i].vy = -CYBERFLYSPEED; // TODO respect gravity

				int j1 = create_part(sim, CYBERWIDTH * 0.4f, CYBERHEIGHT / 2, PT_PLSM, parts[i].pavg[0], parts, i);
				int j2 = create_part(sim, -CYBERWIDTH * 0.4f, CYBERHEIGHT / 2, PT_PLSM, parts[i].pavg[0], parts, i);
				parts[j1].temp = parts[j2].temp = 1000.0f;
				parts[j1].life = rand() % 100 + 50;
				parts[j2].life = rand() % 100 + 50;
			}
			else if (parts[i].tmp == 2) { // Flamethrower
				int j = create_part(sim, -CYBERWIDTH * 0.4f, -CYBERHEIGHT / 2, PT_BCOL, parts[i].pavg[0], parts, i);
				parts[j].life = rand() % 100 + 50;
				parts[j].vx = parts[i].pavg[1] ? 15 : -15;
				parts[j].vy = -(rand() % 3 + 3);
				rotate(parts[j].vx, parts[j].vy, parts[i].pavg[0]);
			}
		}
	}
	
	if (parts[i].tmp2 > 2) {

	}

	// Check for nearby STKM and FIGH
	if (!sim->cybertruck_p1 && parts[i].tmp2 == 0 && sim->player.spawnID >= 0 && check_stkm(&sim->player, sim, parts, i)) {
		parts[i].tmp2 = 1;
		sim->cybertruck_p1 = true;
	}
	if (!sim->cybertruck_p2 && parts[i].tmp2 == 0 && sim->player2.spawnID >= 0 && check_stkm(&sim->player2, sim, parts, i)) {
		parts[i].tmp2 = 2;
		sim->cybertruck_p2 = true;
	}
	if (parts[i].tmp2 == 0) {
		for (unsigned char j = 0; j < sim->fighcount; ++j) {
			if (check_stkm(&sim->fighters[j], sim, parts, i)) {
				parts[i].tmp2 = 3;
				break;
			}
		}
	}
	
	return 0;
}

//#TPT-Directive ElementHeader Element_CYTK static int graphics(GRAPHICS_FUNC_ARGS)
int Element_CYTK::graphics(GRAPHICS_FUNC_ARGS) {
	// The cybertruck is 46px wide and 14px tall
	draw_cybertruck(ren, cpart, cpart->pavg[0]);
	return 0;
}

Element_CYTK::~Element_CYTK() {}
