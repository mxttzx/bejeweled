#include "state.h"
#include "board.h"
#include "M5Unified.h"

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
        cell->color = 0xFFFFFF;
    }

    // c1: [0, 0], c2: [1, 0]
    input->cursor[1].x = 1;

    return input;
}

void move_cursor(InputState *input) {
    Cell *cursor = input->cursor;

    float ax = input->ax;
    float ay = input->ay;

    int8_t dx = 0;
    int8_t dy = 0;

    if (ax > MIN_TILT) dx = -1;
    else if (ax < -MIN_TILT) dx = 1;

    if (ay > MIN_TILT) dy = 1;
    else if (ay < -MIN_TILT) dy = -1;

    if (dx == 0 && dy == 0) return; // No movement needed if no deviation from current position

    uint8_t nx1 = cursor[0].x + dx;
    uint8_t ny1 = cursor[0].y + dy;

    uint8_t nx2 = cursor[1].x + dx;
    uint8_t ny2 = cursor[1].y + dy;

    // Abort if cells move out of bounds
    if ( nx1 >= BOARD_COLS ||
         ny1 >= BOARD_ROWS ||
         nx2 >= BOARD_COLS ||
         ny2 >= BOARD_ROWS) return;

    cursor[0].x += dx;
    cursor[0].y += dy;
    cursor[1].x += dx;
    cursor[1].y += dy;
}

void check_inline_col(Board *board, uint8_t col) {
    uint32_t prev = board->grid[g_idx(board, col, 0)].color;
    uint8_t start = 0;

    for (uint8_t j = 1; j <= board->rows; j++) {
        uint32_t next = 0x000000;

        if (j < board->rows) {
            next = board->grid[g_idx(board, col, j)].color;
        }

        if (next != prev) {
            uint8_t length = j - start;

            if (length >= 3 && prev != 0x000000) {
                for (uint8_t y = start; y < j; y++) {
                    uint8_t idx = g_idx(board, col, y);
                    board->grid[idx].color = 0x000000;
                    board->grid[idx].init = false;
                }
            }
            prev = next;
            start = j;
        }
    }
}

void check_inline_row(Board *board, uint8_t row) {
    uint32_t prev = board->grid[g_idx(board, 0, row)].color;
    uint8_t start = 0;

    for (uint8_t i = 1; i <= board->cols; i++) {
        uint32_t next = 0x000000;

        if (i < board->cols) {
            next = board->grid[g_idx(board, i, row)].color;
        }

        if (next != prev) {
            uint8_t length = i - start;

            if (length >= 3 && prev != 0x000000) {
                for (uint8_t x = start; x < i; x++) {
                    uint8_t idx = g_idx(board, x, row);
                    board->grid[idx].color = 0x000000;
                    board->grid[idx].init = false;
                }
            }
            prev = next;
            start = i;
        }
    }
}

void rotate_cursor(Cell *cursor, float direction) {
    // Rotation based on goniometric circle path
    uint8_t x1 = cursor[0].x;
    uint8_t y1 = cursor[0].y;

    uint8_t dx = cursor[1].x - cursor[0].x;
    uint8_t dy = cursor[1].y - cursor[0].y;

    // Get values of rotation
    float c = cosf(direction);
    float s = sinf(direction);

    // Values will be between -1 and 1
    float rx = (float)dx * c - (float)dy * s;
    float ry = (float)dx * s + (float)dy * c;

    // New cell values
    uint8_t nx = (uint8_t)roundf((float)x1 + rx);
    uint8_t ny = (uint8_t)roundf((float)y1 + ry);

    if (nx >= BOARD_COLS || ny >= BOARD_ROWS) return;

    cursor[1].x = nx;
    cursor[1].y = ny;
}

void switch_block(Board *board, Cell *cursor) {
    uint8_t x1 = cursor[0].x;
    uint8_t y1 = cursor[0].y;

    uint8_t x2 = cursor[1].x;
    uint8_t y2 = cursor[1].y;

    uint8_t idx1 = g_idx(board, x1, y1);
    uint8_t idx2 = g_idx(board, x2, y2);

    if (board->grid[idx1].init == false ||
        board->grid[idx2].init == false) return; // Can't switch to be removed cell
    if (board->grid[idx1].color == board->grid[idx2].color) return; // Switching two of the same colors

    uint32_t temp = board->grid[idx1].color;
    board->grid[idx1].color = board->grid[idx2].color;
    board->grid[idx2].color = temp;
}

void reset_input(InputState *input) {
    input->btn_a = false;
    input->btn_b = false;

    input->ax = 0;
    input->ay = 0;
    input->az = 0;
}

void update_game(Board *board, InputState *input) {
    if (input->btn_a) rotate_cursor(input->cursor, ROTATE);
    if (input->btn_b) {
        switch_block(board, input->cursor);

        check_inline_row(board, input->cursor[0].y);
        check_inline_row(board, input->cursor[1].y);

        check_inline_col(board, input->cursor[1].x);
        check_inline_col(board, input->cursor[0].x);
    }

    move_cursor(input);
    reset_input(input);
}

void read_input(InputState *input) {
    if (M5.BtnA.wasPressed()) input->btn_a = true;
    if (M5.BtnB.wasPressed()) input->btn_b = true;

    M5.Imu.getAccelData(&input->ax, &input->ay, &input->az);
}
