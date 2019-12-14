#include "simulation/quantum/qmath.h"
#include "simulation/quantum/linalg.h"


#include <complex>
#include <cstdint>
#include <limits>
#include <cmath>
#include <vector>
#include <unordered_map>


#include <bitset>
#include <set>
#include <algorithm>
 
/* Helper functions */
// ---------------------------- //

// Function to swap bits are position p and q in integer n
int swapbit(int n, int p, int q) {
    // If bits are different at position p and q swap
    if (((n & (1 << p)) >> p) ^ ((n & (1 << q)) >> q)) {
        n ^= (1 << p);
        n ^= (1 << q);
    }
    return n;
}


/* The QMATH namespace */
// ---------------------------- //
namespace QMATH {

// Given a quantum state swap the qbits at positions d1 and d2 (0 indexed)
// Swaps values of the state in place and returns a set of vector indices that were swapped
void swap_state_vector(LINALG::vec &state, std::set<std::pair<int, int> > &to_swap, int d1, int d2) {
    int b1 = 1 << d1; // Bit mask for pos1
    int b2 = 1 << d2; // Bit mask for pos2

    for (unsigned int i = 0; i < state.size(); i++) {
        bool x1 = (b1 & i);
        bool x2 = (b2 & i);

        if (x1 ^ x2) { // The 2 qbits are opposite (bit1 XOR bit2)
            int swapped = swapbit(i, d1, d2), index = i;
            if (index > swapped) // Make sure the swap pair is in order from lowest to highest
                std::swap(index, swapped);
            std::pair<int, int> swap_pair = std::make_pair(index, swapped);

            // If we haven't already swapped swap and insert into set
            if (to_swap.find(swap_pair) == to_swap.end()) {
                to_swap.insert(swap_pair);
                std::swap(state[index], state[swapped]);
            }
        }
    }
}






























































// OLD JUNK DELETE



// Thanks LBPHacker for fancy float packing. Also these asserts lol.
static_assert(std::numeric_limits<float>::is_iec559, "You can't compile simulation/quantum/qmath on your system! Blame hacky code.");
static_assert(sizeof(std::uint32_t) == sizeof(float), "You can't compile simulation/quantum/qmath on your system! Blame hacky code.");



// ----------- Quantum --------------- \\

/* Observe function, collapses the ION and returns its spin state
 * A is spin up, B is spin down, we'll define A = 1 and B = 0 */
int observe(packed_float &A, packed_float &B) {
    float a, b, c, d;
    unpack_float(A, a, b);
    unpack_float(B, c, d);

    if (A == ONE) // 100% spin down as A = 1+0i
        return 0;
    else if (B == ONE) // 100% spin up as B = 1+0i
        return 1;
    
    float prob = (float)(rand() % 100000) / 100000;
    float prob_spinup = a * a + b * b;
    bool is_up = prob <= prob_spinup;

    // Wave function collapse
    A = is_up ? ONE : 0;
    B = is_up ? 0 : ONE;
    return is_up ? 1 : 0;
}

/* Applies the calculations of a 4x4 gate onto 2 particles
 * A, B are the coefficents for particle 1, and so on.
 * a through p are the matrix [a, b, c, d], [e, f, ... 
 * tmp1, tmp2 are references to the tmp value of each ION
 */
void do_4_by_4_gate(packed_float &A, packed_float &B, packed_float &C, packed_float &D,
        int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l,
        int m, int n, int o, int p, int &tmp1, int &tmp2) {
    // Calculate tensor product
    // [a,b] (+) [c, d] = [ac, ad, bc, bd]
    std::complex<float> x, y, z, w, r1, r2, r3, r4;
    x = to_complex(mult_complex(A, C));
    y = to_complex(mult_complex(A, D));
    z = to_complex(mult_complex(B, C));
    w = to_complex(mult_complex(B, D));

    // If tensor product is [x, y, z, w]
    // Product of matrix is [ax + by + cz + dw, ex + fy...]
    r1 = float(a) * x + float(b) * y + float(c) * z * float(d) * w;
    r2 = float(e) * x + float(f) * y + float(g) * z * float(h) * w;
    r3 = float(i) * x + float(j) * y + float(k) * z * float(l) * w;
    r4 = float(m) * x + float(n) * y + float(o) * z * float(p) * w;

    /* 
     * Resolve state: if output is r1, r2, r3, r4
     * let a = |r1^2|, b = |r2^2|, c = |r3^2|, ...
     * then probability of |0> for:
     * particle 1:  -a - b + 1
     * particle 2:  b / (a + b)
     * given a + b != 0
     * 
     * Thus, particle a's state1 is sqrt(-a - b + 1) and sqrt(a + b)
     * particle b's state is  sqrt(b / (a + b)) and sqrt(1 - b(/a + b))
     * 
     * Note: if a + b = 0 then the solution is
     * particle 1: 1;
     * particle 2: sqrt(1 - c);
     */
    float r12 = abs(r1 * r1);
    float r22 = abs(r2 * r2);

    // TODO try to entangle
    if (is_entangled(r1, r2, r3, r4)) {
        
    }

    // probp1: Should be B, probability of |1> for particle 1
    // probp2: Should be D, probability of |1> for particle 2
    float probp1, probp2;

    if (r12 + r22 != 0) { // Case 1
        probp1 = -r12 - r22 + 1.0f;
        probp2 = probp2 = r22 / (r12 + r22);
    } else {  // Case 2
        probp1 = 1;                        
        probp2 = 1 - abs(r3 * r3);
    }

    A = packed_sqrt(1 - probp1);
    B = packed_sqrt(probp1);
    C = packed_sqrt(1 - probp2);
    D = packed_sqrt(probp2);
}

/* Applies the calculations of a 8x8 gate onto 3 particles
 * A, B are the coefficents for particle 1, and so on.
 * a0 through a63 are the matrix [a0, a1, ... a7], [a8, a9...]...
 * tmp1, tmp2 and tmp3 are references to the tmp value of each ION
 */
void do_8_by_8_gate(packed_float &A, packed_float &B, packed_float &C, packed_float &D,
        packed_float &E, packed_float &F,
        int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20, int a21, int a22, int a23, int a24, int a25, int a26, int a27, int a28, int a29, int a30, int a31, int a32, int a33, int a34, int a35, int a36, int a37, int a38, int a39, int a40, int a41, int a42,  int a43, int a44, int a45, int a46, int a47, int a48, int a49, int a50, int a51, int a52, int a53, int a54, int a55, int a56, int a57, int a58, int a59, int a60, int a61, int a62, int a63,
        int &tmp1, int &tmp2, int &tmp3) {
    // Calculate tensor product
    // [a,b] (+) [c, d] (+) [e, f ] = [ace, ade, bce, bde, acf, adf, bcf, bdf]
    std::complex<float> x1, x2, x3, x4, x5, x6, x7, x8;
    x1 = to_complex(mult_complex(A, mult_complex(C, E)));
    x2 = to_complex(mult_complex(A, mult_complex(D, E)));
    x3 = to_complex(mult_complex(B, mult_complex(C, E)));
    x4 = to_complex(mult_complex(B, mult_complex(D, E)));
    x5 = to_complex(mult_complex(A, mult_complex(C, F)));
    x6 = to_complex(mult_complex(A, mult_complex(D, F)));
    x7 = to_complex(mult_complex(B, mult_complex(C, F)));
    x8 = to_complex(mult_complex(B, mult_complex(D, F)));

    // If tensor product is [x1, x2, x3 ... x8]
    // Product of matrix is [a0x1 + a1x2 ...]
    std::complex<float> r1, r2, r3, r4, r5, r6, r7, r8;
    r1 = float(a0) * x1 + float(a1) * x2 + float(a2) * x3 + float(a3) * x4 + float(a4) * x5 + float(a5) * x6 + float(a6) * x7 + float(a7) * x8;
    r2 = float(a8) * x1 + float(a9) * x2 + float(a10) * x3 + float(a11) * x4 + float(a12) * x5 + float(a13) * x6 + float(a14) * x7 + float(a15) * x8;
    r3 = float(a16) * x1 + float(a17) * x2 + float(a18) * x3 + float(a19) * x4 + float(a20) * x5 + float(a21) * x6 + float(a22) * x7 + float(a23) * x8;
    r4 = float(a24) * x1 + float(a25) * x2 + float(a26) * x3 + float(a27) * x4 + float(a28) * x5 + float(a29) * x6 + float(a30) * x7 + float(a31) * x8;
    r5 = float(a32) * x1 + float(a33) * x2 + float(a34) * x3 + float(a35) * x4 + float(a36) * x5 + float(a37) * x6 + float(a38) * x7 + float(a39) * x8;
    r6 = float(a40) * x1 + float(a41) * x2 + float(a42) * x3 + float(a43) * x4 + float(a44) * x5 + float(a45) * x6 + float(a46) * x7 + float(a47) * x8;
    r7 = float(a48) * x1 + float(a49) * x2 + float(a50) * x3 + float(a51) * x4 + float(a52) * x5 + float(a53) * x6 + float(a54) * x7 + float(a55) * x8;
    r8 = float(a56) * x1 + float(a57) * x2 + float(a58) * x3 + float(a59) * x4 + float(a60) * x5 + float(a61) * x6 + float(a62) * x7 + float(a63) * x8;
}















// ---- Entanglement ----- //
std::unordered_map<int, EntanglementState> entanglement_pairs; // Entanglement ID : entanglement state
std::unordered_map<int, int> id_to_entanglement_id; // particle ID to entanglement id

/* Given the particle state and ID, check for entanglement
 * Call this after single qbit gates to update other particles */
void single_particle_entanglement(packed_float A, packed_float B, int id2, Particle *parts) {
    if (id_to_entanglement_id[id2]) {
        int id = id_to_entanglement_id[id2];
        if (!entanglement_pairs[id].is_valid_entanglement(parts)) { // Entanglement group is no longer valid
            entanglement_pairs.erase(id);
            id_to_entanglement_id.erase(id2);
            return;
        }
        for (unsigned int i = 0; i < entanglement_pairs[id].particleIDs.size(); i++) {
            parts[entanglement_pairs[id].particleIDs[i]].pavg[0] = A;
            parts[entanglement_pairs[id].particleIDs[i]].pavg[1] = B;
            
            // Particle has inverse entanglement with respect to this particle
        }
    }
}

/* Is a system entangled? This is the resulting tensor with respect to basis
 * |00...>, |00...1>, ... |11...> for 2qbits. The tensor is
 * <a, b, c, d>.
 * 
 * The formula below comes from showing the basis result is solveable, that is
 * there exists a contradiction in the solution: ad - bc = 0 if not entangled */
bool is_entangled(cmplx a, cmplx b, cmplx c, cmplx d) {
    return abs(a * d - b * c) >= 0.01;
}

/* Is a system entangled? This is the resulting tensor with respect to basis
 * |00...>, |00...1>, ... |11...> for 3qbits. The tensor is
 * <a, b, c, d, e, f, g, h>.
 * 
 * The below tests: we honestly brute forced all combinations such that there exist
 * |A> (X) |B> (X) |C> = <a, b, c... h> has a solution
 */
bool is_entangled(int a, int b, int c, int d, int e, int f, int g, int h) {
    if (a == 0 && b == 0 && c == 0 && d == 0 && e == 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a == 0 && b == 0 && c == 0 && d == 0 && e == 0 && f == 0 && g == 0 && h != 0 ) { return false; }
    if (a == 0 && b == 0 && c == 0 && d == 0 && e == 0 && f == 0 && g != 0 && h == 0 ) { return false; }
    if (a == 0 && b == 0 && c == 0 && d == 0 && e == 0 && f == 0 && g != 0 && h != 0 ) { return false; }
    if (a == 0 && b == 0 && c == 0 && d == 0 && e == 0 && f != 0 && g == 0 && h == 0 ) { return false; }
    if (a == 0 && b == 0 && c == 0 && d == 0 && e == 0 && f != 0 && g == 0 && h != 0 ) { return false; }
    if (a == 0 && b == 0 && c == 0 && d == 0 && e != 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a == 0 && b == 0 && c == 0 && d == 0 && e != 0 && f == 0 && g != 0 && h == 0 ) { return false; }
    if (a == 0 && b == 0 && c == 0 && d == 0 && e != 0 && f != 0 && g == 0 && h == 0 ) { return false; }
    if (a == 0 && b == 0 && c == 0 && d == 0 && e != 0 && f != 0 && g != 0 && h != 0 ) { return false; }
    if (a == 0 && b == 0 && c == 0 && d != 0 && e == 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a == 0 && b == 0 && c == 0 && d != 0 && e == 0 && f == 0 && g == 0 && h != 0 ) { return false; }
    if (a == 0 && b == 0 && c != 0 && d == 0 && e == 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a == 0 && b == 0 && c != 0 && d == 0 && e == 0 && f == 0 && g != 0 && h == 0 ) { return false; }
    if (a == 0 && b == 0 && c != 0 && d != 0 && e == 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a == 0 && b == 0 && c != 0 && d != 0 && e == 0 && f == 0 && g != 0 && h != 0 ) { return false; }
    if (a == 0 && b != 0 && c == 0 && d == 0 && e == 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a == 0 && b != 0 && c == 0 && d == 0 && e == 0 && f != 0 && g == 0 && h == 0 ) { return false; }
    if (a == 0 && b != 0 && c == 0 && d != 0 && e == 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a == 0 && b != 0 && c == 0 && d != 0 && e == 0 && f != 0 && g == 0 && h != 0 ) { return false; }
    if (a != 0 && b == 0 && c == 0 && d == 0 && e == 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a != 0 && b == 0 && c == 0 && d == 0 && e != 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a != 0 && b == 0 && c != 0 && d == 0 && e == 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a != 0 && b == 0 && c != 0 && d == 0 && e != 0 && f == 0 && g != 0 && h == 0 ) { return false; }
    if (a != 0 && b != 0 && c == 0 && d == 0 && e == 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a != 0 && b != 0 && c == 0 && d == 0 && e != 0 && f != 0 && g == 0 && h == 0 ) { return false; }
    if (a != 0 && b != 0 && c != 0 && d != 0 && e == 0 && f == 0 && g == 0 && h == 0 ) { return false; }
    if (a != 0 && b != 0 && c != 0 && d != 0 && e != 0 && f != 0 && g != 0 && h != 0 ) { return false; }
    return true;
}









// ---------- Math ----------------\\

/* Multiplies: [a b][A] = [C]
*              [c d][B]   [D]
* and sets A = C, B = D once computation is complete.
* Each float is a packed_float representing a complex number */
void mult_matrix(packed_float &A, packed_float &B, packed_float a, packed_float b, packed_float c, packed_float d) {
    packed_float C = add_complex(mult_complex(a, A), mult_complex(b, B));
    packed_float D = add_complex(mult_complex(c, A), mult_complex(d, B));
    A = C; B = D;
}

/* Convert a packed float to a complex number */
std::complex<float> to_complex(packed_float x) {
    float a, b;
    unpack_float(x, a, b);
    return std::complex<float>(a, b);
}

/* Takes sqrt() of a float as a packed_float */
packed_float packed_sqrt(float a) {
    if (a == 0) return 0;
    if (a < 0) return pack_float(0, sqrt(-a));
    return pack_float(sqrt(a), 0);
}

/* Adds 2 complex numbers x and y, where
* x = a + bi and y = c + di represented
* by 2 packed floats.
* Returns a packed float representing the answer */
packed_float add_complex(packed_float x, packed_float y) {
    float a, b, c, d;
    unpack_float(x, a, b);
    unpack_float(y, c, d);
    return pack_float(a + c, b + d);
}

/* Multiplies 2 complex numbers x and y, where
* x = a + bi and y = c + di represented
* by 2 packed floats.
* Returns a packed float representing the answer */
packed_float mult_complex(packed_float x, packed_float y) {
    float a, b, c, d;
    unpack_float(x, a, b);
    unpack_float(y, c, d);

    float p1 = a*c - b*d;
    float p2 = a*d + b*c;

    // Round multiplication if enough
    if (p1 > 0.99) p1 = 1;
    else if (p1 < -0.99) p1 = -1;
    if (p2 > 0.99) p2 = 1;
    else if (p2 < -0.99) p2 = -1;

    return pack_float(p1, p2);
}

packed_float pack_float(float real, float imag) {
    // All magnitudes must be between -1 and 1, this
    // solves some rounding errors
    if (real < -0.99) real = -1;
    else if (real > 0.99) real = 1;
    if (imag < -0.99) imag = -1;
    else if (imag > 0.99) imag = 1;

    std::uint32_t ugly = (((*reinterpret_cast<std::uint32_t *>(&real)) >> 16) |
                          ((*reinterpret_cast<std::uint32_t *>(&imag)) & 0xFFFF0000U));
    return *reinterpret_cast<float *>(&ugly);
}

void unpack_float(float a, float &x, float &y) {
    std::uint32_t t1 = (*reinterpret_cast<std::uint32_t *>(&a)) << 16;
    std::uint32_t t2 = (*reinterpret_cast<std::uint32_t *>(&a)) & 0xFFFF0000U;
    x = *reinterpret_cast<float *>(&t1);
    y = *reinterpret_cast<float *>(&t2);
}


// /* Packs given two floats into one float */
// packed_float pack_float(float x, float y) {
//     int x1 = (int)(x * scaleFactor);
//     int y1 = (int)(y * scaleFactor);
//     float f = (y1 * cp) + x1;
//     return f;
// }

// /* Unpacks given float to two floats */
// void unpack_float(packed_float f, float &x, float &y) {
//     double dy = floor(f / cp);
//     double dx = f - (dy * cp);
//     y = (float)(dy / scaleFactor);
//     x = (float)(dx / scaleFactor);
// }}

const packed_float ONE = pack_float(1, 0);
const packed_float N_ONE = pack_float(-1, 0);
const packed_float I = pack_float(0, 1);
const packed_float N_I = pack_float(0, -1);
const packed_float INVSQRT2 = pack_float(0.7071067811865475, 0);
const packed_float N_INVSQRT2 = pack_float(-0.7071067811865475, 0);
}

