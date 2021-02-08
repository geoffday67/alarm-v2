#include "Screen.h"
#include "Output.h"

#include "wait.xbm"

Screen::Screen () {
}

Screen::~Screen() {}

void Screen::activate() {
    active = true;
    onActivate();
}

void Screen::onActivate() {
}

void Screen::onDeactivate() {
}

void Screen::deactivate() {
    active = false;
    onDeactivate();
}

void Screen::showWaiting() {
    Output.clear();
    Output.drawBitmap(wait_bits);
    Output.flush();
}
