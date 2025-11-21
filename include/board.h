#ifndef M5_PROJECT_BOARD_H_
#define M5_PROJECT_BOARD_H_
#include <cstdint>

#define CELL_WIDTH 10
#define CELL_HEIGHT 10

#define BOARD_ROWS 10
#define BOARD_COLS 10

typedef struct {
    uint32_t color;
    uint8_t x, y;
    uint8_t w, h;
} Cell;

typedef struct {
    Cell *grid;
    uint8_t rows;
    uint8_t cols;
} Board;

Board* init_board();

#endif