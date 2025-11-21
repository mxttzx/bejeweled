#include <M5Unified.h>
#include "EEPROM.h"
#include <cstdlib>
#define SCREEN_WIDTH 135
#define SCREEN_HEIGHT 240

#include "game.h"

Board* board;
Cursor* cursor;

// The setup routine runs once when M5StickC starts up
void setup() {
    // Initialize the M5StickC object
    M5.begin();
    M5.Imu.init();
    M5.Lcd.clear();
    Serial.begin(115200);
    Serial.flush();
    EEPROM.begin(512);

    srand((unsigned) time(NULL));

    board = init_board();
    cursor = init_cursor();
}


// The loop routine keeps looping as long as the controller is on
void loop() {
    update_game();
    render_game(board, cursor);
}