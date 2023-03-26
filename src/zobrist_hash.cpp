#include "zobrist_hash.h"

namespace zobrist_hashing {
    Hash figure[64][MAX_FIGURE];
    Hash castling[16];
    Hash passant[65];
    Hash turn[2];

    std::mt19937_64 rnd_64(0);

    void init() {
        for (auto & pos : figure) {
            for (Hash & val : pos) {
                val = rnd_64();
            }
        }
        for (Hash & val : castling) {
            val = rnd_64();
        }
        for (Hash & val : passant) {
            val = rnd_64();
        }
        turn[0] = rnd_64();
        turn[1] = rnd_64();
    }
}