#include "../include/cursor.h"


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

void rotate_cursor(InputState *input) {
    Cell *cursor = input->cursor;

    // Rotation based on goniometric circle path
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

    if (nx >= BOARD_COLS || ny >= BOARD_ROWS) return;

    cursor[1].x = nx;
    cursor[1].y = ny;
}
