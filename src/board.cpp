#include "../include/board.h"

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

uint8_t get_idx(Board *board, uint8_t x, uint8_t y) {
    return y * board->cols + x;
}

Board* init_board(uint8_t rows, uint8_t cols) {
    Board *board = (Board*)malloc(sizeof(Board));
    if (!board) {
        fprintf(stderr, "init_board: failed to allocate memory for board\n");
        exit(EXIT_FAILURE);
    }

    board->rows = rows;
    board->cols = cols;

    board->grid = (Cell *)calloc(board->rows * board->cols, sizeof(Cell));
    if (!board->grid) {
        fprintf(stderr, "init_board: failed to allocate memory for board\n");
        exit(EXIT_FAILURE);
    }

    for (uint8_t i = 0; i < board->rows; i++) {
        for (uint8_t j = 0; j < board->cols; j++) {
            Cell *cell = &board->grid[get_idx(board, j, i)];
            uint32_t color;

            do {
                color = get_cell_color();

                if (j >= 2) {
                    Cell *c1 = &board->grid[get_idx(board, i, j - 1)];
                    Cell *c2 = &board->grid[get_idx(board, i, j - 2)];
                    if (c1->color == color && c2->color == color)
                        continue;
                }

                if (i >= 2) {
                    Cell *c1 = &board->grid[get_idx(board, i - 1, j)];
                    Cell *c2 = &board->grid[get_idx(board, i - 2, j)];
                    if (c1->color == color && c2->color == color)
                        continue;
                }

                break;

            } while (true);

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

void resupply(Board *board) {
    for (uint8_t y = 0; y < board->rows; y++) {
        for (uint8_t x = 0; x < board->cols; x++) {
            Cell *c = &board->grid[get_idx(board, x, y)];
            if (!c->init) {
                c->init = true;
                c->color = get_cell_color();
            }
        }
    }
}

void free_board(Board *board) {
    free(board->grid);
    free(board);
}

