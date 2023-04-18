#include "app.h"

#define TIME_SELECTOR_MAX 15000

App::App() {
    window_.create(sf::VideoMode(1200, 800), "chess");
    figure_texture.loadFromFile("src/img/figures.png");
    font_.loadFromFile("src/fonts/font.ttf");
    time_selector_.setSize(sf::Vector2f(320.f, 20.f));
    black_turn_sprite_.setTexture(figure_texture);
    white_turn_sprite_.setTexture(figure_texture);
}

std::pair<uint8_t, uint8_t> App::get_coordinates(Square sq) const {
    if (board_flip) sq = 63 - sq;
    uint8_t i = sq / 8;
    uint8_t j = sq % 8;
    return {j, (7-i)};
}

void App::run() {
    sf::Clock clock;

    float FPS = 200;

    while (window_.isOpen()) {
//        std::cout << "hash=" << board_.hash() << std::endl;
//        std::cout << board_.get_string() << std::endl;
        if (clock.getElapsedTime().asMicroseconds() < 1e6 / FPS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        clock.restart();
//        std::cout << "ok2" << std::endl;
        update_();
//        std::cout << "ok3" << std::endl;
        draw_();
//        std::cout << "ok4" << std::endl;

        window_.display();
//        std::cout << "ok5" << std::endl;
    }
}

void App::update_() {
//    std::cout << "update_()" << std::endl;
//    std::cout << "# " << reset_board_ << " " << undo_move_ << std::endl;
//    std::cout << board_.get_string() << std::endl;
    if (human_turn_ == WHITE) {
        board_flip = false;
    } else {
        board_flip = true;
    }
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window_.close();
    }
    if (reset_board_) {
        if (search_container.is_working()) {
            search_container.stop();
        }
        board_ = Board(fen_start);
        reset_board_ = false;
        undo_move_ = false;
        human_turn_ = WHITE;
        choosed_square_ = SQUARE_NULL;
    }
    if (undo_move_) {
        if (search_container.is_working()) {
            search_container.stop();
        }
        if (board_.last_move() != MOVE_NULL) {
            board_.undo_last_move();
        }
        if (human_turn_ != 2 && board_.turn() != human_turn_ && board_.last_move() != MOVE_NULL) {
            board_.undo_last_move();
        }
        undo_move_ = false;
    }
    Move make_move = MOVE_NULL;
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window_);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        for (Square pos = 0; pos < 64; ++pos) {
            if (board_sprites_[pos].getGlobalBounds().contains((float) mouse_pos.x, (float) mouse_pos.y)) {
                if (board_.get_figure(pos) == FIGURE_NULL || board_.turn() != (board_.get_figure(pos) & 1)) {
                    for (Move move: board_.get_legal_moves()) {
                        Square pos1 = move & ((1ULL << 6) - 1);
                        Square pos2 = (move >> 6) & ((1ULL << 6) - 1);
                        uint8_t move_flag = (move >> 12);
                        if (move_flag >= 10 && move_flag != promote_to) continue;
                        if (pos1 == choosed_square_ && pos2 == pos) {
                            make_move = move;
                            continue;
                        }
                    }
                } else {
                    choosed_square_ = pos;
                }
                continue;
            }
        }
        if (time_selector_.getGlobalBounds().contains((float)mouse_pos.x, (float)mouse_pos.y)) {
            choosed_time_ = (float) (mouse_pos.x - time_selector_.getPosition().x) / (float)time_selector_.getSize().x;
        }
        if (mouse_click_flag_ && promote_sprite_.getGlobalBounds().contains((float)mouse_pos.x, (float)mouse_pos.y)) {
            ++promote_to;
            if (promote_to >= 14) promote_to = 10;
        }
        if (mouse_click_flag_ && white_turn_sprite_.getGlobalBounds().contains((float)mouse_pos.x, (float)mouse_pos.y)) {
            if (human_turn_ == WHITE) {
                human_turn_ = 2;
            } else {
                human_turn_ = WHITE;
            }
        }
        if (mouse_click_flag_ && black_turn_sprite_.getGlobalBounds().contains((float)mouse_pos.x, (float)mouse_pos.y)) {
            if (human_turn_ == BLACK) {
                human_turn_ = 2;
            } else {
                human_turn_ = BLACK;
            }
        }
        if (mouse_click_flag_ && undo_button_.getGlobalBounds().contains((float)mouse_pos.x, (float)mouse_pos.y)) {
            undo_move_ = true;
        }
        if (mouse_click_flag_ && reset_button_.getGlobalBounds().contains((float)mouse_pos.x, (float)mouse_pos.y)) {
            reset_board_ = true;
        }
        mouse_click_flag_ = false;
    } else {
        mouse_click_flag_ = true;
    }
    //DOING MOVE
    if (board_.state() == Board::State::None) {
        if (board_.state() == Board::State::None && board_.turn() == human_turn_ && make_move != MOVE_NULL && board_.is_legal(make_move)) {
            board_.do_move(make_move);
            choosed_square_ = SQUARE_NULL;
        }
        if (board_.state() == Board::State::None && board_.turn() != human_turn_ && !(search_container.is_working())) {
            search_container.set_board(board_);
            search_start_ = clock();
        }
        auto max_time = (clock_t) (TIME_SELECTOR_MAX * choosed_time_);
        if (search_container.is_working() && clock() > search_start_ + max_time) {
            search_container.stop();
            Move move = search_container.get_move();
            board_.do_move(move);
            choosed_square_ = SQUARE_NULL;
        }
    }
}



void App::draw_() {
    window_.clear(sf::Color(0, 0, 0));

    draw_undo_();
    draw_reset_();
    draw_turn_selector_();
    draw_game_result_();
    draw_promote_selector_();
    draw_time_selector_();
    draw_board_();
    draw_last_move_();
    draw_possible_moves_();
    draw_figures_();
}

void App::draw_undo_() {
    undo_button_.setSize(sf::Vector2f(280.f, 80.f));
    undo_button_.setFillColor(sf::Color(220, 220, 220));
    undo_button_.setPosition(840.f, 450.f);
    window_.draw(undo_button_);
    sf::Text text("", font_, 40);
//    text.setStyle(sf::Text::Style::Bold);
    text.setString("UNDO");
    text.setPosition(840.f + 140.f - (text.getLocalBounds().width / 2), 450.f + 40.f - 40.f + (text.getLocalBounds().height / 2));
    if (undo_move_) {
        text.setFillColor(sf::Color(150, 150, 150));
    } else {
        text.setFillColor(sf::Color::Black);
    }
    window_.draw(text);
}

void App::draw_reset_() {
    reset_button_.setSize(sf::Vector2f(280.f, 80.f));
    reset_button_.setFillColor(sf::Color(220, 220, 220));
    reset_button_.setPosition(840.f, 580.f);
    window_.draw(reset_button_);
    sf::Text text("", font_, 40);
    text.setString("RESET");
    text.setPosition(840.f + 140.f - (text.getLocalBounds().width / 2), 580.f + 40.f - 40.f + (text.getLocalBounds().height / 2));
    if (reset_board_) {
        text.setFillColor(sf::Color(150, 150, 150));
    } else {
        text.setFillColor(sf::Color::Black);
    }
    window_.draw(text);
}

void App::draw_turn_selector_() {
    uint8_t figure_type_index, figure_color_index;


    white_turn_sprite_.setTextureRect(
            sf::IntRect(((KING >> 1) - 1) * 80.f, WHITE * 80.f, 80, 80));
    black_turn_sprite_.setTextureRect(
            sf::IntRect(((KING >> 1) - 1) * 80.f, BLACK * 80.f, 80, 80));
    sf::Text turn_text("", font_, 25);
    turn_text.setString("YOUR COLOR:");
    turn_text.setPosition(800.f, 290.f + 40.f - 10.f - (turn_text.getLocalBounds().height / 2) + 30.f);
    window_.draw(turn_text);
    sf::RectangleShape board_square(sf::Vector2f(80.f, 80.f));
    if (human_turn_ != WHITE) {
        board_square.setFillColor(sf::Color(200, 200, 200));
    } else {
        board_square.setFillColor(sf::Color(0, 200, 0));
    }
    board_square.setPosition(800.f + turn_text.getLocalBounds().width + 10.f, 290.f + 30.f);
    white_turn_sprite_.setPosition(800.f + turn_text.getLocalBounds().width + 10.f, 290.f + 30.f);
    window_.draw(board_square);
    window_.draw(white_turn_sprite_);
    if (human_turn_ != BLACK) {
        board_square.setFillColor(sf::Color(200, 200, 200));
    } else {
        board_square.setFillColor(sf::Color(0, 200, 0));
    }
    board_square.setPosition(800.f + turn_text.getLocalBounds().width + 10.f + 100.f, 290.f + 30.f);
    black_turn_sprite_.setPosition(800.f + turn_text.getLocalBounds().width + 10.f + 100.f, 290.f + 30.f);
    window_.draw(board_square);
    window_.draw(black_turn_sprite_);
}

void App::draw_promote_selector_() {
    sf::Text promote_text("", font_, 25);
    promote_text.setFillColor(sf::Color::White);
    promote_text.setString("PROMOTE TO:");
    promote_text.setPosition(800.f, 180.f - (promote_text.getLocalBounds().height / 2) + 30.f);
    window_.draw(promote_text);

    sf::RectangleShape board_square(sf::Vector2f(80.f, 80.f));
    board_square.setFillColor(sf::Color(200, 200, 200));
    board_square.setPosition(800.f + promote_text.getLocalBounds().width + 10, 150.f + 30.f);
    window_.draw(board_square);

    promote_sprite_.setTexture(figure_texture);
    Figure figure = QUEEN;
    if (promote_to == 11) figure = ROOK;
    if (promote_to == 12) figure = BISHOP;
    if (promote_to == 13) figure = KNIGHT;
    if (human_turn_ == BLACK) figure |= 1;
    uint8_t figure_type_index = (figure >> 1) - 1;
    uint8_t figure_color_index = (figure & 1);
    promote_sprite_.setTextureRect(
            sf::IntRect(figure_type_index * 80.f, figure_color_index * 80.f, 80, 80));
    promote_sprite_.setPosition(800.f + promote_text.getLocalBounds().width + 10, 150.f + 30.f);
    window_.draw(promote_sprite_);
}

void App::draw_game_result_() {
    if (board_.state() != Board::State::None) {
        sf::Text result_text("", font_, 30);
        result_text.setFillColor(sf::Color::White);
        result_text.setStyle(sf::Text::Style::Bold);
        if (board_.state() == Board::BlackWin) {
            result_text.setString("BLACK WIN");
        } else if (board_.state() == Board::WhiteWin) {
            result_text.setString("WHITE WIN");
        } else if (board_.state() == Board::Draw) {
            result_text.setString("DRAW");
        }
        result_text.setPosition(400.f - result_text.getLocalBounds().width / 2, 20.f - result_text.getLocalBounds().height / 2);
        window_.draw(result_text);
        sf::Text explanation_text("", font_, 20);
        explanation_text.setFillColor(sf::Color::White);
        explanation_text.setStyle(sf::Text::Style::Italic);
        if (board_.state() == Board::BlackWin || board_.state() == Board::WhiteWin) {
            explanation_text.setString("by checkmate");
        } else {
            if (board_.is_50rule()) {
                explanation_text.setString("by fifty-move rule");
            } else if (board_.is_repetition_rule()) {
                explanation_text.setString("by repetition rule");
            } else if (popcount(board_.color_bitboard(WHITE) | board_.color_bitboard(BLACK))) {
                explanation_text.setString("impossibility of checkmate");
            } else {
                explanation_text.setString("by stalemate");
            }
        }
        explanation_text.setPosition(400.f - explanation_text.getLocalBounds().width / 2, (20.f + result_text.getLocalBounds().height / 2 + 80.f) / 2 - explanation_text.getLocalBounds().height / 2);
        window_.draw(explanation_text);
    }
}

void App::draw_time_selector_() {
    time_selector_.setFillColor(sf::Color::White);
    time_selector_.setPosition(800.f, 80.f);
    window_.draw(time_selector_);

    sf::RectangleShape rect(sf::Vector2f(320.f * choosed_time_, 20.f));
    rect.setFillColor(sf::Color::Red);
    rect.setPosition(800.f, 80.f);
    window_.draw(rect);

    if (search_container.is_working()) {
        float passed_time = ((float)(clock() - search_start_) / TIME_SELECTOR_MAX);
        rect.setSize(sf::Vector2f(320.f * passed_time, 20.f));
        rect.setFillColor(sf::Color::Green);
        rect.setPosition(800.f, 80.f);
        window_.draw(rect);
    }
    sf::Text time_text("", font_, 20);
    time_text.setFillColor(sf::Color::White);
    auto max_time = (int) (TIME_SELECTOR_MAX * choosed_time_);
    time_text.setString("AI has " + std::to_string(max_time) + "ms to think");
    time_text.setPosition(800.f, 120.f);
    window_.draw(time_text);
}

void App::draw_board_() {
    sf::RectangleShape board_square(sf::Vector2f(80.f, 80.f));
    for (Square sq = 0; sq < 64; ++sq) {
        std::pair<uint8_t, uint8_t> coord = get_coordinates(sq);
        if (((sq / 8) + (sq % 8)) & 1) {
            board_square.setFillColor(sf::Color(240, 217, 181));
        } else {
            board_square.setFillColor(sf::Color(181, 136, 99));
        }
        board_square.setPosition(80.f + 80.f * (float)coord.first, 80.f + 80.f * (float)coord.second);
        window_.draw(board_square);
    }
}

void App::draw_last_move_() {
    Move last_move = board_.last_move();
    if (last_move == MOVE_NULL) return;
    Square pos1 = last_move & ((1ULL << 6) - 1);
    Square pos2 = (last_move >> 6) & ((1ULL << 6) - 1);
    sf::RectangleShape board_square(sf::Vector2f(80.f, 80.f));
    board_square.setFillColor(sf::Color(200, 200, 0, 100));
    std::pair<uint8_t, uint8_t> coord1 = get_coordinates(pos1);
    std::pair<uint8_t, uint8_t> coord2 = get_coordinates(pos2);
    board_square.setPosition(80.f + 80.f * (float)coord1.first, 80.f + 80.f * (float)coord1.second);
    window_.draw(board_square);
    board_square.setPosition(80.f + 80.f * (float)coord2.first, 80.f + 80.f * (float)coord2.second);
    window_.draw(board_square);

}

void App::draw_possible_moves_() {
    if (choosed_square_ == SQUARE_NULL) return;
    sf::RectangleShape board_square(sf::Vector2f(80.f, 80.f));
    board_square.setFillColor(sf::Color(20, 20, 220, 120));
    std::pair<uint8_t, uint8_t> coord = get_coordinates(choosed_square_);
    board_square.setPosition(80.f + 80.f * (float)coord.first, 80.f + 80.f * (float)coord.second);
    window_.draw(board_square);
    board_square.setFillColor(sf::Color(20, 220, 20, 120));
    for (auto move: board_.get_legal_moves()) {
        Square pos1 = move & ((1ULL << 6) - 1);
        Square pos2 = (move >> 6) & ((1ULL << 6) - 1);
        uint8_t move_flag = (move >> 12);
        if (move_flag >= 10 && move_flag != promote_to) continue;
        if (pos1 == choosed_square_) {
            coord = get_coordinates(pos2);
            board_square.setPosition(80.f + 80.f * (float)coord.first, 80.f + 80.f * (float)coord.second);
            window_.draw(board_square);
        }
    }
}

void App::draw_figures_() {
    for (Square pos = 0; pos < 64; ++pos) {
        Figure figure = board_.get_figure(pos);
        if (figure != FIGURE_NULL) {
            board_sprites_[pos].setTexture(figure_texture);
            uint8_t figure_type_index = (figure >> 1) - 1;
            uint8_t figure_color_index = (figure & 1);
            board_sprites_[pos].setTextureRect(
                    sf::IntRect(figure_type_index * 80.f, figure_color_index * 80.f, 80, 80));
            board_sprites_[pos].setColor(sf::Color(255, 255, 255, 255));
            std::pair<uint8_t, uint8_t> coord = get_coordinates(pos);
            board_sprites_[pos].setPosition(80.f + 80.f * (float) coord.first, 80.f + 80.f * (float) coord.second);
            window_.draw(board_sprites_[pos]);
        } else {
            board_sprites_[pos].setTexture(figure_texture);
            uint8_t figure_type_index = (PAWN >> 1) - 1;
            uint8_t figure_color_index = (PAWN & 1);
            board_sprites_[pos].setTextureRect(
                    sf::IntRect(figure_type_index * 80.f, figure_color_index * 80.f, 80, 80));
            board_sprites_[pos].setColor(sf::Color(255, 255, 255, 0));
            std::pair<uint8_t, uint8_t> coord = get_coordinates(pos);
            board_sprites_[pos].setPosition(80.f + 80.f * (float) coord.first, 80.f + 80.f * (float) coord.second);
            window_.draw(board_sprites_[pos]);
        }
    }
}
