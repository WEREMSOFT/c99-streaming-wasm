#ifndef __GAME_OPTIONS_H__
#define __GAME_OPTIONS_H__

typedef struct game_options_t {
    bool draw_attacked_positions;
    bool allow_move_if_king_is_pinned;
    bool allow_move_king_if_target_is_under_attack;
    bool is_full_creen;
    bool should_close_window;
    bool show_coordinates_on_minimap;
} game_options_t;

#endif