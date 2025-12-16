#ifndef STATE_H_
#define STATE_H_

#include "board.h"


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
void reset_input(InputState *input);
void update_game(Board *board, InputState *input);

void switch_cells(Cell *c1, Cell *c2);


#endif