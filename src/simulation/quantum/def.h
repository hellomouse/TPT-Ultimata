#ifndef QDEF
#define QDEF

#include "common/string.h"

// Ctype enum for type of quantum gate
// ID: ctype value
namespace QGATE {
    const int X = 1;
    const int SWAP = 2;
    const int CNOT = 3;
    const int CCNOT = 4;
    const int CSWAP = 5;
    const int DETECT = 6;
    const int H = 7;
    const int Y = 8;
    const int Z = 9;
    const int S = 10;
    const int SINV = 11;
    const int T = 12;
    const int TINV = 13;
    const int SQRTNOT = 14;

    const int COOLDOWN = 5; // Number of frames QLOG has to cooldown before firing again, if <= 4 then SPRK might SPRK it twice

    extern String get_name(int gate_id);
    extern bool does_nothing(int gate_id);
    extern bool is_multi_gate(int gate_id);
};

#endif