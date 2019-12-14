#ifndef QMATH_H
#define QMATH_H

#include <complex>
#include <unordered_map>
#include <list>
#include <set>

#include "simulation/quantum/linalg.h"
#include "simulation/ElementCommon.h"

class EntanglementState {
public:
    std::vector<int> particleIDs;
    std::vector<int> state_vector;

    bool is_valid_entanglement(Particle *parts) {
        for (unsigned int i = 0; i < particleIDs.size(); i++) {
            if (parts[particleIDs[i]].type != PT_ION)
                return false;
        }
        return true;
    }
};

namespace QMATH {
    void swap_state_vector(LINALG::vec &state, std::set<std::pair<int, int> > &to_swap, int d1, int d2);





    // OLD JUNK DELETE
    typedef float packed_float;
    typedef std::complex<float> cmplx;

    const double scaleFactor = 65530.0;
    const double cp = 256.0 * 256.0;

    // Entanglement
    void single_particle_entanglement(packed_float A, packed_float B, int id, Particle *parts);
    bool is_entangled(cmplx a, cmplx b, cmplx c, cmplx d);                     // Check if 2 qbit tensor is entangled
    bool is_entangled(int a, int b, int c, int d, int e, int f, int g, int h); // Check if 3 qbit tensor is entangled

    // Quantum
    int observe(packed_float &A, packed_float &B);
    void do_4_by_4_gate(packed_float &A, packed_float &B, packed_float &C, packed_float &D,
        int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l,
        int m, int n, int o, int p, int &tmp1, int &tmp2);
    void do_8_by_8_gate(packed_float &A, packed_float &B, packed_float &C, packed_float &D,
        packed_float &E, packed_float &F,
        int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20, int a21, int a22, int a23, int a24, int a25, int a26, int a27, int a28, int a29, int a30, int a31, int a32, int a33, int a34, int a35, int a36, int a37, int a38, int a39, int a40, int a41, int a42,  int a43, int a44, int a45, int a46, int a47, int a48, int a49, int a50, int a51, int a52, int a53, int a54, int a55, int a56, int a57, int a58, int a59, int a60, int a61, int a62, int a63,
        int &tmp1, int &tmp2, int &tmp3);

    // Helper math functions
    void mult_matrix(packed_float &A, packed_float &B, packed_float a, packed_float b, packed_float c, packed_float d);

    std::complex<float> to_complex(packed_float x);

    packed_float packed_sqrt(float a);
    packed_float add_complex(packed_float x, packed_float y);
    packed_float mult_complex(packed_float x, packed_float y);
    packed_float negative(packed_float x);
    packed_float pack_float(float x, float y);
    void unpack_float(packed_float f, float &x, float &y);

    // Packed floats have different representations
    extern const packed_float ONE, N_ONE, I, N_I, INVSQRT2, N_INVSQRT2;
    extern std::unordered_map<int, EntanglementState> entanglement_pairs;
}

#endif