#include <unordered_map>
#include <list>
#include <iostream>

#include "simulation/ElementCommon.h"
#include "simulation/mvsd/movingsolids.h"

// A group of moving solids
MOVINGSOLID::MVSDGroup::MVSDGroup() {

}

MOVINGSOLID::MVSDGroup::MVSDGroup(const std::list<int> &particles, int type, int id) {
    particle_ids = particles;
    vx = vy = 0.0f;
    state_id = id;
    ptype = type;
}

// Recalculate the center of mass
void MOVINGSOLID::MVSDGroup::calc_center(Particle *parts) {

}

// Redefine center of mass
void MOVINGSOLID::MVSDGroup::assign_center(int _cx, int _cy) {
    cx =  _cx; cy = _cy;
}

void MOVINGSOLID::MVSDGroup::update(Particle *parts, int pmap[YRES][XRES], Simulation *sim) {
    // Apply checks: remove particles that don't exist,
    // apply collision list updating
    auto i = particle_ids.begin();
    collisions.clear();
    bool has_particle_removed = false;

    while (i != particle_ids.end()) {
        // Particle no longer exists, delete it
        while (parts[*i].type != ptype || parts[*i].tmp2 != state_id) {
            i = particle_ids.erase(i);
            has_particle_removed = true;

            if (i == particle_ids.end()) {
                calc_center(parts);
                return;
            }
        }

        // Add locations of collisions
        if (parts[*i].flags > 0) {
            collisions.push_back(std::make_pair(
                (int)(parts[*i].x + 0.5),
                (int)(parts[*i].y + 0.5)
            ));
        }
        ++i;
    }

    // Recalculate center of gravity if particles have been removed
    if (has_particle_removed)
        calc_center(parts);

    // TODO apply gravity lol
    vy += 0.05f;
    // TODO consider all forces in sim and create a "net force"
    // with magntidue and direction and then compute torque if applicable


    // Update all individual particles
    i = particle_ids.begin();
    while (i != particle_ids.end()) {
        // Temp code, no bounds checks lol
        if (collisions.size() == 0) {
            parts[*i].x += vx;
            parts[*i].y += vy;
        }
        ++i;
    }
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