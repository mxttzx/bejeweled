#include <M5Unified.h>
#include <EEPROM.h>

#include "../include/render.h"
#include "../include/state.h"

#define SCREEN_WIDTH 135
#define SCREEN_HEIGHT 240


Board* board;
InputState* input;

// The setup routine runs once when M5StickC starts up
void setup() {
    // Initialize the M5StickC object
    M5.begin();
    M5.Imu.init();
    M5.Lcd.clear();
    Serial.begin(115200);
    Serial.flush();
    EEPROM.begin(512);

    srand((unsigned) time(0));

    board = init_board();
    input = init_input();
}

// The loop routine keeps looping as long as the controller is on
void loop() {
    M5.update();

    read_input(input);
    render_game(board, input);
    update_game(board, input);

    delay(200); // Give player enough time to react to movement
}
