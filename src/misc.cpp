#include "misc.h"

char from_figure_to_char(Figure figure) {
    static const std::map<Figure, char> from_figure_to_char_map = {
            {FIGURE_NULL, '.'},
            {PAWN | BLACK, 'p'},
            {BISHOP | BLACK, 'b'},
            {KNIGHT | BLACK, 'n'},
            {ROOK | BLACK, 'r'},
            {QUEEN | BLACK, 'q'},
            {KING | BLACK, 'k'},
            {PAWN | WHITE, 'P'},
            {BISHOP | WHITE, 'B'},
            {KNIGHT | WHITE, 'N'},
            {ROOK | WHITE, 'R'},
            {QUEEN | WHITE, 'Q'},
            { KING | WHITE, 'K'}
    };
    return from_figure_to_char_map.at(figure);
}

Figure from_char_to_figure(char ch) {
    static const std::map<char, Figure> from_char_to_figure_map = {
            {'.', FIGURE_NULL},
            {'p', PAWN | BLACK},
            {'b', BISHOP | BLACK},
            {'n', KNIGHT | BLACK},
            {'r', ROOK | BLACK},
            {'q', QUEEN | BLACK},
            {'k', KING | BLACK},
            {'P', PAWN | WHITE},
            {'B', BISHOP | WHITE},
            {'N', KNIGHT | WHITE},
            {'R', ROOK | WHITE},
            {'Q', QUEEN | WHITE},
            {'K', KING | WHITE}
    };
    return from_char_to_figure_map.at(ch);
}



std::vector<Bitboard> get_submasks(Bitboard mask) {
    std::vector<uint64_t> bits;
    while (mask > 0) {
        bits.push_back(mask ^ (mask & (mask - 1)));
        mask &= (mask - 1);
    }
    uint8_t len = bits.size();
    std::vector<Bitboard> result;
    for (uint64_t check_mask = 0; check_mask < (1 << len); ++check_mask) {
        Bitboard submask = 0;
        for (size_t i = 0; i < len; ++i) {
            if ((check_mask >> i) & 1) {
                submask |= bits[i];
            }
        }
        result.push_back(submask);
    }
    return result;
}

uint8_t lsb(Bitboard bb) {
    static const uint8_t lookup67[67+1] = {
            64,  0,  1, 39,  2, 15, 40, 23,
            3, 12, 16, 59, 41, 19, 24, 54,
            4, 100, 13, 10, 17, 62, 60, 28,
            42, 30, 20, 51, 25, 44, 55, 47,
            5, 32, 100, 38, 14, 22, 11, 58,
            18, 53, 63,  9, 61, 27, 29, 50,
            43, 46, 31, 37, 21, 57, 52,  8,
            26, 49, 45, 36, 56,  7, 48, 35,
            6, 34, 33, 100 };
    return lookup67[(bb & -bb) % 67];
}

uint8_t pop_lsb(Bitboard & bb) {
    uint8_t id = lsb(bb);
    bb ^= (1ULL << id);
    return id;
}

std::string bitboard_string(Bitboard b) {
    std::string res;
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            res.push_back('0' + ((b >> (i * 8 + j)) & 1));
        }
        res += "\n";
    }
    return res;
}

uint16_t create_move(Square pos1, Square pos2, uint8_t move_flag) {
    return pos1 + (pos2 << 6) + (move_flag << 12);
}

std::string get_string_move(Move move) {
    std::string res;
    Square pos1 = move & ((1ULL << 6) - 1);
    Square pos2 = (move >> 6) & ((1ULL << 6) - 1);
    uint8_t move_flag = move >> 12;
    res.push_back('a' + (pos1 % 8));
    res.push_back('1' + (pos1 / 8));
    res.push_back('a' + (pos2 % 8));
    res.push_back('1' + (pos2 / 8));
    if (move_flag == 10) res.push_back('q');
    if (move_flag == 11) res.push_back('r');
    if (move_flag == 12) res.push_back('b');
    if (move_flag == 13) res.push_back('n');
    return res;
}

uint8_t popcount(Bitboard b) {
    return __builtin_popcountll(b);
}