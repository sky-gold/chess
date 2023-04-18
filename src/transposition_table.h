#ifndef CHESS_TRANSPOSITION_TABLE_H
#define CHESS_TRANSPOSITION_TABLE_H

#include <array>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>

#include "types.h"

class TranspositionTable {
public:
    struct Element {
        Element() = default;
        Element(Hash hash, size_t deep, EvalType evaluation, Move best_move, bool final);
        Hash hash = 0;
        size_t deep = 0;
        EvalType evaluation = 0;
        Move best_move = MOVE_NULL;
        bool final = false;
    };

    TranspositionTable();
    ~TranspositionTable();
    void clear();

    Element get(Hash hash) const;

    void update(Element element);

private:
    constexpr static const size_t TRANSPOSITION_TABLE_SIZE = (1ULL << 20);

    mutable std::shared_mutex mtx_;

    Element* data_;
};

//#include "transposition_table.cpp"

#endif //CHESS_TRANSPOSITION_TABLE_H
