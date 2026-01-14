#ifndef CURSOR_H_
#define CURSOR_H_

#include "game.h"

#define MIN_TILT 0.30

typedef struct {
    int quit;
    bool btn_a;
    bool btn_b;
    bool btn_c;
    float ax, ay, az;
    Cell *cursor;
} InputState;

InputState* init_input();

void read_input(InputState *input);
void reset_input(InputState *input);
void reset_input_internal(InputState *input);

void update_game(GameState *gs, Board *board, InputState *input);
void update_menu(GameState *gs, InputState *input);

void move_cursor(Board *board, InputState *input);
void rotate_cursor(Board *board, InputState *input);

#endif