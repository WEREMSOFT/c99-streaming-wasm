#ifndef __SELECTOR_H__
#define __SELECTOR_H__

#include <raylib.h>
#include <string.h>
#include <stdbool.h>
#include "../mister_queen/uci.h"
#include "types.h"
#define DEBUG_PRINT
#include "console_output.h"
#include "game_state.h"
#include "game_board.h"
#include "event.h"

enum enum_selector_state {
    SELECTOR_STATE_DISABLED,
    SELECTOR_STATE_READY,
    SELECTOR_STATE_AWAITING_TARGET,
    SELECTOR_STATE_ILLEGAL,
};

void selector_get_coordinates_as_string(selector_t selector, char *result){
    snprintf(result, 5, "%c%c%c%c", 
        board_coord_x[(int)selector.position_start.x], 
        board_coord_y[(int)selector.position_start.z], 
        board_coord_x[(int)selector.position.x], 
        board_coord_y[(int)selector.position.z]);
}

void selector_process_keys(selector_t *selector)
{
    if (IsKeyPressed(KEY_LEFT))
    {
        selector->position.x -= 1.0f;
    }

    if (IsKeyPressed(KEY_RIGHT))
    {
        selector->position.x += 1.0f;
    }

    if (IsKeyPressed(KEY_UP))
    {
        selector->position.z -= 1.0f;
    }

    if (IsKeyPressed(KEY_DOWN))
    {
        selector->position.z += 1.0f;
    }
}

void selector_pass_to_state_illegal(selector_t* selector){
        selector->state = SELECTOR_STATE_ILLEGAL;
        selector->position_start = selector->position;
}

void selector_process_state_ready(game_state_t* game_state) {
    selector_process_keys(&game_state->selector);

    if(IsKeyPressed(KEY_SPACE))
    {
        if (game_board_is_origin_position_legal(game_state->board, game_state->selector)) {
            game_state->selector.state = SELECTOR_STATE_AWAITING_TARGET;
            game_state->selector.position_start = game_state->selector.position;
            game_state->selector.origin_piece = game_state->board[(int)game_state->selector.position.z][(int)game_state->selector.position.x];
        } else {
            selector_pass_to_state_illegal(&game_state->selector);
        }
    }
}

void selector_send_move_to_engine(game_state_t* game_state)
{
    char coordinates[5] = {0};
    selector_get_coordinates_as_string(game_state->selector, coordinates);

    event_t event = {0};
    event.type = EVENT_COMMAND;

    sprintf(event.data, "%s", coordinates);
    event_queue(event);
}

void selector_process_state_awaiting_target(game_state_t* game_state)
{
    selector_process_keys(&game_state->selector);

    if(IsKeyPressed(KEY_SPACE))
    {
        if(game_board_is_target_position_legal(game_state->board, game_state->selector)){
            game_state->selector.state = SELECTOR_STATE_DISABLED;
            selector_send_move_to_engine(game_state);
        } else {
            selector_pass_to_state_illegal(&game_state->selector);
        }
    }
}

void selector_pass_to_state_ready(selector_t* selector){
    selector->state = SELECTOR_STATE_READY;
    selector->visible = true;
    selector->color = RED;
}


void selector_process_state_illegal(selector_t* selector){
    static float seconds = 0.0f;
    static int counter = 0;
    
    seconds += GetFrameTime();

    if(seconds > 0.1f){
        seconds = 0;
        selector->visible = !selector->visible;
        counter++;
    }

    if(counter > 10){
        counter = 0;
        selector_pass_to_state_ready(selector);
    }

}

void selector_update(game_state_t* game_state)
{
    switch(game_state->selector.state){
        case SELECTOR_STATE_READY:
            selector_process_state_ready(game_state);
            break;
        case SELECTOR_STATE_AWAITING_TARGET:
            selector_process_state_awaiting_target(game_state);
            break;
        case SELECTOR_STATE_DISABLED:
            break;
        case SELECTOR_STATE_ILLEGAL:
            selector_process_state_illegal(&game_state->selector);
            break;
    }
}

void selector_draw(selector_t selector)
{
    switch(selector.state){
        case SELECTOR_STATE_READY:
            DrawCubeWires(selector.position, 1, 1, 1, selector.color);
            break;
        case SELECTOR_STATE_AWAITING_TARGET:
            DrawCubeWires(selector.position, 1, 1, 1, RED);
            DrawCubeWires(selector.position_start, 1, 1, 1, PURPLE);
            break;
        case SELECTOR_STATE_ILLEGAL:
            if(selector.visible){
                DrawCube(selector.position, 1, 1, 1, RED);
            }
            break;
    }

}
#endif