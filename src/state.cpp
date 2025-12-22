#include "../include/state.h"
#include "../include/game.h"

#include "cursor.h"
#include "render.h"

void reset_input_internal(InputState *input) {
    input->btn_a = false;
    input->btn_b = false;
    input->btn_c = false;

    input->ax = 0;
    input->ay = 0;
    input->az = 0;
}

void reset_input(InputState *input) {
   for (int i = 0; i < CURSOR_SIZE; i++) {
        Cell *cell = &input->cursor[i];

        cell->w = CELL_WIDTH;
        cell->h = CELL_HEIGHT;
        cell->color = 0xFFFFFF;
    }

    // c1: [0, 0], c2: [1, 0]
    input->cursor[1].x = 1;

    reset_input_internal(input);
}

InputState* init_input() {
    InputState *input = (InputState *)malloc(sizeof(InputState));
    input->cursor = (Cell *)calloc(CURSOR_SIZE, sizeof(Cell));
    reset_input(input);

    return input;
}


void swap(Cell *c1, Cell *c2) {
    if (!c1->init || !c2->init) return; // Switching dead cells

    uint32_t temp = c1->color;
    c1->color = c2->color;
    c2->color = temp;
}

void disable_cell(Cell *cell) {
    cell->init = false;
    cell->color = 0;
}

bool row_inline(Board *board, uint8_t row) {
    uint32_t prev = 0;
    uint8_t start = 0;
    uint8_t collect = 0;

    for (uint8_t x = 0; x <= board->cols; x++) {
        uint32_t next = 0;

        if (x < board->cols) {
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
                collect += run;
            }

            prev = next;
            start = x;
        }
    }
    return collect;
}

bool col_inline(Board *board, uint8_t col) {
    uint32_t prev = 0;
    uint8_t start = 0;
    uint8_t collect = 0;

    for (uint8_t y = 0; y <= board->rows; y++) {
        uint32_t next = 0;

        if (y < board->rows) {
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
                collect += run;
            }

            prev = next;
            start = y;
        }
    }
    return collect;
}

void cascade(Board *board) {
    for (uint8_t x = 0; x < board->cols; x++) {
        int row = board->rows - 1;

        for (int y = board->rows - 1; y >= 0; y--) {
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

uint8_t mark_inline(Board *board) {
    uint8_t removed = 0;

    for (uint8_t x = 0; x < board->cols; x++) {
        removed += col_inline(board, x);
    }

    for (uint8_t y = 0; y < board->rows; y++) {
        removed += row_inline(board, y);
    }

    return removed;
}

void stabilize(GameState *gs, Board *board) {
    uint8_t removed;

    while ((removed = mark_inline(board)) > 0) {
        update_score(gs, removed);
        cascade(board);
        resupply(board);
    }
}

void toggle_menu(GameState *gs) {
    if (gs->view_mode == MENU) {
        gs->view_mode = GAME;
        gs->actv_item = -1;
    } else {
        gs->view_mode = MENU;
        gs->actv_item = 0;
    }
    M5.Lcd.clear();
}

void up_menu_item(GameState *gs) {
    gs->actv_item = --gs->actv_item % MENU_ITM_SIZE;
    M5.Lcd.clear();
}

void select_menu_item(GameState *gs, Board *board, InputState *input) {
    switch (gs->actv_item) {
        case 0:
            toggle_menu(gs);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            reset_board(board); 
            reset_input(input);
            reset_game(gs);

            toggle_menu(gs);
            break;
    }
}

void handle_game_view(GameState *gs, Board *board, InputState *input) {
    if (input->btn_c) toggle_menu(gs);
    if (input->btn_a) rotate_cursor(board, input);
    if (input->btn_b && gs->moves > 0) {
        Cell *c1 = &board->grid[get_idx(board, input->cursor[0].x, input->cursor[0].y)];
        Cell *c2 = &board->grid[get_idx(board, input->cursor[1].x, input->cursor[1].y)];

        swap(c1, c2);
        stabilize(gs, board);

        gs->moves--;
    }
    move_cursor(board, input);
}

void handle_menu_view(GameState *gs, Board *board, InputState *input) {
    if (input->btn_a) up_menu_item(gs);
    if (input->btn_b) select_menu_item(gs, board, input);
}

void update_game(GameState *gs, Board *board, InputState *input) {
    switch (gs->view_mode) {
        case GAME:
            handle_game_view(gs, board, input);
            break;
        case MENU:
            handle_menu_view(gs, board, input);
            break;
    }
    reset_input_internal(input);
}

void read_input(InputState *input) {
    if (M5.BtnA.wasPressed()) input->btn_a = true;
    if (M5.BtnB.wasPressed()) input->btn_b = true;

    if (input->btn_a && input->btn_b) input->btn_c = true;

    M5.Imu.getAccelData(&input->ax, &input->ay, &input->az);
}
