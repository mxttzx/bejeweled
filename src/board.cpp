#include "../include/board.h"

#include <EEPROM.h>

#include "cursor.h"
#include "M5Unified.h"

uint32_t get_cell_color() {
    const uint32_t colors[] = {
        0x0000FF,
        0xFF0000,
        0x00FF00,
        0xFFFF00,
        0xFF00FF,
        0x00FFFF,
        0xFFFFFF
    };
    return colors[rand() % sizeof(colors)];
}

Cursor* init_cursor() {
    Cursor *cursor = (Cursor *)calloc(1, sizeof(Cursor));
    cursor->dy = cursor->y + 1; // Start at position (0, 0) and (0, 1)
    cursor->color = 0xFFFFFF;
    return cursor;
}

Board* init_board() {
    Board *board = (Board *)malloc(sizeof(Board));

    board->rows = BOARD_ROWS;
    board->cols = BOARD_COLS;

    board->grid = (Cell *)calloc(board->rows * board->cols, sizeof(Cell));

    for (int i = 0; i < board->rows * board->cols; i++) {
        const uint8_t row = i / board->cols;
        const uint8_t col = i % board->cols;

        Cell *cell = &board->grid[i];
        cell->color = get_cell_color();
        cell->x = col * CELL_WIDTH;
        cell->y = row * CELL_WIDTH;
        cell->w = CELL_WIDTH;
        cell->h = CELL_HEIGHT;
    }

    return board;
}

void free_board(Board *board) {
    free(board->grid);
    free(board);
}

