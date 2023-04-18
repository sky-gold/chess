#include "transposition_table.h"

#include <cassert>

TranspositionTable::TranspositionTable() {
    data_ = new Element[TRANSPOSITION_TABLE_SIZE];
}

TranspositionTable::~TranspositionTable() {
    delete[] data_;
}

TranspositionTable::Element::Element(Hash hash, size_t deep, EvalType evaluation, Move best_move, bool final) : hash(
        hash), deep(deep), evaluation(evaluation), best_move(best_move), final(final) {}

void TranspositionTable::clear() {
    std::unique_lock lock(mtx_);
    for (size_t i = 0; i < TRANSPOSITION_TABLE_SIZE; ++i) {
        data_[i] = Element();
    }
}

TranspositionTable::Element TranspositionTable::get(Hash hash) const {
    std::shared_lock lock(mtx_);
    return data_[hash & (TRANSPOSITION_TABLE_SIZE - 1)];
}

void TranspositionTable::update(TranspositionTable::Element element) {
    std::unique_lock lock(mtx_);
    if (element.final && (element.evaluation == 0 || abs(element.evaluation) > EVAL_MAX)) {
        element.deep = 1000000;
    }
    size_t index = element.hash & (TRANSPOSITION_TABLE_SIZE - 1);
    if (data_[index].hash != element.hash) {
        data_[index] = element;
        return;
    }
    if (data_[index].deep < element.deep) {
        data_[index] = element;
        return;
    }
}