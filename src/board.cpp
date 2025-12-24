#include "../include/board.h"

uint16_t get_cell_color(uint8_t amt) {
    const uint16_t colors[] = {
        RED,
        GREEN,
        BLUE,
        MAGENTA,
        CYAN,
        YELLOW,
        PURPLE
    };
    return colors[rand() % amt];
}

uint8_t get_dims(uint8_t amt) {
    uint8_t dims[] = {6, 7, 8, 9};
    return dims[rand() % amt];
}

uint8_t get_idx(Board *board, uint8_t x, uint8_t y) {
    return y * board->cols + x;
}

void reset_board(Board *board) {
    for (uint8_t i = 0; i < board->rows; i++) {
        for (uint8_t j = 0; j < board->cols; j++) {
            // j -> columns (x)
            // i -> rows (y)
            Cell *cell = &board->grid[get_idx(board, j, i)]; 
            uint16_t color;

            do {
                color = get_cell_color(board->colors);

                if (j >= 2) {
                    // check if two preceding x coordinates have the same color
                    // If they do then we pick another color 
                    Cell *c1 = &board->grid[get_idx(board, j - 1, i)];
                    Cell *c2 = &board->grid[get_idx(board, j - 2, i)];
                    if (c1->color == color && c2->color == color)
                        continue;
                }

                if (i >= 2) {
                    // Same concept for y coordinates
                    // Pick another if two preciding y coordinates have the same color 
                    Cell *c1 = &board->grid[get_idx(board, j, i - 1)];
                    Cell *c2 = &board->grid[get_idx(board, j, i - 2)];
                    if (c1->color == color && c2->color == color)
                        continue;
                }

                break;

            } while (true);

            cell->init = true;
            cell->color = color;
            cell->x = j;
            cell->y = i;
            cell->w = CELL_WIDTH;
            cell->h = CELL_HEIGHT;
        }
    }
}

Board *new_board(Board *board, uint8_t rows, uint8_t cols, uint8_t colors) {
    free_board(board);
    return init_board(rows, cols, colors);
}

Board* init_board(uint8_t rows, uint8_t cols, uint8_t colors) {
    Board *board = (Board*)malloc(sizeof(Board));
    board->rows = rows;
    board->cols = cols;
    board->colors = colors;
    board->grid = (Cell *)calloc(board->rows * board->cols, sizeof(Cell));

    reset_board(board);

    return board;
}

void resupply(Board *board) {
    for (uint8_t y = 0; y < board->rows; y++) {
        for (uint8_t x = 0; x < board->cols; x++) {
            Cell *c = &board->grid[get_idx(board, x, y)];
            if (!c->init) {
                c->init = true;
                c->color = get_cell_color(board->colors);
            }
        }
    }
}

void free_board(Board *board) {
    free(board->grid);
    free(board);
}

