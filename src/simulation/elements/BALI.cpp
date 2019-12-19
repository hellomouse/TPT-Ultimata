#include "simulation/ElementCommon.h"
#include <cmath>
#include "common/tpt-rand.h"

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
	const int wavelengths[8][4] = {
		{380, 128, 0, 128},
		{450, 0, 0, 255},
		{485, 0, 128, 128},
		{500, 0, 255, 0},
		{565, 128, 128, 0},
		{590, 255, 165, 0},
		{625, 255, 0, 0},
		{0, 0, 0, 0}
	};

	void findBoundary(int life, int& colr, int& colg, int& colb) {
		float lifediff;
		if (life < 380) return;
		if (life > 625) {
			colr = colg = colb = 255;
			return;
		}
		for (unsigned int i = 0; i < 7; ++i) {
			if (life > wavelengths[i][0]) continue;

			lifediff = wavelengths[i + 1][0] - wavelengths[i][0];
			lifediff /= (float)wavelengths[i][0];
			colr = (int)((wavelengths[i + 1][1] * lifediff) + (wavelengths[i][1] * (1 - lifediff)));
			colg = (int)((wavelengths[i + 1][2] * lifediff) + (wavelengths[i][2] * (1 - lifediff)));
			colb = (int)((wavelengths[i + 1][3] * lifediff) + (wavelengths[i][3] * (1 - lifediff)));
			break;
		}

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
	Diffusion = 0.00f;
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
	Description = "Ball Lightning. Actually a macroelectron";
	Properties = PROP_CONDUCTS | PROP_HOT_GLOW;

	// element properties here

	Update = &Element_BALI::update;
	Graphics = &Element_BALI::graphics;
	Create = &Element_BALI::create;
}

//#TPT-Directive ElementHeader Element_BALI static void create(ELEMENT_CREATE_FUNC_ARGS)
void Element_BALI::create(ELEMENT_CREATE_FUNC_ARGS) {
	sim->parts[i].tmp = RNG::Ref().between(0, 9999);
	sim->parts[i].life = RNG::Ref().between(380, 625);
}

//#TPT-Directive ElementHeader Element_BALI static int update(UPDATE_FUNC_ARGS)
int Element_BALI::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, id;
	parts[i].tmp--;

	if (parts[i].tmp == 0) {

	}

	if (parts[i].life >= 0 && RNG::Ref().chance(1, 100)) {
		for (rx = -3; rx <= 3; rx++)
			for (ry = -3; ry <= 3; ry++)
				if (BOUNDS_CHECK && (rx || ry)) {
					r = pmap[y + ry][x + rx];
					if (!r) continue;
					if (sim->elements[ID(r)].Properties & PROP_CONDUCTS) {
						parts[ID(r)].life = 4;
						parts[ID(r)].ctype = ID(r);
						sim->part_change_type(ID(r), x + rx, y + ry, PT_SPRK);
					}
				}
		parts[i].life--;
		return 0;
	}

	for (rx = -1; rx <= 1; rx++)
		for (ry = -1; ry <= 1; ry++)
			if (BOUNDS_CHECK && (rx || ry)) {
				r = pmap[y + ry][x + rx];
				if (!r) continue;
				id = ID(r);

				if (TYP(r) == PT_SPRK || TYP(r) == PT_LIGH) {
					parts[i].life++;
				}
				else if (TYP(r) == PT_BALI) {
					parts[i].life += parts[id].life;
					sim->kill_part(id);
				}
			}

	return 0;
}

//#TPT-Directive ElementHeader Element_BALI static int graphics(GRAPHICS_FUNC_ARGS)
int Element_BALI::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here
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
