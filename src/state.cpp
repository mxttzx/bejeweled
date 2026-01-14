#include "input.h"
#include "save.h"


void swap(Cell *c1, Cell *c2) {
    if (!c1->init || !c2->init) return; // Switching dead cells

    uint16_t temp = c1->color;
    c1->color = c2->color;
    c2->color = temp;
}

// Disable a cell by turning it off and removing its color
void disable_cell(Cell *cell) {
    cell->init = false;
    cell->color = 0;
}

// Clear the scheduling after every stabalization is complete
void clear_scheduled(Board *board) {
    for (int i = 0; i < board->rows * board->cols; i++) {
        board->grid[i].scheduled = false;
    }
}

// Remove the cells that are scheduled for deletion
void remove_scheduled(Board *board) {
    for (int i = 0; i < board->rows * board->cols; i++) {
        Cell *c = &board->grid[i];
        if (!c->scheduled) continue;
        disable_cell(c);
        c->scheduled = false;
    }
}

// A singular function that scans an array of cell pointers for 3 in a row of same color
// Returns the amount of cells that need removing
uint8_t scan(Cell **cells, uint8_t size) {
    uint8_t accum = 0; // Total number of accumulated cells to be removed
    uint8_t run = 1; // Run is automatically 1 for first cell

    for (uint8_t i = 1; i <= size; i++) {
        if (i < size) {
            // Transitivity c[i] = c[i - 1] && c[i - 2] = c[i - 1] -> c[i] = c[i - 2]
            Cell *c1 = cells[i];
            Cell *c2 = cells[i - 1];

            if (c1->init && c2->init &&
                c1->color == c2->color) {
                run++; // Increment if same color is found
                continue;
            }
        }

        if (run >= 3) {
            accum += run;
            // Schedule all cells in the run for deletion
            for (uint8_t j = 0; j < run; j++) {
                cells[i - j - 1]->scheduled = true;
            }
        }
        run = 1;
    }
    return accum;
}

// We use a double for loop to build an array of cells that can be scanned
uint8_t scan_row(Board *board) {
    uint8_t accum = 0;
    Cell *cells[board->cols];

    // Iterate over y first then x to collect row cells
    for (uint8_t y = 0; y < board->rows; y++) {
        for (uint8_t x = 0; x < board->cols; x++) {
            cells[x] = &board->grid[get_idx(board, x, y)];
        }
        accum += scan(cells, board->cols);
    }

    return accum;
}

uint8_t scan_col(Board *board) {
    uint8_t accum = 0;
    Cell *cells[board->rows];

    // Iterate over x first then y to collect column cells
    for (uint8_t x = 0; x < board->cols; x++) {
        for (uint8_t y = 0; y < board->rows; y++) {
            cells[y] = &board->grid[get_idx(board, x, y)];
        }
        accum += scan(cells, board->rows);
    }

    return accum;
}

// Make a column cascade downwards
// We look for all the cells above the uninitialized cells and switch them
void cascade(Board *board) {
    for (uint8_t x = 0; x < board->cols; x++) {
        int row = board->rows - 1;

        for (int y = board->rows - 1; y >= 0; y--) {
            Cell *c1 = &board->grid[get_idx(board, x, y)];

            // Keep looking for initialized cell to switch downwards
            if (!c1->init) continue;

            if (y != row) {
                // Switch the first initialized we come across with the first uninitialized cell
                Cell *c2 = &board->grid[get_idx(board, x, row)];
                c2->color = c1->color;
                c2->init = true;
            }

            row--;
        }

        // Disable all cells above the checked row
        // This gets filled back up with resupply later in the loop
        for (int y = row; y >= 0; y--) {
            disable_cell(&board->grid[get_idx(board, x, y)]);
        }
    }
}

// Mark a span of cells as inline and to be removed
// This only marks cells and does not delete
// Deleting right away causes the game to not remove certain rows or columns that were supposed to be removed
uint8_t mark_inline(Board *board) {
    uint8_t removed = 0;

    removed += scan_col(board);
    removed += scan_row(board);

    return removed;
}

// Return a boolean to indicate if we are allowed to swap or not
bool stabilize(GameState *gs, Board *board) {
    clear_scheduled(board);
    uint8_t removed = mark_inline(board);

    if (gs->game_mode == HARD && removed == 0) return false;

    while (removed > 0) {
        remove_scheduled(board); // We clear the scheduled cells
        cascade(board); // Move the cells down
        resupply(board); // Add new cells to the board

        // Check if a level has been won
        // update_score returns true if points threshold has been met
        if (update_score(gs, removed)) {
            gs->game_won = true;
            uint8_t rows = get_dims(4); // 4 different dimensions to choose from
            uint8_t cols = get_dims(4);

            // If the color amount is gte 6 then we reset the color amount to 5, otherwise increment
            uint8_t colors = board->colors >= 6
                ? 5
                : board->colors + 1;

            new_board(board, rows, cols, colors);
            return true;
        }

        clear_scheduled(board);
        removed = mark_inline(board);
    }

    return true;
}

void toggle_menu(GameState *gs) {
    switch (gs->view_mode) {
        // Both startup and menu return to the game when exiting
        case STARTUP:
        case MENU:
            gs->view_mode = GAME;
            gs->actv_item = -1;
            break;
        case GAME:
            gs->view_mode = MENU;
            gs->actv_item = 0;
            break;
    }
    M5.Lcd.clear();
}

// Handle the actions associated with each menu item
void select_menu_item(GameState *gs, Board *board, InputState *input) {
    switch (gs->actv_item) {
        case 0: toggle_menu(gs); break;
        // Load game here
        case 1:
            reset_game(gs);
            reset_input(input);
            load_game(gs, board);

            toggle_menu(gs);
            break;
        // Save game here
        case 2:
            save_game(gs, board);
            toggle_menu(gs);
            break;
        case 3:
            GameMode game_mode = gs->game_mode; // Remember the current game mode when resetting game

            reset_board(board); 
            reset_input(input);
            reset_game(gs);

            gs->game_mode = game_mode;
            gs->actv_item = -1;

            toggle_menu(gs);
            break;
    }
}

// Handle the actions associated with each menu startup item
void select_startup_item(GameState *gs) {
    switch (gs->actv_item) {
        case 0: gs->game_mode = EASY; break;
        case 1: gs->game_mode = HARD; break;
    }
    toggle_menu(gs);
}

// Handle the input when in game view
void handle_game_view(GameState *gs, Board *board, InputState *input) {
    if (input->btn_c) toggle_menu(gs);
    if (input->btn_b) rotate_cursor(board, input);
    if (input->btn_a && gs->moves > 0) {
        Cell *c1 = &board->grid[get_idx(board, input->cursor[0].x, input->cursor[0].y)];
        Cell *c2 = &board->grid[get_idx(board, input->cursor[1].x, input->cursor[1].y)];

        if (c1->color != c2->color) {
            swap(c1, c2);
            gs->moves--;
        }

        // If we aren't allowed to swap, swap them back and refund move
        if (!stabilize(gs, board)) {
            swap(c2, c1);
            gs->moves++;
        }

        if (gs->moves == 0) gs->game_over = true;
    }
    move_cursor(board, input);
}

void navigate_menu_item(GameState *gs, uint8_t view_size) {
    gs->actv_item = (gs->actv_item + view_size + 1) % view_size;
    M5.Lcd.clear();
}

void handle_menu_view(GameState *gs, Board *board, InputState *input) {
    if (input->btn_b) navigate_menu_item(gs, 4);
    if (input->btn_a) select_menu_item(gs, board, input);
}

void handle_startup_view(GameState *gs, Board *board, InputState *input) {
    if (input->btn_b) navigate_menu_item(gs, 2);
    if (input->btn_a) select_startup_item(gs);
}

void update_game(GameState *gs, Board *board, InputState *input) {
    switch (gs->view_mode) {
        case GAME: handle_game_view(gs, board, input); break;
        case MENU: handle_menu_view(gs, board, input); break;
        case STARTUP: handle_startup_view(gs, board, input); break;
    }
    reset_input_internal(input);
}

void read_input(InputState *input) {
    if (M5.BtnA.wasPressed()) input->btn_a = true;
    if (M5.BtnB.wasPressed()) input->btn_b = true;

    if (input->btn_a && input->btn_b) input->btn_c = true;

    M5.Imu.getAccelData(&input->ax, &input->ay, &input->az);
}
