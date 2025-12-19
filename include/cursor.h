#ifndef CURSOR_H_
#define CURSOR_H_

#include "state.h"

#define MIN_TILT 0.30

void move_cursor(InputState *input);
void rotate_cursor(InputState *input);

#endif