#include "simulation/ElementCommon.h"

//#TPT-Directive ElementClass Element_FFGN PT_FFGN 205
Element_FFGN::Element_FFGN()
{
	Identifier = "DEFAULT_PT_FFGN";
	Name = "FFGN";
	Colour = PIXPACK(0xFFCC00);
	MenuVisible = 1;
	MenuSection = SC_POWERED;
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
	
	DefaultProperties.temp = 100.0f;
	HeatConduct = 0;
	Description = "Force field generator. Generates field based on ctype";

	Properties = TYPE_SOLID | PROP_NOAMBHEAT | PROP_NOCTYPEDRAW;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = 100.8f;
	HighPressureTransition = PT_BREC;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITL;
	HighTemperatureTransition = NT;

	Update = &Element_FFGN::update;
	Graphics = &Element_FFGN::graphics;
	CtypeDraw = &Element::ctypeDrawVInTmp;
}

//#TPT-Directive ElementHeader Element_FFGN static void flood_fill_FFLD(Particle *parts, Simulation *sim, int i, int x, int y)
void Element_FFGN::flood_fill_FFLD(Particle *parts, Simulation *sim, int i, int x, int y) {
	if (x < 0 || y < 0 || x >= XRES || y >= YRES)
		return;

	PropertyValue value1, value2, value3;
	value1.Integer = parts[i].tmp;
	value2.Integer = parts[i].ctype;
	value3.UInteger = parts[i].dcolour;
	sim->flood_prop(x, y, offsetof(Particle, tmp), value1, StructProperty::Integer);
	sim->flood_prop(x, y, offsetof(Particle, ctype), value2, StructProperty::Integer);
	sim->flood_prop(x, y, offsetof(Particle, dcolour), value3, StructProperty::UInteger);
}

//#TPT-Directive ElementHeader Element_FFGN static int update(UPDATE_FUNC_ARGS)
int Element_FFGN::update(UPDATE_FUNC_ARGS)
{
	/**
	 * Props:
	 * temp: radius / size
	 * ctype: type to affect
	 * life: on / off state
	 * tmp: shape
	 *   0: circle
	 *   1: square
	 *   2: connect to nearest FFGN
	 * tmp2: type
	 *   See FFLD.cpp
	 */

	// IMPORTANT NOTE:
	// ALL THE STUFF BELOW MAKES PT_FFGN BECAUSE FOR SOME REASON
	// THAT MAKES FFLD (No it doesn't make FFGN and IDs are correct!)
	if (parts[i].life > 0) {
		if (parts[i].tmp == 0) { // Circle
			int prevx = -1, prevy = -1;
			for (float angle = 0.0f; angle < 2 * 3.141592f; angle += 0.001f){
				int x1 = sin(angle) * parts[i].temp + parts[i].x;
				int y1 = cos(angle) * parts[i].temp + parts[i].y;

				if (x1 == prevx && y1 == prevy)
					continue;
				int ni = sim->create_part(-1, x1, y1, PT_FFGN);
				parts[ni].ctype = parts[i].ctype;
				parts[ni].tmp2 = parts[i].tmp2;
				parts[ni].dcolour = parts[i].dcolour;
				prevx = x1, prevy = y1;
			}
		}
		else if (parts[i].tmp == 1) { // Square
			sim->CreateBox(x - parts[i].temp, y - parts[i].temp, x + parts[i].temp, y - parts[i].temp, PT_FFGN, 0);
			sim->CreateBox(x - parts[i].temp, y + parts[i].temp, x + parts[i].temp, y + parts[i].temp, PT_FFGN, 0);
			sim->CreateBox(x - parts[i].temp, y - parts[i].temp, x - parts[i].temp, y + parts[i].temp, PT_FFGN, 0);
			sim->CreateBox(x + parts[i].temp, y - parts[i].temp, x + parts[i].temp, y + parts[i].temp, PT_FFGN, 0);

			// Find a middle of a side to floodfill from
			flood_fill_FFLD(parts, sim, i, x, y + parts[i].temp);
			flood_fill_FFLD(parts, sim, i, x, y - parts[i].temp);
			flood_fill_FFLD(parts, sim, i, x + parts[i].temp, y);
			flood_fill_FFLD(parts, sim, i, x - parts[i].temp, y);
		}
		else if (parts[i].tmp == 2) { // Electrode like behavior

		}
	}

	// Check for ON / OFF
	int r, rx, ry, rt;
	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			if (BOUNDS_CHECK) {
				r = pmap[y+ry][x+rx];
				if (!r) continue;
				rt = TYP(r);

				if (rt == PT_SPRK && (parts[ID(r)].ctype == PT_PSCN || parts[ID(r)].ctype == PT_NSCN)) {
					PropertyValue value;
					value.Integer = parts[ID(r)].ctype == PT_PSCN ? 1 : 0;
					sim->flood_prop(parts[i].x, parts[i].y, offsetof(Particle, life), value, StructProperty::Integer);
				}
			}


	return 0;
}

//#TPT-Directive ElementHeader Element_FFGN static int graphics(GRAPHICS_FUNC_ARGS)
int Element_FFGN::graphics(GRAPHICS_FUNC_ARGS)
{
	// Darker if deactivated
	if (cpart->life == 0) {
		*colr *= 0.5;
		*colg *= 0.5;
		*colb *= 0.5;
	}

	// Fancy rings emitting from the generator
	else {
		// Ring lasts 100 frames
		float multi = (ren->sim->timer % 100) / 100.0f; 
		if (cpart->tmp == 0) { // Circle
			for (float angle = 0.0f; angle < 2 * 3.141592f; angle += 0.06f){
				int x1 = multi * sin(angle) * cpart->temp + cpart->x;
				int y1 = multi * cos(angle) * cpart->temp + cpart->y;
				int x2 = multi * sin(angle + 0.06f) * cpart->temp + cpart->x;
				int y2 = multi * cos(angle + 0.06f) * cpart->temp + cpart->y;
				ren->draw_line(x1, y1, x2, y2, 200, 200, 255, 100);
			}
		}
		else if (cpart->tmp == 1) { // Square
			ren->draw_line(cpart->x - cpart->temp * multi, cpart->y - cpart->temp * multi, cpart->x - cpart->temp * multi, cpart->y + cpart->temp * multi, 200, 200, 255, 100);
			ren->draw_line(cpart->x + cpart->temp * multi, cpart->y - cpart->temp * multi, cpart->x + cpart->temp * multi, cpart->y + cpart->temp * multi, 200, 200, 255, 100);
			ren->draw_line(cpart->x + cpart->temp * multi, cpart->y - cpart->temp * multi, cpart->x - cpart->temp * multi, cpart->y - cpart->temp * multi, 200, 200, 255, 100);
			ren->draw_line(cpart->x + cpart->temp * multi, cpart->y + cpart->temp * multi, cpart->x - cpart->temp * multi, cpart->y + cpart->temp * multi, 200, 200, 255, 100);
		}
	}

	return 0;
}

Element_FFGN::~Element_FFGN() {}
