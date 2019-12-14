#include "simulation/ElementCommon.h"
#include <cmath>

// Ball lightning
// Invisible until it charges up with energy and glows a brilliant white (Depends on how much it was charged)
// It's energy output is electricity like LIGH/SPRK
// Emits a circle with same color
// Goes against air flow and after random amount of time, will explode
// Explosion will destroy only it's ctyle nearby replacing it with something that glows then turns into ash


//#TPT-Directive ElementClass Element_BALI PT_BALI 195
Element_BALI::Element_BALI()
{
	Identifier = "DEFAULT_PT_BALI";
	Name = "BALI";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
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
	Meltable = 1;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 251;
	Description = "Ball Lightning. Actually a macroelectron";

	Properties = PROP_CONDUCTS|PROP_HOT_GLOW;

	// element properties here

	Update = &Element_BALI::update;
	Graphics = &Element_BALI::graphics;
}

//#TPT-Directive ElementHeader Element_BALI static int update(UPDATE_FUNC_ARGS)
int Element_BALI::update(UPDATE_FUNC_ARGS)
{

	return 0;
}

//#TPT-Directive ElementHeader Element_BALI static int graphics(GRAPHICS_FUNC_ARGS)
int Element_BALI::graphics(GRAPHICS_FUNC_ARGS)
{
	// graphics code here
	// return 1 if nothing dymanic happens here
	*colr = *colg = *colb = 0;
	*cola = 255; // Black
	if (cpart->life >= 380 && cpart->life < 450) {
		// color lol
	} else if (cpart->life >= 450 && cpart->life < 485) {
		*colr = 128;
		*colb = 128;
	} else if (cpart->life >= 485 && cpart->life < 500) {
		*colg = 255;
		*colb = 255;
	} else if (cpart->life >= 500 && cpart->life < 565) {
		*colg = 255;
	} else if (cpart->life >= 565 && cpart->life < 590) {
		*colr = 255;
		*colg = 255;
	} else if (cpart->life >= 590 && cpart->life < 625) {
		*colr = 255;
		*colg = 128;
	} else if (cpart->life >= 625 && cpart->life < 740) {
		*colr = 255;
	}
	// (cx + rcos(theta), cy + rsin(theta))
	int radius = 6;
	for (float angle = 0.0f; angle < 6.28f; angle += 0.1f) {
		ren->addpixel(nx + (radius * cos(angle)) + 0.5f, ny + (radius * sin(angle)) + 0.5f, *colr, *colg, *colb, 96);
	}
	*colr = *colg = *colb = *cola = 0;
	return 0;
}

Element_BALI::~Element_BALI() {}
