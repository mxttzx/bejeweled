#include "../include/game.h"

#include "state.h"


uint32_t get_score() {
    return EEPROM.read(ADDR_SCORE);
}

uint8_t get_level() {
    return EEPROM.read(ADDR_LEVEL);
}

void set_score(uint32_t score) {
    EEPROM.write(ADDR_SCORE, score);
}

void set_level(uint8_t level) {
    EEPROM.write(ADDR_LEVEL, level);
}

void reset_game(GameState *gs) {
    if (!gs) return;
    gs->score = 0;
    gs->level = 0;
    gs->moves = INIT_MOVES;
    gs->points = INIT_POINTS;
    gs->game_over = false;
    gs->view_mode = STARTUP;
    gs->game_mode = EASY;
    gs->actv_item = 0; // The index of the menu_items array, -1 for when in GAME mode
}

GameState* init_game() {
    GameState *gs = (GameState*)malloc(sizeof(GameState));
    reset_game(gs); 
 
    return gs;
}

bool update_score(GameState *gs, uint8_t blocks) {
    uint8_t level = gs->level;
    uint8_t moves = gs->moves;
    uint32_t score = gs->score;
    uint32_t points = gs->points;

    bool new_lvl = false;

    score += blocks * POINT_PER_CELL;
    if (score >= points) {
        // Set up new level
        points = gs->points * 2;
        moves = gs->moves + 10;

        level++;

        new_lvl = true;
    }

    gs->score = score;
    gs->level = level;
    gs->moves = moves;
    gs->points = points;

    return new_lvl;
}
