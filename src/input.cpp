#include "input.h"


void reset_input_internal(InputState *input) {
    input->btn_a = false;
    input->btn_b = false;
    input->btn_c = false;

    input->ax = 0;
    input->ay = 0;
    input->az = 0;
}

void reset_input(InputState *input) {
    if (!input || !input->cursor) return;

    for (int i = 0; i < CURSOR_SIZE; i++) {
        Cell *cell = &input->cursor[i];

        cell->w = CELL_WIDTH;
        cell->h = CELL_HEIGHT;
        cell->color = WHITE;
        cell->x = 0;
        cell->y = 0;
    }

    // Place second cursor block next to first one
    // They cannot be both at origin
    input->cursor[0].x = 0;
    input->cursor[1].x = 1;

    reset_input_internal(input);
}

InputState* init_input() {
    InputState *input = (InputState *)malloc(sizeof(InputState));
    if (input == NULL) exit(EXIT_FAILURE);

    input->cursor = (Cell *)calloc(CURSOR_SIZE, sizeof(Cell));
    if (input->cursor == NULL) exit(EXIT_FAILURE);

    reset_input(input);

    return input;
}

void move_cursor(Board *board, InputState *input) {
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
    // Unsigned int cannot be negative, no bounds check
    if (nx1 >= board->cols ||
        ny1 >= board->rows ||
        nx2 >= board->cols ||
        ny2 >= board->rows) return;

    cursor[0].x += dx;
    cursor[0].y += dy;
    cursor[1].x += dx;
    cursor[1].y += dy;
}

void rotate_cursor(Board *board, InputState *input) {
    Cell *cursor = input->cursor;

    // Rotate based on goniometric circle path
    uint8_t x1 = cursor[0].x;
    uint8_t y1 = cursor[0].y;

    uint8_t dx = cursor[1].x - cursor[0].x;
    uint8_t dy = cursor[1].y - cursor[0].y;

    // Get values of rotation
    float c = cosf(M_PI / 2);
    float s = sinf(M_PI / 2);

    // Values will be between -1 and 1
    float rx = (float)dx * c - (float)dy * s;
    float ry = (float)dx * s + (float)dy * c;

    // New cell values
    uint8_t nx = (uint8_t)roundf((float)x1 + rx);
    uint8_t ny = (uint8_t)roundf((float)y1 + ry);

    // Unsigned int cannot be negative, no bounds check
    if (nx >= board->cols ||
        ny >= board->rows) return;

    cursor[1].x = nx;
    cursor[1].y = ny;
}
