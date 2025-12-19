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


GameState* init_game() {
    GameState *gs = (GameState*)malloc(sizeof(GameState));

    gs->score = get_score();
    gs->level = get_level();
    gs->moves = INIT_MOVES;
    gs->points = INIT_POINTS;
    gs->view_mode = GAME;
    gs->actv_item = -1; // The index of the menu_items array, -1 for when in GAME mode

    return gs;
}

void update_score(GameState *gs, uint8_t blocks) {
    uint8_t level = gs->level;
    uint8_t moves = gs->moves;
    uint32_t score = gs->score;
    uint32_t points = gs->points;

    score += blocks * POINT_PER_CELL;
    if (score >= points) {
        // Set up new level
        score -= points;

        points = gs->points * MULTIPLIER;
        moves =  gs->moves * MULTIPLIER;

        level++;
    }

    gs->score = score;
    gs->level = level;
    gs->moves = moves;
    gs->points = points;
}
