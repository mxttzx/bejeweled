#ifndef BOARD_H_
#define BOARD_H_

#include "M5Unified.h"

#define CELL_WIDTH 15
#define CELL_HEIGHT 15

#define BOARD_OFFSET 50

#define CURSOR_SIZE 2

#define INIT_ROWS 6
#define INIT_COLS 6
#define INIT_COLORS 5

typedef struct {
    uint16_t color;
    uint8_t x, y; // Should be a value in [0, cols] and [0, rows]
    uint8_t w, h;
    bool init;
} Cell;

typedef struct {
    Cell *grid;
    uint8_t rows;
    uint8_t cols;
    uint8_t colors;
} Board;


uint8_t get_idx(Board *board, uint8_t x, uint8_t y);
uint8_t get_dims(uint8_t amt);

Board* init_board(uint8_t rows, uint8_t cols, uint8_t colors);
Board* new_board(Board *board, uint8_t rows, uint8_t cols, uint8_t colors);
void free_board(Board *board);
void reset_board(Board *board);
void resupply(Board *board);

#endif
