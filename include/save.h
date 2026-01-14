#ifndef SAVE_H_
#define SAVE_H_

#include "board.h"
#include "game.h"

void save_game(GameState *gs, Board *board);
void load_game(GameState *gs, Board *board);

#endif