#ifndef CHESS_CLEAR_ARRAY_H
#define CHESS_CLEAR_ARRAY_H

#include <array>
#include <cstddef>

template<typename Element, size_t Size>
class ClearArray {
public:
    ClearArray() = default;

    void clear() {
        ++version_;
    }
    Element & operator[] (size_t index) {
        if (last_version_[index] != version_) {
            data_[index] = Element();
            last_version_[index] = version_;
        }
        return data_[index];
    }

    const Element & operator[] (size_t index) const {
        if (last_version_[index] != version_) {
            data_[index] = Element();
            last_version_[index] = version_;
        }
        return data_[index];
    }


private:
    mutable std::array<Element, Size> data_;
    mutable std::array<size_t, Size> last_version_{};
    size_t version_ = 1;

};


#endif //CHESS_CLEAR_ARRAY_H
