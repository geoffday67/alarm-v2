#include "Output.h"

classOutput Output;

classOutput::classOutput() {}

classOutput::~classOutput() {}

void classOutput::begin() {
  pdisplay = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, 5, 4);
  pdisplay->begin();
  pdisplay->sendBuffer();
  pdisplay->setDisplayRotation(U8G2_R2);
  setSize(FONT_MEDIUM);
  pdisplay->setFontMode(0);
  //pdisplay->setFontPosTop();
  displayWidth = pdisplay->getDisplayWidth();
  displayHeight = pdisplay->getDisplayHeight();
}

void classOutput::clear() {
  pdisplay->clearBuffer();
  //pdisplay->drawFrame(0, 0, displayWidth, displayHeight);
}

void classOutput::flush() {
  pdisplay->sendBuffer();
}

void classOutput::addText(int x, int y, char c) {
  char s[2];
  s[0] = c;
  s[1] = 0;
  addText(x, y, s);
}

void classOutput::addText(int x, int y, const char *ptext) {
  int useX, useY;

  pdisplay->setFontPosTop();

  if (x == CENTRED) {
    useX = (displayWidth - pdisplay->getStrWidth(ptext)) / 2;
  } else {
    useX = x;
  }

  if (y == CENTRED) {
    useY = (displayHeight + pdisplay->getMaxCharHeight()) / 2;
  } else {
    useY = y;
  }
  pdisplay->drawStr(useX, useY, ptext);
}

void classOutput::setSize(int size) {
  fontSize = size;

  switch (size) {
    case FONT_TIME:
      pdisplay->setFont(u8g2_font_profont29_mn);
      break;
    case FONT_ALARM:
      pdisplay->setFont(u8g2_font_profont22_mn);
      break;
    case FONT_LARGE:
      pdisplay->setFont(u8g2_font_profont29_tr);
      break;
    case FONT_MEDIUM:
      pdisplay->setFont(u8g2_font_profont17_tr);
      break;
    case FONT_SMALL:
      pdisplay->setFont(u8g2_font_profont15_tr);
      break;
  }
}

void classOutput::addFooter(const char *pleft, const char *pright) {
  setSize(FONT_SMALL);
  // Set baseline alignment, this means that text will be missing its descenders
  pdisplay->setFontPosBaseline();
  pdisplay->drawStr(0, displayHeight, pleft);
  pdisplay->drawStr(displayWidth - pdisplay->getStrWidth(pright), displayHeight, pright);
}

void classOutput::addTitle(const char *ptitle) {
  char s [32];
  sprintf (s, ">> %s <<", ptitle);
  setSize(FONT_SMALL);
  //pdisplay->drawHLine(0, pdisplay->getMaxCharHeight() / 2, displayWidth);
  addText(CENTRED, 0, s);
}

int classOutput::getCharWidth() {
  return pdisplay->getMaxCharWidth();
}

int classOutput::getCharHeight() {
  return pdisplay->getMaxCharHeight();
}

int classOutput::getDisplayWidth() {
  return pdisplay->getDisplayWidth();
}

int classOutput::getBufferSize() {
  return 8 * pdisplay->getBufferTileHeight() * pdisplay->getBufferTileWidth();
}

void classOutput::drawBitmap(void *pdata) {
  pdisplay->drawXBM(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (uint8_t*) pdata);
}