#include "transposition_table.h"

#include <cassert>


void TranspositionTable::clear() {
    ++version_;
}

size_t TranspositionTable::count(Hash hash) const {
    size_t bucket = hash & (TRANSPOSITION_TABLE_SIZE - 1);
    std::lock_guard<std::mutex> lg(mtx_[bucket / MUTEX_BLOCK_SIZE]);
//    std::lock_guard<std::mutex> lg(mtx_);
    if (last_version_[bucket] != version_) return 0;
    size_t i = 0;
    while (i < data_[bucket].size()) {
        if (data_[bucket][i].hash == hash) {
            return data_[bucket][i].deep;
        }
        ++i;
    }
    return 0;
}

void TranspositionTable::update(Hash hash, size_t deep, int evaluation, Move best_move) {
    size_t bucket = hash & (TRANSPOSITION_TABLE_SIZE - 1);
    std::lock_guard<std::mutex> lg(mtx_[bucket / MUTEX_BLOCK_SIZE]);
//    std::lock_guard<std::mutex> lg(mtx_);
    if (last_version_[bucket] != version_) {
        last_version_[bucket] = version_;
        data_[bucket].clear();
    }
    size_t i = 0;
    while (i < data_[bucket].size()) {
        if (data_[bucket][i].hash == hash) {
            if (data_[bucket][i].deep < deep) {
                data_[bucket][i].deep = deep;
                data_[bucket][i].evaluation = evaluation;
                data_[bucket][i].best_move = best_move;
            }
            return;
        }
        ++i;
    }
    data_[bucket].push_back(Element(hash, deep, evaluation, best_move));
}

std::pair<int, Move> TranspositionTable::get(Hash hash) const {
    size_t bucket = hash & (TRANSPOSITION_TABLE_SIZE - 1);
    std::lock_guard<std::mutex> lg(mtx_[bucket / MUTEX_BLOCK_SIZE]);
//    std::cout << "! " << data_[bucket].size() << std::endl;
//    std::lock_guard<std::mutex> lg(mtx_);
    assert(last_version_[bucket] == version_);
//    if (last_version_[bucket] != version_) return false;
    size_t i = 0;
    while (i < data_[bucket].size()) {
        if (data_[bucket][i].hash == hash) {
            return {data_[bucket][i].evaluation, data_[bucket][i].best_move};
        }
        ++i;
    }
    assert(0);
    return {0, MOVE_NULL};
}