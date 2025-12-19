#ifndef GAME_H_
#define GAME_H_

#include "board.h"
#include "EEPROM.h"

#define POINT_PER_CELL 5
#define MULTIPLIER 1.1

#define INIT_POINTS 100
#define INIT_MOVES 16

#define ADDR_SCORE 1
#define ADDR_LEVEL 2

typedef enum {
   EASY, HARD
} GameMode;

typedef enum {
    MENU, GAME
} ViewMode;

typedef struct {
    bool should_continue;
    bool game_over;
    ViewMode view_mode;
    GameMode game_mode;
    uint8_t actv_item;
    uint32_t points;
    uint32_t score;
    uint8_t moves;
    uint8_t level;
} GameState;

GameState* init_game();

uint32_t get_score();
uint8_t get_level();

void set_score(uint32_t score);
void set_level(uint8_t level);

void update_score(GameState *gs, uint8_t blocks);

#endif