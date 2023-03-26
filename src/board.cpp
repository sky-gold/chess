#include "board.h"

#include <iostream>

Board::Board(std::string fen_string) {
//    std::cout << "version_=" << version_ << std::endl;
    for (size_t i = 0; i < 64; ++i) {
        board_[i] = FIGURE_NULL;
    }
    std::vector<std::string> words;
    std::string word;
    for (auto to: fen_string) {
        if (to == ' ') {
            if (!word.empty()) {
                words.push_back(word);
            }
            word = "";
        } else {
            word.push_back(to);
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    assert(words.size() == 6);
    std::string board_string = words[0];
    if (words[1] == "w") {
        turn_ = WHITE;
    } else {
        turn_ = BLACK;
    }
    castling_flag_ = 0;
    for (auto to: words[2]) {
        if (to == 'K') {
            castling_flag_ |= (1 << 3);
        }
        if (to == 'Q') {
            castling_flag_ |= (1 << 2);
        }
        if (to == 'k') {
            castling_flag_ |= (1 << 1);
        }
        if (to == 'q') {
            castling_flag_ |= (1 << 0);
        }

    }
    std::string passant_string = words[3];
    if (words[3] == "-") {
        passant_flag_ = SQUARE_NULL;
    } else {
        passant_flag_ = (words[3][0] - 'a') + (words[3][1] - '1') * 8;
    }
    quiet_count_ = stoi(words[4]);
    step_count_ = stoi(words[5]);
    int x = 7;
    int y = 0;
    int i = 0;
//    std::cout << "board_string=" << board_string << std::endl;
    while (i < board_string.size()) {
        if ('1' <= board_string[i] && board_string[i] <= '8') {
            y += board_string[i] - '0';
        } else if (board_string[i] == '/') {
            --x;
            y = 0;
        } else {
            board_[x * 8 + y] = from_char_to_figure(board_string[i]);
            ++y;
        }
        ++i;
    }

    for (int pos = 0; pos < 64; ++pos) {
        if (get_figure(pos) != FIGURE_NULL) {
            hash_ ^= zobrist_hashing::figure[pos][get_figure(pos)];
            color_bitboard_[get_figure(pos) & 1] |= (1ULL << pos);
        }
        figure_bitboard_[get_figure(pos)] |= (1ULL << pos);
    }
    hash_ ^= zobrist_hashing::passant[passant_flag_];
    hash_ ^= zobrist_hashing::castling[castling_flag_];
    repetition_history_.add_pos(hash());
//    calc_legal_moves_();
}

std::string Board::fen() const {
    std::string res;
    std::string board_string;
    int x = 0;
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            uint8_t pos = 8 * i + j;
            if (get_figure(pos) == FIGURE_NULL) {
                ++x;
            } else {
                if (x != 0) {
                    board_string.push_back(x + '0');
                    x = 0;
                }
                board_string.push_back(from_figure_to_char(get_figure(pos)));
            }
        }
        if (x != 0) {
            board_string.push_back(x + '0');
            x = 0;
        }
        if (i != 0) {
            board_string.push_back('/');
        }
    }
    res += board_string;
    if (turn() == 0) {
        res += " w";
    } else {
        res += " b";
    }
    if (castling_flag_ == 0) {
        res += " -";
    } else {
        std::string castling_string;
        if ((castling_flag_ >> 3) & 1) castling_string += 'K';
        if ((castling_flag_ >> 2) & 1) castling_string += 'Q';
        if ((castling_flag_ >> 1) & 1) castling_string += 'k';
        if ((castling_flag_ >> 0) & 1) castling_string += 'q';
        res += " ";
        res += castling_string;
    }
    if (passant_flag_ == SQUARE_NULL) {
        res += " -";
    } else {
        res += " ";
        res.push_back(passant_flag_ % 8 + 'a');
        res.push_back(passant_flag_ / 8 + '1');
    }
    res += " 0";
    res += " " + std::to_string(step_count());
    return res;
}

Figure Board::get_figure(Square pos) const {
    return board_[pos];
}

Bitboard Board::figure_bitboard(Figure figure) const {
    return figure_bitboard_[figure];
}

Bitboard Board::color_bitboard(uint8_t color) const {
    return color_bitboard_[color];
}

bool Board::turn() const {
    return turn_;
}

size_t Board::step_count() const {
    return step_count_;
}

bool Board::is_50rule() const {
    return (quiet_count_ >= MAX_QUIET_MOVES);
}

bool Board::is_repetition_rule() const {
    return (repetition_history_.count(hash()) >= REPETITION_RULE);
}

Board::State Board::state() const {
    if (is_50rule() || is_repetition_rule()) return State::Draw;
    if (((color_bitboard_[WHITE] - 1) & color_bitboard_[WHITE]) == 0 &&
        ((color_bitboard_[BLACK] - 1) & color_bitboard_[BLACK]) == 0) {
        return State::Draw;
    }
    if (get_legal_moves().empty()) {
        Square king_pos;
        if (turn()) {
            king_pos = lsb(figure_bitboard_[BLACK | KING]);
        } else {
            king_pos = lsb(figure_bitboard_[WHITE | KING]);
        }
        if (is_attacked_(king_pos, (turn_ ^ 1))) {
            if (turn()) {
                return State::WhiteWin;
            } else {
                return State::BlackWin;
            }
        }
        return State::Draw;
    }
    return State::None;
}

Move Board::last_move() const {
    if (moves_.empty()) return MOVE_NULL;
    return moves_.back().first;
}

Hash Board::hash() const {
    return hash_ ^ zobrist_hashing::turn[turn_];
}

MoveList Board::get_legal_moves() const {
    if (legal_moves_calced) {
        return legal_moves_;
    }
    calc_legal_moves_();
    legal_moves_calced = true;
    return legal_moves_;
}

bool Board::is_legal(Move move) const {
    for (auto legal_move: get_legal_moves()) {
        if (legal_move == move) {
            return true;
        }
    }
    return false;
}

void Board::change_pos_(Square pos, Figure figure) {
    hash_ ^= zobrist_hashing::figure[pos][get_figure(pos)];
    if (board_[pos] != FIGURE_NULL) {
        color_bitboard_[board_[pos] & 1] ^= (1ULL << pos);
    }
    figure_bitboard_[board_[pos]] ^= (1ULL << pos);
    board_[pos] = figure;
    hash_ ^= zobrist_hashing::figure[pos][board_[pos]];
    if (board_[pos] != FIGURE_NULL) {
        color_bitboard_[board_[pos] & 1] ^= (1ULL << pos);
    }
    figure_bitboard_[board_[pos]] ^= (1ULL << pos);
}

void Board::do_move(Move move) {
//    std::cout << "do_move " << get_string_move(move) << std::endl;
    ++step_count_;
    Square pos1 = move & ((1 << 6) - 1);
    Square pos2 = (move >> 6) & ((1 << 6) - 1);
    uint8_t move_flag = (move >> 12);
    Changes changes;
    changes.old_castling_flag = castling_flag_;
    changes.old_passant_flag = passant_flag_;
    changes.old_quiet_count = quiet_count_;
    changes.pos = pos2;
    changes.figure = board_[pos2];
    bool is_quiet = true;
    if (move_flag != 0 || ((get_figure(pos1) >> 1) << 1) == PAWN || get_figure(pos2) != FIGURE_NULL) {
        is_quiet = false;
    }
    hash_ ^= zobrist_hashing::passant[passant_flag_];
    passant_flag_ = SQUARE_NULL;
    if (get_figure(pos1) == (WHITE | PAWN) && pos2 == pos1 + 16) {
        passant_flag_ = (pos1 + 8);
    }
    if (get_figure(pos1) == (BLACK | PAWN) && pos2 == pos1 - 16) {
        passant_flag_ = (pos1 - 8);
    }
    hash_ ^= zobrist_hashing::passant[passant_flag_];
    change_pos_(pos2, board_[pos1]);
    change_pos_(pos1, FIGURE_NULL);
    if (move_flag) {
        if (move_flag == 1) {
            change_pos_(3, get_figure(0));
            change_pos_(0, FIGURE_NULL);
        } else if (move_flag == 2) {
            change_pos_(5, get_figure(7));
            change_pos_(7, FIGURE_NULL);
        } else if (move_flag == 3) {
            change_pos_(3 + 56, get_figure(0 + 56));
            change_pos_(0 + 56, FIGURE_NULL);
        } else if (move_flag == 4) {
            change_pos_(5 + 56, get_figure(7 + 56));
            change_pos_(7 + 56, FIGURE_NULL);
        } else if (move_flag == 6) {
            changes.pos = pos2 - 8;
            changes.figure = board_[pos2 - 8];
            change_pos_(pos2 - 8, FIGURE_NULL);
        } else if (move_flag == 7) {
            changes.pos = pos2 + 8;
            changes.figure = board_[pos2 + 8];
            change_pos_(pos2 + 8, FIGURE_NULL);
        } else if (move_flag == 10) {
//            undo_pos_(pos2);
            change_pos_(pos2, QUEEN | turn_);
        } else if (move_flag == 11) {
//            undo_pos_(pos2);
            change_pos_(pos2, ROOK | turn_);
        } else if (move_flag == 12) {
//            undo_pos_(pos2);
            change_pos_(pos2, BISHOP | turn_);
        } else if (move_flag == 13) {
//            undo_pos_(pos2);
            change_pos_(pos2, KNIGHT | turn_);
        }
    }
    hash_ ^= zobrist_hashing::castling[castling_flag_];
    if ((color_bitboard_[WHITE] & ((1ULL << 0) | (1ULL << 4))) != ((1ULL << 0) | (1ULL << 4))) {
        castling_flag_ &= ~(1ULL << 2);
    }
    if ((color_bitboard_[WHITE] & ((1ULL << 4) | (1ULL << 7))) != ((1ULL << 4) | (1ULL << 7))) {
        castling_flag_ &= ~(1ULL << 3);
    }
    if ((color_bitboard_[BLACK] & ((1ULL << 56) | (1ULL << 60))) != ((1ULL << 56) | (1ULL << 60))) {
        castling_flag_ &= ~(1ULL << 0);
    }
    if ((color_bitboard_[BLACK] & ((1ULL << 63) | (1ULL << 60))) != ((1ULL << 63) | (1ULL << 60))) {
        castling_flag_ &= ~(1ULL << 1);
    }
    hash_ ^= zobrist_hashing::castling[castling_flag_];
    turn_ ^= 1;
    if (is_quiet) {
        ++quiet_count_;
        repetition_history_.add_pos(hash());
    } else {
        quiet_count_ = 0;
        repetition_history_.clear();
        repetition_history_.add_pos(hash());
    }
//    std::cout << "turn_=" << turn_ << std::endl;

//    std::cout << "turn_=" << turn_ << std::endl;
    moves_.emplace_back(move, changes);
    legal_moves_calced = false;

}

void Board::undo_last_move() {
//    std::cout << "undo_last_move" << std::endl;
    --step_count_;
    Move move = moves_.back().first;
    Changes changes = moves_.back().second;
    Square pos1 = move & ((1 << 6) - 1);
    Square pos2 = (move >> 6) & ((1 << 6) - 1);
    uint8_t move_flag = (move >> 12);
    change_pos_(pos1, board_[pos2]);
    change_pos_(pos2, FIGURE_NULL);
    if (move_flag > 0) {
        if (move_flag == 1) {
            change_pos_(0, board_[3]);
            change_pos_(3, FIGURE_NULL);
        } else if (move_flag == 2) {
            change_pos_(7, board_[5]);
            change_pos_(5, FIGURE_NULL);
        } else if (move_flag == 3) {
            change_pos_(0 + 56, board_[3 + 56]);
            change_pos_(3 + 56, FIGURE_NULL);
        } else if (move_flag == 4) {
            change_pos_(7 + 56, board_[5 + 56]);
            change_pos_(5 + 56, FIGURE_NULL);
        } else if (move_flag >= 10) {
            change_pos_(pos1, PAWN | (board_[pos1] & 1));
        }
    }
    if (changes.pos != SQUARE_NULL) {
        change_pos_(changes.pos, changes.figure);
    }
    repetition_history_.undo();
    turn_ ^= 1;

    hash_ ^= zobrist_hashing::passant[passant_flag_];
    passant_flag_ = changes.old_passant_flag;
    hash_ ^= zobrist_hashing::passant[passant_flag_];

    hash_ ^= zobrist_hashing::castling[castling_flag_];
    castling_flag_ = changes.old_castling_flag;
    hash_ ^= zobrist_hashing::castling[castling_flag_];

    quiet_count_ = changes.old_quiet_count;
    moves_.pop_back();
    legal_moves_calced = false;
}

bool Board::is_attacked_(Square pos, uint8_t by_color) const {
    if (pawn_logic::pawn_attack_bitboard[pos][by_color ^ 1] & figure_bitboard_[PAWN | by_color]) {
        return true;
    }
    if (king_logic::get_moves_bitboard(pos, color_bitboard_[by_color ^ 1], color_bitboard_[by_color]) &
        figure_bitboard_[KING | by_color]) {
        return true;
    }
    if (knight_logic::get_moves_bitboard(pos, color_bitboard_[by_color ^ 1], color_bitboard_[by_color]) &
        figure_bitboard_[KNIGHT | by_color]) {
        return true;
    }
    if (rook_logic::get_moves_bitboard(pos, color_bitboard_[by_color ^ 1], color_bitboard_[by_color]) &
        (figure_bitboard_[ROOK | by_color] | figure_bitboard_[QUEEN | by_color])) {
        return true;
    }
    if (bishop_logic::get_moves_bitboard(pos, color_bitboard_[by_color ^ 1], color_bitboard_[by_color]) &
        (figure_bitboard_[BISHOP | by_color] | figure_bitboard_[QUEEN | by_color])) {
        return true;
    }
    return false;


}

bool Board::short_castling_() const {
    Bitboard occupied = color_bitboard_[WHITE] | color_bitboard_[BLACK];
    if (turn() == 0) {
        if (!((castling_flag_ >> 3) & 1)) return false;
        if ((occupied >> 5) & 1) return false;
        if ((occupied >> 6) & 1) return false;
        if (is_attacked_(5, turn_ ^ 1)) return false;
        if (is_attacked_(4, turn_ ^ 1)) return false;
        return true;
    } else {
        if (!((castling_flag_ >> 1) & 1)) return false;
        if ((occupied >> (5 + 56)) & 1) return false;
        if ((occupied >> (6 + 56)) & 1) return false;
        if (is_attacked_(5 + 56, turn_ ^ 1)) return false;
        if (is_attacked_(4 + 56, turn_ ^ 1)) return false;
        return true;
    }
}

bool Board::long_castling_() const {
    Bitboard occupied = color_bitboard_[WHITE] | color_bitboard_[BLACK];
    if (turn() == 0) {
        if (!((castling_flag_ >> 2) & 1)) return false;
        if ((occupied >> 3) & 1) return false;
        if ((occupied >> 2) & 1) return false;
        if ((occupied >> 1) & 1) return false;
        if (is_attacked_(3, turn_ ^ 1)) return false;
        if (is_attacked_(4, turn_ ^ 1)) return false;
        return true;
    } else {
        if (!((castling_flag_ >> 0) & 1)) return false;
        if ((occupied >> (3 + 56)) & 1) return false;
        if ((occupied >> (2 + 56)) & 1) return false;
        if ((occupied >> (1 + 56)) & 1) return false;
        if (is_attacked_(3 + 56, turn_ ^ 1)) return false;
        if (is_attacked_(4 + 56, turn_ ^ 1)) return false;
        return true;
    }
}

Square Board::king_pos_() const {
    return lsb(figure_bitboard_[KING | turn_]);
}

void Board::fast_change_pos_(Square pos, Figure figure) const {
    if (board_[pos] != FIGURE_NULL) {
        color_bitboard_[board_[pos] & 1] ^= (1ULL << pos);
    }
    figure_bitboard_[board_[pos]] ^= (1ULL << pos);
    board_[pos] = figure;
    if (board_[pos] != FIGURE_NULL) {
        color_bitboard_[board_[pos] & 1] ^= (1ULL << pos);
    }
    figure_bitboard_[board_[pos]] ^= (1ULL << pos);
}

Board::Changes Board::fast_move_(Move move) const {
    Changes changes;
    Square pos1 = move & ((1 << 6) - 1);
    Square pos2 = (move >> 6) & ((1 << 6) - 1);
    uint8_t move_flag = (move >> 12);
    changes.pos = pos2;
    changes.figure = board_[pos2];
    fast_change_pos_(pos2, board_[pos1]);
    fast_change_pos_(pos1, FIGURE_NULL);
    if (move_flag) {
        if (move_flag == 1) {
            fast_change_pos_(3, get_figure(0));
            fast_change_pos_(0, FIGURE_NULL);
        } else if (move_flag == 2) {
            fast_change_pos_(5, get_figure(7));
            fast_change_pos_(7, FIGURE_NULL);
        } else if (move_flag == 3) {
            fast_change_pos_(3 + 56, get_figure(0 + 56));
            fast_change_pos_(0 + 56, FIGURE_NULL);
        } else if (move_flag == 4) {
            fast_change_pos_(5 + 56, get_figure(7 + 56));
            fast_change_pos_(7 + 56, FIGURE_NULL);
        } else if (move_flag == 6) {
            changes.pos = pos2 - 8;
            changes.figure = board_[pos2 - 8];
            fast_change_pos_(pos2 - 8, FIGURE_NULL);
        } else if (move_flag == 7) {
            changes.pos = pos2 + 8;
            changes.figure = board_[pos2 + 8];
            fast_change_pos_(pos2 + 8, FIGURE_NULL);
        } else if (move_flag == 10) {
            fast_change_pos_(pos2, QUEEN | turn_);
        } else if (move_flag == 11) {
            fast_change_pos_(pos2, ROOK | turn_);
        } else if (move_flag == 12) {
            fast_change_pos_(pos2, BISHOP | turn_);
        } else if (move_flag == 13) {
            fast_change_pos_(pos2, KNIGHT | turn_);
        }
    }
//    turn_ ^= 1;
    return changes;
}

void Board::fast_undo_(Move move, Changes changes) const {
    Square pos1 = move & ((1 << 6) - 1);
    Square pos2 = (move >> 6) & ((1 << 6) - 1);
    uint8_t move_flag = (move >> 12);
    fast_change_pos_(pos1, board_[pos2]);
    fast_change_pos_(pos2, FIGURE_NULL);
    if (move_flag > 0) {
        if (move_flag == 1) {
            fast_change_pos_(0, board_[3]);
            fast_change_pos_(3, FIGURE_NULL);
        } else if (move_flag == 2) {
            fast_change_pos_(7, board_[5]);
            fast_change_pos_(5, FIGURE_NULL);
        } else if (move_flag == 3) {
            fast_change_pos_(0 + 56, board_[3 + 56]);
            fast_change_pos_(3 + 56, FIGURE_NULL);
        } else if (move_flag == 4) {
            fast_change_pos_(7 + 56, board_[5 + 56]);
            fast_change_pos_(5 + 56, FIGURE_NULL);
        } else if (move_flag >= 10) {
            fast_change_pos_(pos1, PAWN | (board_[pos1] & 1));
        }
    }
    if (changes.pos != SQUARE_NULL) {
        fast_change_pos_(changes.pos, changes.figure);
    }
//    turn_ ^= 1;
}

void Board::add_to_legal_(const std::vector<uint16_t> &moves) const {
    for (auto move: moves) {
        add_to_legal_(move);;
    }
}

void Board::add_to_legal_(uint16_t move) const {
    if (move == MOVE_NULL) return;
//    std::cout << "add_to_legal_ " << get_string_move(move) << std::endl;
    Changes changes = fast_move_(move);
    Square turn_king = lsb(figure_bitboard_[KING | turn_]);
    if (!is_attacked_(turn_king, turn_ ^ 1)) {
//        std::cout << "OK" << std::endl;
        legal_moves_.push_back(move);
    }
    fast_undo_(move, changes);
}

void Board::calc_legal_moves_() const {
//    std::cout << "calc_legal_moves" << std::endl;
    legal_moves_.clear();
    if (is_50rule() || is_repetition_rule()) return;
    if (((color_bitboard_[WHITE] - 1) & color_bitboard_[WHITE]) == 0 &&
        ((color_bitboard_[BLACK] - 1) & color_bitboard_[BLACK]) == 0) {
        return;// State::Draw;
    }
    if (long_castling_()) {
        if (turn()) {
            add_to_legal_(create_move(60, 58, 3));
        } else {
            add_to_legal_(create_move(4, 2, 1));
        }
    }
    if (short_castling_()) {
        if (turn()) {
            add_to_legal_(create_move(60, 62, 4));
        } else {
            add_to_legal_(create_move(4, 6, 2));
        }
    }
    if (passant_flag_ != SQUARE_NULL) {
        add_to_legal_(pawn_logic::get_passant(passant_flag_, figure_bitboard_[PAWN | turn_]));
    }
    Bitboard turn_bitboard = color_bitboard_[turn_];
    Bitboard opposite_bitboard = color_bitboard_[turn_ ^ 1];
    std::vector<Move> pawn_moves = pawn_logic::get_moves(figure_bitboard_[PAWN | turn_], turn_bitboard,
                                                         opposite_bitboard, turn_);
    add_to_legal_(pawn_moves);
    add_to_legal_(king_logic::get_moves(king_pos_(), turn_bitboard, opposite_bitboard));
    Bitboard queen = figure_bitboard_[QUEEN | turn_];
    while (queen > 0) {
        std::pair<const std::vector<Move> &, const std::vector<Move> &> queen_moves = queen_logic::get_moves(
                pop_lsb(queen), turn_bitboard, opposite_bitboard);
        add_to_legal_(queen_moves.first);
        add_to_legal_(queen_moves.second);
    }
    Bitboard rook = figure_bitboard_[ROOK | turn_];
    while (rook > 0) {
        const std::vector<Move> &rook_moves = rook_logic::get_moves(pop_lsb(rook), turn_bitboard, opposite_bitboard);
        add_to_legal_(rook_moves);
    }
    Bitboard bishop = figure_bitboard_[BISHOP | turn_];
    while (bishop > 0) {
        const std::vector<Move> &bishop_moves = bishop_logic::get_moves(pop_lsb(bishop), turn_bitboard,
                                                                        opposite_bitboard);
        add_to_legal_(bishop_moves);
    }
    Bitboard knight = figure_bitboard_[KNIGHT | turn_];
    while (knight > 0) {
        const std::vector<Move> &knight_moves = knight_logic::get_moves(pop_lsb(knight), turn_bitboard,
                                                                        opposite_bitboard);
        add_to_legal_(knight_moves);
    }
}

std::string Board::get_string() const {
    std::cout << "! " << (int)repetition_history_.count(hash()) << std::endl;
    std::cout << "legal_moves=" << std::endl;
    for (auto move: legal_moves_) {
        std::cout << get_string_move(move) << " ";
    }
    std::cout << std::endl;
    std::string result;
    result += "fen=" + fen() + "\n";
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            Square pos = 8 * i + j;
            result.push_back(from_figure_to_char(board_[pos]));
        }
        result.push_back('\n');
    }
    result += "step_count_=" + std::to_string(step_count()) + "\n";
    return result;
}//*/