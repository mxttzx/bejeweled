#include "../include/state.h"

#include "cursor.h"
#include "M5Unified.h"
#include "util.h"

InputState* init_input() {
    InputState *input = (InputState *)malloc(sizeof(InputState));
    if (!input) {
        fprintf(stderr, "Failed to allocate memory for input\n");
        exit(EXIT_FAILURE);
    }

    input->cursor = (Cell *)calloc(CURSOR_SIZE, sizeof(Cell));
    if (!input->cursor) {
        fprintf(stderr, "Failed to allocate memory for cursor\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < CURSOR_SIZE; i++) {
        Cell *cell = &input->cursor[i];

        cell->w = CELL_WIDTH;
        cell->h = CELL_HEIGHT;
        cell->color = 0;
    }

    // c1: [0, 0], c2: [1, 0]
    input->cursor[1].x = 1;

    return input;
}


void swap(Cell *c1, Cell *c2) {
    if (c1->color == c2->color) return; // Switching cells of the same color
    if (!c1->init || !c2->init) return; // Switching dead cells

    uint32_t temp = c1->color;
    c1->color = c2->color;
    c2->color = temp;
}

void reset_input(InputState *input) {
    input->btn_a = false;
    input->btn_b = false;

    input->ax = 0;
    input->ay = 0;
    input->az = 0;
}

void disable_cell(Cell *cell) {
    cell->init = false;
    cell->color = 0;
}

bool row_inline(Board *board, uint8_t row) {
    uint32_t prev = 0;
    uint8_t start = 0;
    bool rm = false;

    for (uint8_t x = 0; x <= BOARD_COLS; x++) {
        uint32_t next = 0;

        if (x < BOARD_COLS) {
            Cell *c = &board->grid[get_idx(board, x, row)];
            next = c->init ? c->color : 0;
        }

        if (x == 0) {
            prev = next;
            continue;
        }

        if (next != prev) {
            uint8_t run = x - start;

            if (run >= 3 && prev != 0) {
                for (uint8_t k = start; k < x; k++) {
                    disable_cell(&board->grid[get_idx(board, k, row)]);
                }
                rm = true;
            }

            prev = next;
            start = x;
        }
    }
    return rm;
}

bool col_inline(Board *board, uint8_t col) {
    uint32_t prev = 0;
    uint8_t start = 0;
    bool rm = false;

    for (uint8_t y = 0; y <= BOARD_ROWS; y++) {
        uint32_t next = 0;

        if (y < BOARD_ROWS) {
            Cell *c = &board->grid[get_idx(board, col, y)];
            next = c->init ? c->color : 0;
        }

        if (y == 0) {
            prev = next;
            continue;
        }

        if (next != prev) {
            uint8_t run = y - start;

            if (run >= 3 && prev != 0) {
                for (uint8_t k = start; k < y; k++) {
                    disable_cell(&board->grid[get_idx(board, col, k)]);
                }
                rm = true;
            }

            prev = next;
            start = y;
        }
    }
    return rm;
}

void cascade(Board *board) {
    for (uint8_t x = 0; x < BOARD_COLS; x++) {
        int row = BOARD_ROWS - 1;

        for (int y = BOARD_ROWS - 1; y >= 0; y--) {
            Cell *c1 = &board->grid[get_idx(board, x, y)];
            if (!c1->init) continue;
            if (y != row) {
                Cell *c2 = &board->grid[get_idx(board, x, row)];
                c2->color = c1->color;
                c2->init = true;
                disable_cell(c1);
            }
            row--;
        }

        for (int y = row; y >= 0; y--) {
            Cell *c = &board->grid[get_idx(board, x, y)];
            disable_cell(c);
        }
    }
}

bool mark_inline(Board *board) {
    bool rm = false;

    for (uint8_t x = 0; x < BOARD_COLS; x++) {
        if (col_inline(board, x)) rm = true;
    }

    for (uint8_t y = 0; y < BOARD_ROWS; y++) {
        if (row_inline(board, y)) rm = true;
    }

    return rm;
}

void stabilize(Board *board) {
    while (mark_inline(board)) {
        cascade(board);
        resupply(board);
    }
}

void update_game(Board *board, InputState *input) {
    if (input->btn_a) rotate_cursor(input);
    if (input->btn_b) {
        Cell *c1 = &board->grid[get_idx(board, input->cursor[0].x, input->cursor[0].y)];
        Cell *c2 = &board->grid[get_idx(board, input->cursor[1].x, input->cursor[1].y)];

        swap(c1, c2);
        stabilize(board);
    }

    move_cursor(input);
    reset_input(input);
}

void read_input(InputState *input) {
    if (M5.BtnA.wasPressed()) input->btn_a = true;
    if (M5.BtnB.wasPressed()) input->btn_b = true;

    M5.Imu.getAccelData(&input->ax, &input->ay, &input->az);
}
