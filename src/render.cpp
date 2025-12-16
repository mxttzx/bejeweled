#include "../include/render.h"

#include "state.h"


void outline(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t color) {
    M5.Lcd.drawRect(x * CELL_WIDTH, y * CELL_HEIGHT, w, h, color);
}

void fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t color) {
    M5.Lcd.fillRect(x * CELL_WIDTH, y * CELL_HEIGHT, w, h, color);
}

void render_cell(Cell *cell, Draw draw) {
    draw(cell->x, cell->y, cell->w, cell->h, cell->color);
}

void render_game(Board *board, InputState *input) {
    Cell *cursor = input->cursor;

    for (int i = 0; i < board->cols * board->rows; i++) {
        Cell *cell = &board->grid[i];
        render_cell(cell, fill);
    }

    render_cell(&cursor[0], outline);
    render_cell(&cursor[1], outline);
}