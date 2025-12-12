
#ifndef M5_PROJECT_GAME_H
#define M5_PROJECT_GAME_H

#include "board.h"

void read_input();
void update_game();

Board *new_game();

typedef void (*Draw)(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t);

void render_cell(Cell *cell, Draw *func);
void render_game(Board *board, Cell *cursor);

void rotate_cursor(Cell *cursor, float direction);

#endif
