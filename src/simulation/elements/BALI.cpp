#include "simulation/ElementCommon.h"
#include <cmath>

// SPOILERS
// Do not read this element code unless
// you want to be spoiled for Liu Cixin's Ball Lightning (novel)

// Ball lightning
// Invisible until it charges up with energy and glows a brilliant white (Depends on how much it was charged)
// It's energy output is electricity like LIGH/SPRK
// Emits a circle with same color
// Goes against air flow and after random amount of time, will explode
// Explosion will destroy only it's ctyle nearby replacing it with something that glows then turns into ash
namespace BALI_DATA {
	// Marker life value, r, g, b
	// For example, if life = 400, the color would be eased between
	// the color at life = 380 and life = 450
	const int wavelengths[9][4] = {
		{0, 0, 0, 0},
		{380, 128, 0, 128},
		{450, 0, 0, 255},
		{485, 0, 128, 128},
		{500, 0, 255, 0},
		{565, 128, 128, 0},
		{590, 255, 165, 0},
		{625, 255, 0, 0},
		{825, 255, 255, 255}
	};

	void findBoundary(int life, int& colr, int& colg, int& colb) {
		float lifediff;
		int rdiff, bdiff, gdiff;

		for (unsigned int i = 0; i < 8; ++i) {
			if (life > wavelengths[i][0] && life > wavelengths[i + 1][0]) continue; // Stop at first life we're between 2 boundaries

			// Linear easing, we take the range between the 2 colors
			// and do a proportional averaging, ie, if:
			// 		life = 100 and r1 = 255
			// 		life = 200 and r2 = 100
			// and life is currently 125 (25% from 255 to 0)
			// we compute new life value by taking (life - min) / range * (rdiff) + r2
			// (125 - 100) / (200 - 100) * (100 - 255) + 255
			// = 0.25 * -155 + 255 = 216.75
			lifediff = wavelengths[i + 1][0] - wavelengths[i][0];
			rdiff = wavelengths[i + 1][1] - wavelengths[i][1];
			gdiff = wavelengths[i + 1][2] - wavelengths[i][2];
			bdiff = wavelengths[i + 1][3] - wavelengths[i][3];

			colr = (life - wavelengths[i][0]) / lifediff * rdiff + wavelengths[i][1];
			colg = (life - wavelengths[i][0]) / lifediff * gdiff + wavelengths[i][2];
			colb = (life - wavelengths[i][0]) / lifediff * bdiff + wavelengths[i][3];
			return;
		}

		// It's white now
		colr = colg = colb = 255;
	}
}

//#TPT-Directive ElementClass Element_BALI PT_BALI 195
Element_BALI::Element_BALI()
{
	Identifier = "DEFAULT_PT_BALI";
	Name = "BALI";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	Advection = -0.7f;
	AirDrag = -0.01f * CFDS;
	AirLoss = 0.96f;
	Loss = 0.5f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.50f;
	HotAir = 0.000f * CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 30;

	Weight = 100;

	Flammable = 0;
	Explosive = 0;
	Meltable = 1;
	Hardness = 1;

	Weight = 1;
	HeatConduct = 251;
	Description = "Ball Lightning. Actually a macroelectron, releases energy onto its ctype.";
	Properties = TYPE_ENERGY;

	Update = &Element_BALI::update;
	Graphics = &Element_BALI::graphics;
	Create = &Element_BALI::create;
}

//#TPT-Directive ElementHeader Element_BALI static void create(ELEMENT_CREATE_FUNC_ARGS)
void Element_BALI::create(ELEMENT_CREATE_FUNC_ARGS) {
	sim->parts[i].tmp = RNG::Ref().between(600, 4999);
	sim->parts[i].life = RNG::Ref().between(380, 625);
}

int Element_BALI::update(UPDATE_FUNC_ARGS) {
	/**
	 * Properties:
	 * life = energy level
	 * ctype = target element to act upon
	 * tmp = Time before explosion
	 */
	int r, rx, ry;
	float randInt = RNG::Ref().uniform01();

	parts[i].tmp--;
	if (parts[i].tmp == 0) {

	}

	// if (sim->timer % 5 == 0)
		parts[i].life++;

	if (parts[i].life >= 0) {
		for (rx = -3; rx <= 3; rx++)
			for (ry = -3; ry <= 3; ry++)
				if (BOUNDS_CHECK && (rx || ry)) {
					r = pmap[y + ry][x + rx];
					if (!r) r = sim->photons[y + ry][x + rx];
					if (!r) continue;

					// Electric elements increase BALI's energy level
					if (TYP(r) == PT_SPRK || TYP(r) == PT_LIGH || TYP(r) == PT_THDR || TYP(r) == PT_ELEC) {
						parts[i].life++;
					}

					// BALI absorbs other BALI
					if (TYP(r) == PT_BALI) {
						parts[i].life += parts[ID(r)].life;
						sim->kill_part(ID(r));
						return 0;
					}

					// Randomly spark elements if there are
					if (randInt < 0.01) {
						if (sim->elements[ID(r)].Properties & PROP_CONDUCTS) {
							parts[ID(r)].life = 4;
							parts[ID(r)].ctype = ID(r);
							sim->part_change_type(ID(r), x + rx, y + ry, PT_SPRK);
						}
					}
				}
	}
	return 0;
}

//#TPT-Directive ElementHeader Element_BALI static int graphics(GRAPHICS_FUNC_ARGS)
int Element_BALI::graphics(GRAPHICS_FUNC_ARGS) {
	*colr = *colg = *colb = 0;
	*cola = 255; // Black

	BALI_DATA::findBoundary(cpart->life, *colr, *colg, *colb);

	// (cx + rcos(theta), cy + rsin(theta))
	int radius = 6;
	for (float angle = 0.0f; angle < 6.28f; angle += 0.1f) {
		ren->addpixel(nx + (radius * cos(angle)) + 0.5f, ny + (radius * sin(angle)) + 0.5f, *colr, *colg, *colb, 96);
	}
	*colr = *colg = *colb = *cola = 0;
	return 0;
}

Element_BALI::~Element_BALI() {}
