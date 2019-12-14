#include "simulation/quantum/def.h"
#include "common/string.h"

namespace QGATE {
    String get_name(int gate_id) {
        switch(gate_id) {
            case X: return String("X");
            case SWAP: return String("SWAP");
            case CNOT: return String("CNOT");
            case CCNOT: return String("CCNOT");
            case CSWAP: return String("CSWAP");
            case DETECT: return String("OBSERVE");
            case H: return String("H");
            case Y: return String("Y");
            case Z: return String("Z");
            case S: return String("S");
            case SINV: return String("SINV");
            case T: return String("T");
            case TINV: return String("TINV");
            case SQRTNOT: return String("SQRTNOT");
            default: return String("Nothing");
        }
    }

    // Does the gate do nothing?
    bool does_nothing(int gate_id) {
        return gate_id < 1 || gate_id > 14;
    }

    // Is the gate a multi gate (> 1 input)
    bool is_multi_gate(int gate_id) {
        if (gate_id == X || gate_id == H || gate_id == Y || gate_id == Z ||
            gate_id == S || gate_id == SINV || gate_id == T ||
            gate_id == TINV || gate_id == DETECT) return false;
        return true;
    }
};