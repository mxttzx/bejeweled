#include "../include/render.h"

const char *menu_items[] = { "New game", "Load", "Save", "Back" };

void outline(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t color) {
    M5.Lcd.drawRect(x * CELL_WIDTH, GRID_OFFSET + y * CELL_HEIGHT, w, h, color);
}

void fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t color) {
    M5.Lcd.fillRect(x * CELL_WIDTH, GRID_OFFSET + y * CELL_HEIGHT, w, h, color);
}

void render_cell(Cell *cell, Draw draw) {
    draw(cell->x, cell->y, cell->w, cell->h, cell->color);
}

void render_state(GameState *gs) {
    M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, GRID_OFFSET, BLACK);
    M5.Lcd.setCursor(0, 0);

    M5.Lcd.printf("Score: %d\n", gs->score);
    M5.Lcd.printf("Moves: %d\n", gs->moves);
    M5.Lcd.printf("Points: %d\n", gs->points);
    M5.Lcd.printf("Level: %d\n", gs->level);
}

void render_menu(GameState *gs) {
    for (int i = 0; i < MENU_ITM_SIZE; i++) {
        uint8_t font = gs->actv_item == i ? 2 : 0;
        M5.Lcd.drawString(menu_items[i], MARGIN_X, i * MARGIN_Y + OFFSET_Y, font);
    }
}

void render_board(GameState *gs, Board *board, InputState *input) {
    Cell *cursor = input->cursor;

    for (int i = 0; i < board->cols * board->rows; i++) {
        Cell *cell = &board->grid[i];
        render_cell(cell, fill);
    }

    render_cell(&cursor[0], outline);
    render_cell(&cursor[1], outline);

    render_state(gs);
}

void render_game(GameState *gs, Board *board, InputState *input) {
    switch (gs->view_mode) {
        case GAME:
            render_board(gs, board, input);
            break;

        case MENU:
            render_menu(gs);
            break;
    }
}