#ifndef M5_PROJECT_BOARD_H_
#define M5_PROJECT_BOARD_H_

#include <cstdint>

#define CELL_WIDTH 15
#define CELL_HEIGHT 15

#define BOARD_ROWS 9
#define BOARD_COLS 9

#define CURSOR_SIZE 2

typedef struct {
    uint32_t color;
    uint8_t x, y; // Should be a value in [0, cols] and [0, rows]
    uint8_t w, h;
    bool init;
} Cell;

typedef struct {
    Cell *grid;
    uint8_t rows;
    uint8_t cols;
} Board;

uint8_t g_idx(Board *board, uint8_t x, uint8_t y);
Board* init_board();

#endif
