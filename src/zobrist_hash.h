#ifndef CHESS_ZOBRIST_HASH_H
#define CHESS_ZOBRIST_HASH_H

#include <random>

#include "types.h"

namespace zobrist_hashing {
    extern Hash figure[64][MAX_FIGURE];
    extern Hash castling[16];
    extern Hash passant[65];
    extern Hash turn[2];
    void init();
}

#include "zobrist_hash.cpp"

#endif //CHESS_ZOBRIST_HASH_H
