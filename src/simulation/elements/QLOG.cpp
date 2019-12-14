#include "simulation/ElementCommon.h"
#include "simulation/quantum/def.h"
#include "simulation/quantum/qmath.h"
#include "simulation/quantum/linalg.h"
#include "simulation/quantum/quantum.h"

#include <iostream>

//#TPT-Directive ElementClass Element_QLOG PT_QLOG 192
Element_QLOG::Element_QLOG()
{
	Identifier = "DEFAULT_PT_QLOG";
	Name = "QLOG";
	Colour = PIXPACK(0xCF3A3A);
	MenuVisible = 1;
	MenuSection = SC_ELEC;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 0;
	Description = "Quantum Logic Gate. Fires a beam that alters ION like ARAY. Ctype determines logic gate type, see github wiki.";

	Properties = TYPE_SOLID;

	// Stores IDs of other logic gates
	DefaultProperties.pavg[0] = -1;
	DefaultProperties.pavg[1] = -1;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	DefaultProperties.life = 0;

	Update = &Element_QLOG::update;
	Graphics = &Element_QLOG::graphics;
}

//#TPT-Directive ElementHeader Element_QLOG static int update_scan(Particle *parts, int pmap[YRES][XRES], int i, int tx, int ty, bool multi_check)
int Element_QLOG::update_scan(Particle *parts, int pmap[YRES][XRES], int i, int tx, int ty, bool multi_check) {
	// Func when sparking, sets life and determines a gate's other inputs
	parts[ID(pmap[ty][tx])].life = QGATE::COOLDOWN;
	if (multi_check && parts[ID(pmap[ty][tx])].tmp == parts[i].tmp) {
		if (parts[ID(pmap[ty][tx])].tmp2 == 1 && parts[i].pavg[0] < 0)
			parts[i].pavg[0] = ID(pmap[ty][tx]);
		else if (parts[ID(pmap[ty][tx])].tmp2 == 2 && parts[i].pavg[1] < 0)
			parts[i].pavg[1] = ID(pmap[ty][tx]);
	}
	return 0;
}

//#TPT-Directive ElementHeader Element_QLOG static int find_ion(Particle *parts, int pmap[YRES][XRES], int i)
int Element_QLOG::find_ion(Particle *parts, int pmap[YRES][XRES], int i) {
	int dx, dy; dx = dy = 0;
	int tx = parts[i].x;
	int ty = parts[i].y;
	if (parts[i].tmp == 0 || parts[i].tmp == 2)
		dx = parts[i].tmp == 0 ? -1 : 1;
	else dy = parts[i].tmp == 1 ? -1 : 1;

	while (ty >= 0 && ty < YRES && tx >= 0 && tx < XRES &&
			(!pmap[ty][tx] || TYP(pmap[ty][tx]) != PT_ION)) {
		tx += dx;
		ty += dy;
	}

	// Couldn't find an ION
	if (!pmap[ty][tx] || TYP(pmap[ty][tx]) != PT_ION)
		return -1;
	return ID(pmap[ty][tx]);
}

//#TPT-Directive ElementHeader Element_QLOG static int update(UPDATE_FUNC_ARGS)
int Element_QLOG::update(UPDATE_FUNC_ARGS) {
	/* 
	 * Properties
     * - life: is beam firing
	 * - tmp: beam direction
	 * - tmp2: input number (for multi gate inputs, 0 indexed)
	 * - ctype: logic gate type
	 * 
	 * For multi-input gates (up to 3 inputs)
	 * - pavg[0], pavg[1]: ID(r) of the other 2 gate inputs (Assumes self is input 0)
	 */
	int rx, ry, r;

    /* When firing the beam searches for any ION in a straight line to modify. Saves distance in flags */
	if (!QGATE::does_nothing(parts[i].ctype) && parts[i].life == QGATE::COOLDOWN) {
		parts[i].life--;
		parts[i].flags = -1;

		// Find the ion
		int ION = find_ion(parts, pmap, i);
		std::vector<int> IONS;
		IONS.push_back(ION);

		LINALG::matrix XGATE;
		XGATE.push_back(LINALG::vec({0, 1}));
		XGATE.push_back(LINALG::vec({1, 0}));
		QUANTUM::apply_gate(XGATE, parts, std::vector<int>(1, ION));


		// -- Actual gate logic begins here -- \\
		// ----------------------------------- \\

		// Single gate, acts on the particle
		if (!QGATE::is_multi_gate(parts[i].ctype)) {
			switch(parts[i].ctype) {
				case QGATE::X: {
					break;
				}
				case QGATE::Y: {
					break;
				}
				case QGATE::Z: {
					break;
				}
				case QGATE::H: {
					break;
				}
				case QGATE::S: {
					break;
				}
				case QGATE::SINV: {
					break;
				}
				case QGATE::T: {
					break;
				}
				case QGATE::TINV: {
					break;
				}
				case QGATE::SQRTNOT: {
					break;
				}

				case QGATE::DETECT: {
					int spin = QMATH::observe(parts[ION].pavg[0], parts[ION].pavg[1]);
					if (spin == 0) goto end_logic; // Don't spark if spindown

					// SPRK nearby metal
					for (rx = -1; rx <= 1; rx++)
					for (ry = -1; ry <= 1; ry++)
						if (BOUNDS_CHECK && (rx || ry)) {
							r = pmap[y + ry][x + rx];
							if (!r) continue;
							if (!(sim->elements[TYP(r)].Properties&PROP_CONDUCTS))
								continue;

							parts[ID(r)].life = 4;
							parts[ID(r)].ctype = TYP(r);
							sim->part_change_type(ID(r), x + rx, y + ry, PT_SPRK);
						}
					break;
				}
				default: {
					break;
				}
			}
		}
		// Multi gate
		else {
			// Only multi gate input 0 actually does anything
			if (parts[i].tmp2 > 0)
				return 0;

			// Failed to find a 2ND input (All checks must be present as element might have moved)
			// or been deleted since last frame
			if (parts[i].pavg[0] == -1 || parts[(int)(parts[i].pavg[0])].type != PT_QLOG ||
					parts[i].ctype != parts[(int)(parts[i].pavg[0])].ctype ||
					parts[i].tmp != parts[(int)(parts[i].pavg[0])].tmp ||
					parts[(int)(parts[i].pavg[0])].tmp2 != 1)
				goto end_logic;

			// Self = Input 0
			// pavg[0] = Input 1
			// pavg[1] = Input 2 (Not calculated unless needed)
			int ION1 = find_ion(parts, pmap, parts[i].pavg[0]);

			// 2 Input gates
			switch(parts[i].ctype) {
				case QGATE::SWAP: {
					std::swap(parts[ION].pavg[0], parts[ION1].pavg[1]);
					goto end_logic;
				}
				case QGATE::CNOT: {
					goto end_logic;
				}
				default: break;
			}

			// 3 INPUT gates
			int ION2 = find_ion(parts, pmap, parts[i].pavg[1]);

			// Failed to find a 3RD input (All checks must be present as element might have moved)
			// or been deleted since last frame
			if (parts[i].pavg[1] == -1 || parts[(int)(parts[i].pavg[1])].type != PT_QLOG ||
					parts[i].ctype != parts[(int)(parts[i].pavg[1])].ctype ||
					parts[i].tmp != parts[(int)(parts[i].pavg[1])].tmp ||
					parts[(int)(parts[i].pavg[1])].tmp2 != 2)
				goto end_logic;

			switch(parts[i].ctype) {
				
				default: break;
			}
		}

		end_logic:

		// Reset pavg
		parts[i].pavg[0] = -1;
		parts[i].pavg[1] = -1;

		return 0; // Stop now since we already fired and updated
	} else if (parts[i].life > 0) {
		parts[i].life--;
		parts[i].flags = -1;
		return 0;
	}


	/* Beam directions: tmp = 0: left, 1 = up, 2 = right, 3 = down */
	int dir = parts[i].tmp;
	bool multi_check = QGATE::is_multi_gate(parts[i].ctype); // Multi gate check

	for (rx = -1; rx <= 1; rx++)
		for (ry = -1; ry <= 1; ry++)
			if (BOUNDS_CHECK && (rx || ry)) {
				r = pmap[y + ry][x + rx];
				if (!r) continue;
				
				/* If it detects a spark, fire in the direction of tmp and activates any QLOG
				 * in an orthagonal direction to the beam. SPRK cannot be detected diagonally. */
				if (!(rx && ry) && parts[ID(r)].type == PT_SPRK) {
					int tx = parts[i].x;
					int ty = parts[i].y;

					if (dir == 0 || dir == 2) { // Search vertical
						ty++;
						while (ty >= 0 && ty < YRES && parts[ID(pmap[ty][tx])].type == PT_QLOG) {
							update_scan(parts, pmap, i, tx, ty, multi_check);
							ty++;
						}
						ty = parts[i].y - 1;
						while (ty >= 0 && ty < YRES && parts[ID(pmap[ty][tx])].type == PT_QLOG) {
							update_scan(parts, pmap, i, tx, ty, multi_check);
							ty--;
						}
					}
					else { // Search horizontal
						tx++;
						while (tx >= 0 && tx < XRES && parts[ID(pmap[ty][tx])].type == PT_QLOG) {
							update_scan(parts, pmap, i, tx, ty, multi_check);
							tx++;
						}
						tx = parts[i].x - 1;
						while (tx >= 0 && tx < XRES && parts[ID(pmap[ty][tx])].type == PT_QLOG) {
							update_scan(parts, pmap, i, tx, ty, multi_check);
							tx--;
						}
					}
					parts[i].life = QGATE::COOLDOWN; // Set self life
				}
			}

	return 0;
}

//#TPT-Directive ElementHeader Element_QLOG static int graphics(GRAPHICS_FUNC_ARGS)
int Element_QLOG::graphics(GRAPHICS_FUNC_ARGS) {
	// Vary color based on ctype
	// TODO maybe pre-make array of colors
	if (cpart->ctype % 2 == 0) *colb += 30 * cpart->ctype;
	else *colg += 30 * cpart->ctype;

	// Draw beam if it should
	if (cpart->life <= 0) return 0;
	
	// Brighten when triggered
	*colr = (int)(*colr * 1.5);
	*colg = (int)(*colg * 1.5);
	*colb = (int)(*colb * 1.5);

	return 0;
}

Element_QLOG::~Element_QLOG() {}
