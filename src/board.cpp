#include "../include/board.h"

#include <EEPROM.h>
#include "M5Unified.h"
#include "util.h"


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

void resupply(Board *board) {
    for (uint8_t y = 0; y < BOARD_ROWS; y++) {
        for (uint8_t x = 0; x < BOARD_COLS; x++) {
            Cell *c = &board->grid[get_idx(board, x, y)];
            if (!c->init) {
                c->init = true;
                c->color = get_cell_color();
            }
        }
    }
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

    for (uint8_t i = 0; i < BOARD_ROWS; i++) {
        for (uint8_t j = 0; j < BOARD_COLS; j++) {
            Cell *cell = &board->grid[get_idx(board, i, j)];
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

