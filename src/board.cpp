#include "../include/board.h"

#include <EEPROM.h>
#include "M5Unified.h"

uint32_t get_cell_color() {
    const uint32_t colors[] = {
        0xFF0000,
        0x0000FF,
        0x00FF00,
        0xFF00FF,
        0xFFFF00
    };
    return colors[rand() % 5];
}

uint8_t g_idx(Board *board, uint8_t x, uint8_t y) {
    return y * board->cols + x;
}

Board* init_board() {
    Board *board = (Board*)malloc(sizeof(Board));
    if (!board) {
        fprintf(stderr, "Failed to allocate memory for board\n");
        exit(EXIT_FAILURE);
    }

    board->rows = BOARD_ROWS;
    board->cols = BOARD_COLS;

    board->grid = (Cell *)calloc(board->rows * board->cols, sizeof(Cell));
    if (!board->grid) {
        fprintf(stderr, "Failed to allocate memory for board\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < board->rows; i++) {
        for (int j = 0; j < board->cols; j++) {
            Cell *cell = &board->grid[i * board->cols + j];

            cell->init = true;
            cell->color = get_cell_color();
            cell->x = j;
            cell->y = i;
            cell->w = CELL_WIDTH;
            cell->h = CELL_HEIGHT;
        }
    }

    return board;
}

void free_board(Board *board) {
    free(board->grid);
    free(board);
}

