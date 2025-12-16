#ifndef GAME_H_
#define GAME_H_

#include "board.h"
#include <M5Unified.h>

#include "state.h"

typedef void (*Draw)(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t);

void render_cell(Cell *cell, Draw *func);
void render_game(Board *board, InputState *input);

#endif
