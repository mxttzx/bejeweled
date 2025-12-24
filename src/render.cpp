#include "../include/render.h"

const char *menu_items[4] = {"BACK", "LOAD", "SAVE", "RESET"}; // Compiler keeps warning me about const
const char *startup_items[2] = {"EASY", "HARD"};

void cursor_fn(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    M5.Lcd.drawRect(x * CELL_WIDTH, BOARD_OFFSET + y * CELL_HEIGHT, w, h, color);
    M5.Lcd.drawRect(x * CELL_WIDTH + 1, BOARD_OFFSET + y * CELL_HEIGHT + 1, w - 2, h - 2, color);
}

void fill_fn(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    M5.Lcd.fillRect(x * CELL_WIDTH, BOARD_OFFSET + y * CELL_HEIGHT, w, h, color);
}

void render_cell(Cell *cell, Draw draw) {
    draw(cell->x, cell->y, cell->w, cell->h, cell->color);
}

void render_state(GameState *gs) {
    M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, BOARD_OFFSET, BLACK);

    M5.Lcd.setCursor(MARGIN_X, MARGIN_Y);
    M5.Lcd.printf("Score: %d\n", gs->score);

    M5.Lcd.setCursor(MARGIN_X, MARGIN_Y + OFFSET_Y);
    M5.Lcd.printf("Moves:%d\n", gs->moves);
}

void render_menu(GameState *gs, const char *items[], uint8_t size) {
    for (int i = 0; i < size; i++) {
        uint8_t font = gs->actv_item == i ? MENU_ITM_ACTV : MENU_ITM_INACTV;
        M5.Lcd.drawString(items[i], MARGIN_X, i * MARGIN_Y + OFFSET_Y, font);
    }
}

void render_board(GameState *gs, Board *board, InputState *input) {
    for (int i = 0; i < board->cols * board->rows; i++) {
        Cell *cell = &board->grid[i];
        render_cell(cell, fill_fn);
    }

    render_cell(&input->cursor[0], cursor_fn);
    render_cell(&input->cursor[1], cursor_fn);
    render_state(gs);
}

void render_game(GameState *gs, Board *board, InputState *input) {
    switch (gs->view_mode) {
        case GAME: render_board(gs, board, input); break;
        case MENU: render_menu(gs, menu_items, 4); break;
        case STARTUP: render_menu(gs, startup_items, 2); break;
    }
}