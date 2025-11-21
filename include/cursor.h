#ifndef M5_PROJECT_CURSOR_H_
#define M5_PROJECT_CURSOR_H_
#include <cstdint>


typedef struct {
    uint8_t x, y, dx, dy;
    uint8_t w, h;
    uint32_t color;
} Cursor;

Cursor* init_cursor();

#endif