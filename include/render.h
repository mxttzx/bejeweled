#ifndef RENDER_H_
#define RENDER_H_

#include "board.h"
#include "state.h"
#include "game.h"
#include <M5Unified.h>

#define SCREEN_WIDTH 135
#define SCREEN_HEIGHT 240
#define GRID_OFFSET 50

#define MENU_ITM_SIZE 4
#define MENU_ITM_HEIGHT 25

#define MARGIN_X 25
#define MARGIN_Y 25
#define OFFSET_Y 10


typedef void (*Draw)(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t);

void render_cell(Cell *cell, Draw *func);
void render_game(GameState *gs, Board *board, InputState *input);
void render_board(GameState *gs, Board *board);
void render_menu(GameState, InputState *input);
void render_state(GameState *gs);

#endif
