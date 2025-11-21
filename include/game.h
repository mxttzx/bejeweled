
#ifndef M5_PROJECT_GAME_H
#define M5_PROJECT_GAME_H

#include "board.h"
#include "cursor.h"

void read_input();
void update_game();

Board *new_game();

void render_cell(Cell *cell);
void render_cursor(Cursor *cursor);
void render_game(Board *board, Cursor *cursor);
typedef struct {} GameState;

#endif //M5_PROJECT_GAME_H