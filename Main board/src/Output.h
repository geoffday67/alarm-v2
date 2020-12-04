#ifndef __OUTPUT__
#define __OUTPUT__

#include <U8g2lib.h>

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define TOP_MARGIN      8
#define BOTTOM_MARGIN   8
#define LEFT_MARGIN     8
#define RIGHT_MARGIN    8
#define GRID_WIDTH      56
#define GRID_HEIGHT     56
#define LEFT_BUTTON_X   4
#define LEFT_BUTTON_Y   4//180
#define CENTRE_BUTTON_X (SCREEN_WIDTH - GRID_WIDTH) / 2
#define CENTRE_BUTTON_Y 4//180
#define RIGHT_BUTTON_X  260
#define RIGHT_BUTTON_Y  4//180
#define SCREEN_CENTRE   -1
#define SCREEN_CHAR_WIDTH       26
#define SCREEN_CHAR_HEIGHT      11

#define CENTRED         -1

#define FONT_LARGE      1
#define FONT_MEDIUM     2
#define FONT_SMALL      3
#define FONT_TIME       4
#define FONT_ALARM      5

#define FONT_NORMAL     1
#define FONT_BOLD       2

class classOutput {
private:
    int fontSize;
    int fontStyle;

public:
    classOutput();
    virtual ~classOutput();
    virtual void begin();
    virtual void clear();
    virtual void flush();
    virtual void addText(int x, int y, const char *ptext);
    virtual void addText(int x, int y, char c);
    virtual void setSize(int size);
    virtual void addFooter(const char *pleft, const char *pright);
    virtual void addTitle(const char *ptitle);
    virtual int getCharWidth();
    virtual int getCharHeight();
    virtual int getDisplayWidth();
    virtual int getBufferSize();
    virtual void drawBitmap(void *pdata);
 
private:
    U8G2_SSD1306_128X64_NONAME_F_SW_I2C *pdisplay;
    int displayWidth, displayHeight;
};

extern classOutput Output;

#endif