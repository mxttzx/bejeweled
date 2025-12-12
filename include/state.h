#ifndef BEJEWELED_STATE_H_
#define BEJEWELED_STATE_H_

#include "board.h"

#define MIN_TILT 0.25

#define ROTATE (M_PI / 2)


typedef struct {
    int should_continue;
    int game_over;
    int score;
} GameState;

typedef struct {
    int quit;
    bool btn_a;
    bool btn_b;
    float ax, ay, az;
    Cell *cursor;
} InputState;

InputState* init_input();
void read_input(InputState *input);
void update_game(Board *board, InputState *input);

#endif