#ifndef CYBERTRUCK_H
#define CYBERTRUCK_H

#include "simulation/ElementCommon.h"
#include <vector>

const int CYBERWIDTH = 46;
const int CYBERHEIGHT = 14;

// Do not make too large or is buggy
// NOTE: CYBERTRUCK UPDATES TWICE PER FRAME
const int CYBERSPEED = 3.0f; // Since truck bouncy half the time speed doesn't apply
const int CYBERFLYSPEED = 1.0f;
const float CYBER_ROTATION_SPEED = 0.1f;
const float MAX_CYBER_SPEED = 10.0f;

class CyberPixel {
public:
    CyberPixel(int x_, int y_, int r_, int g_, int b_):
        x(x_), y(y_), r(r_), g(g_), b(b_) {}
    const int x, y, r, g, b;
};

extern std::vector<CyberPixel> CYBERTRUCK_PIXELS;
void draw_cybertruck(Renderer *ren, Particle *cpart, float rotation);
void rotate(int &x, int &y, float rotation);
void rotate(float &x, float &y, float rotation);

#endif