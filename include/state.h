#ifndef STATE_H_
#define STATE_H_

#include "board.h"
#include "game.h"
#include "M5Unified.h"


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
void update_game(GameState *gs, Board *board, InputState *input);
void update_menu(GameState *gs, InputState *input);

#endif