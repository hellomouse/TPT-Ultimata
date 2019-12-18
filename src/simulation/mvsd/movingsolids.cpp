#include <unordered_map>
#include <list>
#include <iostream>
#include <cmath>

#include "simulation/ElementCommon.h"
#include "simulation/mvsd/movingsolids.h"


// A group of moving solids
MOVINGSOLID::MVSDGroup::MVSDGroup() {

}

MOVINGSOLID::MVSDGroup::MVSDGroup(const std::list<int> &particles, int type, int id) {
    particle_ids = particles;
    vx = vy = fx = fy = 0.0f;
    state_id = id;
    ptype = type;
    dx = dy = 0;
}

// Recalculate the center of mass
void MOVINGSOLID::MVSDGroup::calc_center(Particle *parts) {
    if (particle_ids.size() == 0)
        return;

    int sumx = 0, sumy = 0;
    for (auto itr = particle_ids.begin(); itr != particle_ids.end(); ++itr) {
        // Ignore particles that are outside of screen
        if (parts[*itr].x < 0 || parts[*itr].y < 0)
            continue;

        sumx += (int)(parts[*itr].x + 0.5);
        sumy += (int)(parts[*itr].y + 0.5);
    }

    cx = sumx / particle_ids.size();
    cy = sumy / particle_ids.size();
}

// Redefine center of mass
void MOVINGSOLID::MVSDGroup::assign_center(int _cx, int _cy) {
    cx =  _cx; cy = _cy;
}

// Compute forces due to gravity, pressure, etc...
void MOVINGSOLID::MVSDGroup::calc_forces(Particle *parts, int pmap[YRES][XRES], Simulation *sim) {
    // TODO consider all forces in sim and create a "net force"
    // with magntidue and direction and then compute torque if applicable
    // calculate net torque on solid and net force and
    // move particles with the force and rotate the entire solid


    // We can pretend gravity acts on the center of mass
    switch (sim->gravityMode) {
        case 0: { // Downwards gravity
            fy = MVSD_GRAVITY;
            break;
        }
        case 1: {  break; } // No gravity
        case 2: { // Radial gravity
            float pGravD = 0.01f - hypotf((cx - XCNTR), (cy - YCNTR));
            fx = MVSD_GRAVITY * ((float)(cx - XCNTR) / pGravD);
            fy = MVSD_GRAVITY * ((float)(cy - YCNTR) / pGravD);
            break;
        }
    }

    // Newtonian gravity
    fx += MVSD_NEWTON_GRAVITY * sim->gravx[(cy / CELL) * (XRES / CELL) + (cx / CELL)];
    fy += MVSD_NEWTON_GRAVITY * sim->gravy[(cy / CELL) * (XRES / CELL) + (cx / CELL)];

    // Pressure
    fx += MVSD_AIR * sim->vx[cy / CELL][cx / CELL];
    fy += MVSD_AIR * sim->vy[cy / CELL][cx / CELL];

    // Stasis field (TODO)

    // Apply the net force (it's really an acceleration tbh)
    vx += fx;
    vy += fy;
}

void MOVINGSOLID::MVSDGroup::update(Particle *parts, int pmap[YRES][XRES], Simulation *sim) {
    auto i = particle_ids.begin();
    bool has_particle_removed = false;
    bool willcollide = false;
    int collision_type = 0;
    int newx, newy;
   
    while (i != particle_ids.end()) {
        // Particle no longer exists, delete it
        while (parts[*i].type != ptype || parts[*i].tmp2 != state_id) {
            i = particle_ids.erase(i);
            has_particle_removed = true;
            if (i == particle_ids.end()) break;
        }
        if (i != particle_ids.end()) ++i;
    }

    if (particle_ids.size() == 0)
        return;

    // Bounce on collision
    bool should_bounce = false; // Should the particle bounce off the surface or ignore bounce
    bool should_freeze = false; // Should the particle freeze? (ie on a surface) or ignore

    float deflectx = 0, deflecty = 0; // Keep track of deflection dir, this is like a "net force" vector
    int total_repel_x = 0, total_repel_y = 0; // Tracks direction to "repel" away from other particles / solids
    float rotate_sum = 0; // Keep track of total torque

    int fake_vx, fake_vy;
    get_fake_velocity(fake_vx, fake_vy);

    // bool velocity_is_fast = (fabs(vx) > 1 || fabs(vy) > 1);
    bool velocity_is_fast = (fabs(vx) > 0.5 || fabs(vy) > 0.5);

    for (auto i = collisions.begin(); i != collisions.end(); ++i) {
        /**
         * Here we distinct between merely being "beside"
         * and touching an object and actually colliding with it (ie, is there
         * a normal force from the collision?
         * 
         * If a particle exists 1px in the direction of the velocity vector, OR
         * the displacement particle that detected the collision to the collided particle
         * is nearly along the current velocity vector, we have collided
         */
        
        // Determine if moving in the direction of vx or vy will intersect another particle
        // Since high speed collisions are handled by MVSD we can assume vx and vy are not that big
        // and just check the location directly without doing a line intersection
        int testx = (int)round(i->cx + fake_vx);
        int testy = (int)round(i->cy + fake_vy);
        if (testx < 0 || testy < 0 || testx >= XRES || testy >= YRES)
            continue;

        int r = pmap[testy][testx];
        if ((i->cx - i->px == fake_vx && i->cy - i->py == fake_vy) || 
            (r && (TYP(r) != ptype || parts[ID(r)].tmp2 != state_id))) {

            // If we're approaching the other object at "high speed" then bounce
            if (velocity_is_fast)
                should_bounce = true;

            // Since it's a collision and if the solid continues on its current velocity
            // it will intersect another particle we mark that it should stop (since it's
            // not bouncing)
            else if (!should_bounce) {
                should_freeze = true;
                total_repel_x += i->px - i->cx;
                total_repel_y += i->py - i->cy;
            }

            // Update net force direction
            deflectx -= i->cx - i->px;
            deflecty -= i->cy - i->py;

            // Update net torque
        }

        // Collision with static object: dampen the velocity
        if (i->type == STATIC) {

        }

        // Collision with another moving solid
        else if (i->type == MOVING) {
        }
    }

    // To prevent phasing through solids we negate solids that should
    // be stopped (ie resting on a surface). Every 100 frames we
    // randomly impart a repeling velocity from the direction of objects
    // it collided with (DO NOT CHANGE THE MAGIC CONSTANTS BELOW)
    // to avoid particles being "stuck" inside one another
    if (should_freeze) {
        if (sim->timer % 100 == 0) { 
            vx = total_repel_x / 15.0f / collisions.size();
            vy = total_repel_y / 15.0f / collisions.size();
        } else {
            vx = vy = 0.0f;
        }
    }

    // Solid should bounce. If a dx or dy is indicated the solid will bounce
    // anyways because that only happens if velocity is extremely large and solid
    // risks phasing into solids
    if (!should_freeze && (usedx || usedy || should_bounce)) {
        // Only bounce if deflect is large enough, otherwise you might get
        // extreme deflection angles from 1 or 2px of collision
        if (deflectx || deflecty) {
            float angle = atan2(deflecty, deflectx);
            float v_mag = sqrtf(vx * vx + vy * vy);

            vx = v_mag * cos(angle) * MVSD_BOUNCE;
            vy = v_mag * sin(angle) * MVSD_BOUNCE;
        }
    }

    // Limit max velocity
    if (fabs(vx) > MAX_VELOCITY)
        vx = vx < 0 ? -MAX_VELOCITY : MAX_VELOCITY;
    if (fabs(vy) > MAX_VELOCITY)
        vy = vy < 0 ? -MAX_VELOCITY : MAX_VELOCITY;

    // Update all individual particles
    i = particle_ids.begin();
    while (i != particle_ids.end()) {
        int px = parts[*i].x;
        int py = parts[*i].y;

        // Temp code to mark center of mass for deco
        if ((int)(parts[*i].x) == cx || 
            (int)(parts[*i].y) == cy)
            parts[*i].tmp = 2;
        else
            parts[*i].tmp = 0;

        if (usedx) parts[*i].x += dx;
        else parts[*i].x += vx;
        if (usedy) parts[*i].y += dy;
        else parts[*i].y += vy;

        // Warns about rubberbanding debug TODO
        if (abs(px - parts[*i].x) + abs(py - parts[*i].y) > 50) {
            std::cout << px << ", " << py << " -> " << parts[*i].x << ", " << parts[*i].y << "\n";
            std::cout << vx << ", " << vy << ", delta: " << dx << ", " << dy << "\n";
        }
        
        ++i;
    }

    // We can approximate the center of gravity update
    // However the below applied value is not 100% accurate
    // due to rounding error so we have to recalculate
    // every couple of frames to avoid drift. Also center should
    // be recalculated on collision
    if (sim->timer % 10 == 0 || (collisions.size() > 0 && previous_collision_size != collisions.size())) {
        calc_center(parts);
    } else {
        cx = (int)(cx + vx + 0.5);
        cy = (int)(cy + vy + 0.5);
    }

    if (cx < 0) cx = 0;
    if (cy < 0) cy = 0;

    dx = dy = 100;
    usedx = false, usedy = false;
    previous_collision_size = collisions.size();
    collisions.clear();

    fx = fy = 0.0;
    calc_forces(parts, pmap, sim);
}

// Everything else
namespace MOVINGSOLID {

// Hash map of all moving solids
std::unordered_map<int, MVSDGroup> solids;

// ID handling
int get_new_state() {
    int new_state_id;
    do {
        new_state_id = rand() % 10000000 + 1;
        if (solids.find(new_state_id) == solids.end())
            break;
    } while(true);
    return new_state_id;
}

// Floodfill creation
// Helper to floodfill
void floodfill_helper(Particle *parts, int x, int y, int pmap[YRES][XRES], int state_id,
        std::list<int> &to_add_id, int &sumx, int &sumy, int &count, int type=PT_MVSD) {
    if (x < 0 || y < 0 || x > XRES || y > YRES)
        return;

    // Check if current spot is valid, if not return now
    // Invalid if not right type or already has ID (tmp2 > 0)
    if (parts[ID(pmap[y][x])].type != type || parts[ID(pmap[y][x])].tmp2 > 0)
        return;

    parts[ID(pmap[y][x])].tmp2 = state_id;
    to_add_id.push_back(ID(pmap[y][x]));
    
    ++count;
    sumx += (int)(parts[ID(pmap[y][x])].x + 0.5);
    sumy += (int)(parts[ID(pmap[y][x])].y + 0.5);

    floodfill_helper(parts, x - 1, y, pmap, state_id, to_add_id, sumx, sumy, count, type);
    floodfill_helper(parts, x + 1, y, pmap, state_id, to_add_id, sumx, sumy, count, type);
    floodfill_helper(parts, x, y - 1, pmap, state_id, to_add_id, sumx, sumy, count, type);
    floodfill_helper(parts, x, y + 1, pmap, state_id, to_add_id, sumx, sumy, count, type);
}


void create_moving_solid(Particle *parts, int pmap[YRES][XRES], int i, int type) {
    const int state_id = get_new_state();
    std::list<int> particle_ids;

    int sumx, sumy, n;
    sumx = sumy = n = 0;

    floodfill_helper(parts, (int)(parts[i].x + 0.5), (int)(parts[i].y + 0.5), pmap, state_id, particle_ids,
        sumx, sumy, n, type);
    solids[state_id] = MVSDGroup(particle_ids, type, state_id);
    solids[state_id].assign_center(sumx / n, sumy / n);
}
};