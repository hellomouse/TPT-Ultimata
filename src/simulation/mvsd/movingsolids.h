#include <unordered_map>
#include <list>

#include "simulation/ElementCommon.h"

namespace MOVINGSOLID {

// A group of moving solids
class MVSDGroup {
public:
    MVSDGroup();
    MVSDGroup(const std::list<int> &particles, int type, int id);
    void update(Particle *parts, int pmap[YRES][XRES], Simulation *sim);
    void calc_center(Particle *parts);
    void assign_center(int cx, int cy);

    int stateID() { return state_id; }
    int particles() { return particle_ids.size(); }
private:
    std::list<int> particle_ids;
    int ptype, state_id, cx, cy;
    float vx, vy;

    // Collision handling, saves where the solid has collided with other blocks
    std::list<std::pair<int, int> > collisions;
};

// Hash map of all moving solids
extern std::unordered_map<int, MVSDGroup> solids;

// ID handling
int get_new_state();

// Floodfill creation
void create_moving_solid(Particle *parts, int pmap[YRES][XRES], int i, int type=PT_MVSD);
};