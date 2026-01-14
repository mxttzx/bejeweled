#include "save.h"

// We write to EEPROM 2 bytes at a time to write a singular color
void write_uint16(uint32_t addr, uint16_t val) {
    EEPROM.write(addr, val & 0xFF); // Write last 8 bits
    EEPROM.write(addr + 1, val >> 8); // Shift 8 bits and write remaining bits
}

// We read from EEPROM 2 bytes at a time to read in a singular color
uint16_t read_uint16(uint32_t addr) {
    return EEPROM.read(addr) | EEPROM.read(addr + 1) << 8;
}

// Saves the state of the game and board by writing to EEPROM
void save_game(GameState *gs, Board *board) {
    uint32_t addr = 0;

    write_uint16(addr, gs->score);
    addr += 2; // Move two address spaces because of 16-bit size

    EEPROM.write(addr++, gs->level);
    EEPROM.write(addr++, gs->moves);
    EEPROM.write(addr++, gs->game_mode);

    EEPROM.write(addr++, board->rows);
    EEPROM.write(addr++, board->cols);
    EEPROM.write(addr++, board->colors);

    for (uint8_t y = 0; y < board->rows; y++) {
        for (uint8_t x = 0; x < board->cols; x++) {
            Cell *c = &board->grid[get_idx(board, x, y)];

            write_uint16(addr, c->color);
            addr += 2;
        }
    }

    EEPROM.commit();
}

// Load game and board by reading from EEPROM
void load_game(GameState *gs, Board *board) {
    uint32_t addr = 0;

    gs->score = read_uint16(addr);
    addr += 2;

    gs->level = EEPROM.read(addr++);
    gs->moves = EEPROM.read(addr++);
    gs->game_mode = (GameMode)EEPROM.read(addr++);

    gs->actv_item = -1;
    gs->points = gs->level == 0 ? 100 : gs->level * 100;

    uint8_t rows = EEPROM.read(addr++);
    uint8_t cols = EEPROM.read(addr++);
    uint8_t colors = EEPROM.read(addr++);


    new_board(board, rows, cols, colors);

    for (uint8_t y = 0; y < board->rows; y++) {
        for (uint8_t x = 0; x < board->cols; x++) {
            board->grid[get_idx(board, x, y)].color = read_uint16(addr);
            addr += 2;
        }
    }

}
