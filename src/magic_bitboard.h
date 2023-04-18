#ifndef CHESS_MAGIC_BITBOARD_H
#define CHESS_MAGIC_BITBOARD_H

#include <vector>
#include <random>

#include "types.h"
#include "misc.h"

extern std::mt19937_64 rnd_64;//(time(0));

template<typename Value>
struct Magic {
    Bitboard mask = ~0ULL;
    uint64_t magic_constant = 0;
    uint8_t shift = 0;
    std::vector<Value> data;

    [[nodiscard]] Bitboard index(Bitboard index) const {
        return (((index & mask) * magic_constant) >> shift);
    }

    Value &operator[](Bitboard index) {
        return data[((index & mask) * magic_constant) >> shift];
    }
};

template<typename Value>
Magic<Value> get_magic(uint8_t shift, const std::vector<Bitboard> & indices) {
    Magic<Value> result;
    result.shift = shift;
    result.data.resize((1ULL << (64 - shift)));
    std::vector<size_t> used((1ULL << (64 - shift)), 0);
    size_t version = 0;
    bool correct = false;
    while (!correct) {
        result.magic_constant = rnd_64();
        correct = true;
        ++version;
        for (auto index: indices) {
            if (used[result.index(index)] == version) {
                correct = false;
                break;
            }
            used[result.index(index)] = version;
        }
    }
    return result;
}

template<typename Value>
Magic<Value> get_magic(Bitboard mask, uint8_t added_space) {
    uint8_t len = std::__popcount(mask);
    Magic<Value> result = get_magic<Value>(64 - len - added_space, get_submasks(mask));
    result.mask = mask;
    return result;
}


//#include "magic_bitboard.cpp"

#endif //CHESS_MAGIC_BITBOARD_H
