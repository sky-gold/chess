#include "move_list.h"

MoveList::MoveList() = default;

void MoveList::push_back(Move move) {
    data_[len_++] = move;
}

Move &MoveList::operator[](uint8_t index) {
    return data_[index];
}

Move MoveList::operator[](uint8_t index) const {
    return data_[index];
}

Move *MoveList::begin() {
    return data_.begin();
}

const Move *MoveList::begin() const {
    return data_.begin();
}

Move *MoveList::end() {
    return data_.begin() + len_;
}

const Move *MoveList::end() const {
    return data_.begin() + len_;
}

size_t MoveList::size() const {
    return len_;
}

bool MoveList::empty() const {
    return (len_ == 0);
}

void MoveList::clear() {
    len_ = 0;
}