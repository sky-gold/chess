#ifndef CHESS_APP_H
#define CHESS_APP_H

#include <SFML/Graphics.hpp>
#include <cassert>
#include <thread>
#include <future>
#include <chrono>

#include "board.h"
#include "search.h"

class App {
public:
    App();

    void run();

private:

    std::pair<uint8_t, uint8_t> get_coordinates(Square sq) const;

    void draw_();

    void update_();

    void draw_board_();

    void draw_figures_();

    void draw_last_move_();

    void draw_possible_moves_();

    void draw_time_selector_();

    void draw_promote_selector_();

    void draw_turn_selector_();

    void draw_game_result_();

    void draw_undo_();

    void draw_reset_();

    sf::RenderWindow window_;

    bool board_flip = false;

    uint8_t promote_to = 10; //QUEEN
    bool mouse_click_flag_ = true;

    Square choosed_square_ = SQUARE_NULL;

    Board board_;
    std::array<sf::Sprite, 64> board_sprites_;

    sf::Texture figure_texture;
    sf::Font font_;

    sf::Sprite promote_sprite_;

    sf::Sprite white_turn_sprite_;
    sf::Sprite black_turn_sprite_;
    uint8_t human_turn_ = WHITE;

    sf::RectangleShape time_selector_;
    float choosed_time_ = 0.1334;

    search::Search search_container;
    clock_t search_start_ = 0;

    sf::RectangleShape undo_button_;
    sf::RectangleShape reset_button_;

    bool undo_move_ = false;
    bool reset_board_ = false;
};

//#include "app.cpp"

#endif //CHESS_APP_H
