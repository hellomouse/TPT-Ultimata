#include "simulation/ElementCommon.h"
#include "simulation/quantum/quantum.h"

//#TPT-Directive ElementClass Element_ION PT_ION 193
Element_ION::Element_ION()
{
	Identifier = "DEFAULT_PT_ION";
	Name = "ION";
	Colour = PIXPACK(0x80DBFF);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;
	Weight = -1;

	Collision = -0.99f;
	DefaultProperties.temp = 50.0f;
	Properties = TYPE_ENERGY;

	HeatConduct = 251;
	Description = "Supercooled ion, can be used for quantum computing. Decoheres with heat and interactions.";

	Update = &Element_ION::update;
	Graphics = &Element_ION::graphics;
}

//#TPT-Directive ElementHeader Element_ION static int update(UPDATE_FUNC_ARGS)
int Element_ION::update(UPDATE_FUNC_ARGS) {
	/**
	 * ION stores a state id, which is the quantum state its linked to, inside
	 * of its tmp2 variable. Most ION interactions occur in QLOG
	 */

	// Create quantum state if none:
	if (parts[i].flags != 1) {
		QUANTUM::create_particle_state(parts, i);
		parts[i].flags = 1;
	}

	if (parts[i].temp > 20.0f) {
		// If overheated will impart a random velocity to still IONs
		if (parts[i].vx == 0 && parts[i].vy == 0) {
			parts[i].vx = ((rand() % 1000) / 1000.0f * 8.0f + 1.0f) * (rand() % 2 == 0 ? -1 : 1);
			parts[i].vy = ((rand() % 1000) / 1000.0f * 8.0f + 1.0f) * (rand() % 2 == 0 ? -1 : 1);
		}

		// Randomly decohere
		if (rand() % 30 == 0)
			QUANTUM::decohere_particle(parts, i);
	}

	int r, rx, ry;

	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry)) {
				r = pmap[y+ry][x+rx];
				if (!r) r = sim->photons[y + ry][x + rx];
				if (!r) continue;

				// Decohere when touching other particles
				if (parts[ID(r)].type != PT_QLOG && parts[ID(r)].type != PT_ION) {
					// Collision, reverse other particle velocity
					// Yes this violates conservation of momentum
					parts[i].vx = parts[ID(r)].vx;
					parts[i].vy = parts[ID(r)].vy;
					parts[ID(r)].vx = -parts[ID(r)].vx;
					parts[ID(r)].vy = -parts[ID(r)].vy;
					
					
					if (rand() % 20 == 0)
						QUANTUM::decohere_particle(parts, i);
					if (rand() % 800 == 0)
						sim->kill_part(i);
				}
			}

			// SPRK metals and destroy self
			if ((sim->elements[TYP(r)].Properties & PROP_CONDUCTS)) {
				parts[ID(r)].life = 4;
				parts[ID(r)].ctype = TYP(r);
				sim->part_change_type(ID(r), x + rx, y + ry, PT_SPRK);
				sim->kill_part(i);
			}

	return 0;
}

//#TPT-Directive ElementHeader Element_ION static int graphics(GRAPHICS_FUNC_ARGS)
int Element_ION::graphics(GRAPHICS_FUNC_ARGS) {
	*firea = 30;
	*firer = *colr;
	*fireg = *colg;
	*fireb = *colb;

	*pixel_mode &= ~PMODE_FLAT;
	*pixel_mode |= FIRE_ADD | PMODE_ADD;
	if (rand() % 1000 == 0) // Don't flare too much
		*pixel_mode |= PMODE_LFLARE;

	return 0;
}

Element_ION::~Element_ION() {}
