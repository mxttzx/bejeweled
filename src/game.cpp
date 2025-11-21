#include "game.h"

#include "M5Unified.h"

Board *new_game() {
    Cursor *cursor = init_cursor();
    Board *board = init_board();

    return board;
}

void update_game() {

}

void render_cell(Cell *cell) {
    M5.Lcd.fillRect(cell->x, cell->y, cell->w, cell->h, cell->color);
}

void render_cursor(Cursor *cursor) {
    M5.Lcd.drawRect(cursor->dx, cursor->dy, cursor->w, cursor->h, cursor->color);
}

void render_game(Board *board, Cursor *cursor) {
    for (int i = 0; i < board->cols * board->rows; i++) {
        Cell *cell = &board->grid[i];
        render_cell(cell);

        if (cursor->x == cell->x &&
            cursor->y == cell->y) {
            render_cursor(cursor);
        }
    }
}
