#include <M5Unified.h>
#include <EEPROM.h>

#include "../include/render.h"
#include "../include/state.h"
#include "../include/game.h"

Board* board;
InputState* input;
GameState* gs;


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

    board = init_board(INIT_ROWS, INIT_COLS);
    input = init_input();
    gs = init_game();
}

// The loop routine keeps looping as long as the controller is on
void loop() {
    M5.update();

    read_input(input);
    update_game(gs, board, input);
    render_game(gs, board, input);

    delay(200);
}
