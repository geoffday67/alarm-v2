#include "Screen.h"
#include "Output.h"

#include "wait.xbm"

Screen::Screen () {
}

Screen::~Screen() {}

void Screen::showWaiting() {
    Output.clear();
    Output.drawBitmap(wait_bits);
    Output.flush();
}
