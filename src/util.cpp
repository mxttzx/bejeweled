#include "util.h"
#include "board.h"

// Helper function for accessing elements inside flattened array via index
uint8_t get_idx(Board *board, uint8_t x, uint8_t y) {
    return y * board->cols + x;
}

Cell *get_column(Board *board, uint8_t n) {
    uint16_t idx = get_idx(board, n, 0);
    return &board->grid[idx];
}

Cell *get_row(Board *board, uint8_t n) {
    uint16_t idx = get_idx(board, 0, n);
    return &board->grid[idx];
}
