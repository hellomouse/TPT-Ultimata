#ifndef QUANTUM_H
#define QUANTUM_H

#include <unordered_map>
#include <vector>

#include "simulation/quantum/linalg.h"
#include "simulation/quantum/def.h"
#include "simulation/ElementCommon.h"

namespace QUANTUM {
    struct QuantumState {
        LINALG::vec state;
        std::vector<int> id_order;
    };

    extern std::unordered_map<int, QuantumState> quantum_states;

    void observe(int state_id, int particle_index, int basis);
    void apply_gate(const LINALG::matrix &gate, Particle *parts, std::vector<int> particle_ids);

    void create_particle_state(Particle *parts, int i);
};

#endif