#include "simulation/ElementCommon.h"
#include "music/audio.h"
#include "music/music.h"

#include <iostream>

//#TPT-Directive ElementClass Element_NOTE PT_NOTE 203
Element_NOTE::Element_NOTE()
{
	Identifier = "DEFAULT_PT_NOTE";
	Name = "NOTE";
	Colour = PIXPACK(0xDDDDDD);
	MenuVisible = 1;
	MenuSection = SC_ELEC;
	Enabled = 1;

	Advection = 0.00f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.00f;
	Loss = 0.00f;
	Collision = 0.00f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 100;

	Weight = 100;
	HeatConduct = 0;
	Description = "Plays a NOTE when sparked, tmp corresponds to piano key. (Earrape warning)";

	DefaultProperties.tmp = 49; // A
	Properties = TYPE_SOLID;
	
	Update = &Element_NOTE::update;
	Graphics = &Element_NOTE::graphics;
}

//#TPT-Directive ElementHeader Element_NOTE static int update(UPDATE_FUNC_ARGS)
int Element_NOTE::update(UPDATE_FUNC_ARGS) {
	int rx, ry, rt, r;
	
	NOTE::next_audio_device = NOTE::next_audio_device % NOTE::MAX_NOTES_AT_SAME_TIME;
	int index = NOTE::next_audio_device;
	int freq = NOTE::get_frequency_from_key(parts[i].tmp);

	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			if (BOUNDS_CHECK && (rx == 0 || ry == 0)) {
				r = pmap[y + ry][x + rx];
				if (!r) continue;
				rt = TYP(r);

				// Sparked, play note!
				if (rt == PT_SPRK || rt == PT_BRAY) {
					parts[i].life = NOTE::LIFE_CHUNK;
					NOTE::next_audio_device++;

					// Avoid lag, don't play too many notes at same time
					// if (NOTE::next_audio_device > NOTE::MAX_NOTES_AT_SAME_TIME)
					//	return 0;

					// If the sound for the freq already exists just play it, or construct it
					int t = parts[i].tmp;
					
					
					if (NOTE::sounds[index]) {
						// NOTE::sounds[parts[i].tmp]->stop();
						parts[i].life = 1231;
						NOTE::sounds[index]->set_freq(freq);
						// NOTE::sounds[parts[i].tmp]->play();
					} else {
						NOTE::sounds[index] = new Sound(freq);
						NOTE::sounds[index]->play();
					}

					parts[i].tmp = t;

					return 0;
				}
			}

	// Decrement life
	parts[i].life--;
	if (parts[i].life < 0)
		parts[i].life = 0;

	// No spark, stop playing noise
	if (parts[i].life <= 0 && NOTE::sounds[index]) {
		// NOTE::sounds[index]->stop();
		// delete NOTE::sounds[parts[i].tmp2];
		// NOTE::sounds.erase(NOTE::sounds.find(parts[i].tmp2));
	}

	return 0;
}

//#TPT-Directive ElementHeader Element_NOTE static int graphics(GRAPHICS_FUNC_ARGS)
int Element_NOTE::graphics(GRAPHICS_FUNC_ARGS) {
	if (cpart->life > 0)
		*colr = *colg = *colb = 255;
	return 0;
}

Element_NOTE::~Element_NOTE() {}
