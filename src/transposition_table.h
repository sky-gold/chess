#ifndef CHESS_TRANSPOSITION_TABLE_H
#define CHESS_TRANSPOSITION_TABLE_H

#include <array>
#include <vector>
#include <thread>
#include <mutex>

#include "types.h"

const size_t TRANSPOSITION_TABLE_SIZE = (1ULL << 23);
const size_t MUTEX_BLOCK_SIZE = TRANSPOSITION_TABLE_SIZE / 32;

class TranspositionTable {
public:
    struct Element {
        Element(Hash hash, size_t deep, int evaluation, Move best_move) : hash(hash), deep(deep),
                                                                          evaluation(evaluation),
                                                                          best_move(best_move) {};
        Hash hash = 0;
        size_t deep = 0;
        int evaluation = 0;
        Move best_move = MOVE_NULL;
    };
    void clear();

    size_t count(Hash hash) const;

    void update(Hash hash, size_t deep, int evaluation, Move best_move = MOVE_NULL);

    std::pair<int, Move> get(Hash hash) const;

private:


    mutable std::array<std::mutex, TRANSPOSITION_TABLE_SIZE / MUTEX_BLOCK_SIZE> mtx_;
//    mutable std::mutex mtx_;
    std::array<std::vector<Element>, TRANSPOSITION_TABLE_SIZE> data_;
    std::array<size_t, TRANSPOSITION_TABLE_SIZE> last_version_;
    size_t version_ = 0;
};

#include "transposition_table.cpp"

#endif //CHESS_TRANSPOSITION_TABLE_H
